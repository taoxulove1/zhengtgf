"""Build the repository's eight native Zebra services in an isolated Linux tree."""

import argparse
import os
from pathlib import Path
import platform
import shlex
import shutil
import struct
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src" / "server"
OUTPUT = ROOT / "build" / "p3-native"
SERVICES = (
    "SuperServer", "FLServer", "RecordServer", "BillServer",
    "MiniServer", "SessionServer", "ScenesServer", "GatewayServer",
)
MODULES = ("base/EncDec", "base", *SERVICES)
FLAGS = "-m32 -O2 -D_USE_EPOLL_ -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG"
PREREQUISITES = ("make", "gcc", "g++", "ar", "ranlib", "mysql_config", "xml2-config")


def elf32_executable(path):
    with path.open("rb") as file:
        header = file.read(52)
    return (
        len(header) == 52
        and header[:7] == b"\x7fELF\x01\x01\x01"
        and struct.unpack_from("<H", header, 16)[0] in (2, 3)
        and struct.unpack_from("<H", header, 18)[0] == 3
        and os.access(path, os.X_OK)
    )


def preflight(directory):
    if platform.system() != "Linux":
        raise ValueError(f"Native Zebra build requires Linux; detected {platform.system()}.")
    missing = [command for command in PREREQUISITES if not shutil.which(command)]
    if missing:
        raise ValueError("Missing Linux build tools: " + ", ".join(missing))
    for module in MODULES:
        if not (SOURCE / module / "Makefile").is_file():
            raise ValueError(f"Missing source Makefile: {module}")

    flags = []
    for helper, option in (
        ("mysql_config", "--cflags"), ("xml2-config", "--cflags"),
        ("mysql_config", "--libs_r"), ("xml2-config", "--libs"),
    ):
        result = subprocess.run([helper, option], text=True, capture_output=True, check=False)
        if result.returncode:
            raise ValueError(f"Dependency query failed: {helper} {option}")
        flags.extend(shlex.split(result.stdout))

    probe = directory / "link-check"
    subprocess.run(
        ["g++", "-m32", "-x", "c++", "-", "-o", str(probe), *flags,
         "-L/usr/local/lib", "-llog4cxx", "-lgd", "-ljpeg", "-lssl", "-lz",
         "-lpthread", "-llua", "-lluabind"],
        input="int main() { return 0; }\n", text=True, check=True,
    )
    if not elf32_executable(probe):
        raise ValueError("The dependency probe did not produce an x86 Linux ELF executable")


def build(work, output, jobs):
    if output.exists():
        raise ValueError(f"Output already exists: {output}")
    source = work / "server"
    shutil.copytree(
        SOURCE, source,
        ignore=shutil.ignore_patterns(".svn", ".backup", "*.tar.gz", "*.o", "*.a", "*.so", ".*.d"),
    )
    for service in SERVICES:
        (source / service / service).unlink(missing_ok=True)
    for module in MODULES:
        print(f"Building {module}", flush=True)
        subprocess.run(
            ["make", "-C", str(source / module), f"-j{jobs}", f"ZEBRA_COMPILE_OPT={FLAGS}"],
            check=True,
        )

    release = work / "release"
    release.mkdir()
    for service in SERVICES:
        binary = source / service / service
        if not elf32_executable(binary):
            raise ValueError(f"{service}: build did not produce an x86 Linux ELF executable")
        shutil.copy2(binary, release / service)
    release.rename(output)
    print(f"Built {len(SERVICES)} services in {output}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="Only check the native toolchain and libraries")
    parser.add_argument("--jobs", default=1, type=int)
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error("--jobs must be positive")
    try:
        with tempfile.TemporaryDirectory(prefix="zebra-p3-check-") as directory:
            preflight(Path(directory))
        print("P3 x86 Linux compiler and link dependencies passed.")
        if not args.check:
            OUTPUT.parent.mkdir(parents=True, exist_ok=True)
            with tempfile.TemporaryDirectory(prefix="p3-native-", dir=OUTPUT.parent) as directory:
                build(Path(directory), OUTPUT, args.jobs)
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        parser.exit(1, f"P3 build failed: {error}\n")


if __name__ == "__main__":
    main()
