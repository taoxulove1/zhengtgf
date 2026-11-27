#!/usr/bin/env python3
"""Exercise the native FLServer with normal PING requests in a fresh Linux namespace."""

import hashlib
import os
from pathlib import Path
import re
import shutil
import signal
import socket
import struct
import subprocess
import sys
import tempfile
import time
import uuid


COUNT = 100
ZONE_ID = 0x00010001
PING_PORT = 7002
DB_PORT = 13306
PAYLOAD = struct.Struct("<BBI")
FRAME_HEADER = struct.Struct("<I")
REQUEST = PAYLOAD.pack(30, 2, 0) + struct.pack("<I", ZONE_ID)
REFERENCE_PREFIX = PAYLOAD.pack(30, 1, 0) + struct.pack("<I", ZONE_ID) + bytes(16)


def find_program(*names):
    for name in names:
        path = shutil.which(name)
        if path:
            return path
    raise RuntimeError("Program not installed: " + " or ".join(names))


def digest(path):
    sha = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(65536), b""):
            sha.update(chunk)
    return sha.hexdigest()


def wait_until(process, description, check, seconds=25):
    deadline = time.monotonic() + seconds
    while time.monotonic() < deadline:
        if process.poll() is not None:
            raise RuntimeError("{} exited with status {}".format(description, process.returncode))
        result = check()
        if result is not None:
            return result
        time.sleep(0.2)
    raise RuntimeError("Timed out waiting for " + description)


def stop(process):
    if process is not None and process.poll() is None:
        process.terminate()
        try:
            process.wait(timeout=6)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()


def listener_identity(process, binary):
    status = subprocess.check_output(["ss", "-lntp"], universal_newlines=True)
    pid_pattern = re.compile(r"pid={}\b".format(process.pid))
    owned_ports = set()
    for line in status.splitlines():
        fields = line.split()
        if len(fields) > 4 and fields[0] == "LISTEN" and pid_pattern.search(line):
            owned_ports.add(fields[3].rsplit(":", 1)[-1])
    if not {"7001", str(PING_PORT)} <= owned_ports:
        return None
    proc = Path("/proc") / str(process.pid)
    if not os.path.samefile(str(proc / "exe"), str(binary)):
        raise RuntimeError("Listeners belong to a different executable")
    start_ticks = (proc / "stat").read_text().rsplit(")", 1)[1].split()[19]
    return process.pid, start_ticks


def read_exact(conn, size):
    data = bytearray()
    while len(data) < size:
        part = conn.recv(size - len(data))
        if not part:
            raise RuntimeError("Connection closed while reading a PING reply")
        data.extend(part)
    return bytes(data)


def framed(data):
    return FRAME_HEADER.pack(len(data)) + data


def exercise(conn, out, count=COUNT):
    # In an empty gateway list the server never assigns the last byte, state.
    # The comparison excludes only that byte. The original frame is saved.
    expected_size = len(REFERENCE_PREFIX) + 1
    hello = framed(PAYLOAD.pack(30, 3, 0))
    request_frame = framed(REQUEST)
    conn.sendall(hello)
    time.sleep(1)
    for index in range(count):
        conn.sendall(request_frame)
        header = read_exact(conn, FRAME_HEADER.size)
        if FRAME_HEADER.unpack(header)[0] != expected_size:
            raise RuntimeError("Unexpected PING reply header at exchange {}".format(index + 1))
        body = read_exact(conn, expected_size)
        if index == 0:
            (out / "ping-hello.bin").write_bytes(hello)
            (out / "ping-request.bin").write_bytes(request_frame)
            (out / "ping-reply.bin").write_bytes(header + body)
            (out / "ping-source-reference.bin").write_bytes(
                FRAME_HEADER.pack(expected_size) + REFERENCE_PREFIX)
            (out / "ping-reply-compared.bin").write_bytes(header + body[:-1])
        if body[:-1] != REFERENCE_PREFIX:
            raise RuntimeError("PING reply differs from the source reference at exchange {}".format(index + 1))
        if (index + 1) % 25 == 0:
            print("[PROGRESS][EXCHANGES] {}/{}".format(index + 1, count), flush=True)
    print("[OK][SOURCE_BYTES] compared={} raw={} skipped=state".format(
        FRAME_HEADER.size + len(REFERENCE_PREFIX), FRAME_HEADER.size + expected_size), flush=True)


def run_inside(binary, parent_network, out):
    if os.readlink("/proc/self/ns/net") == parent_network:
        raise RuntimeError("Expected a private network namespace")
    subprocess.run(["ip", "link", "set", "lo", "up"], check=True)
    print("[OK][NETNS] Isolated loopback ready", flush=True)

    database = find_program("mariadbd", "mysqld", "/usr/libexec/mysqld", "/usr/sbin/mysqld")
    installer = find_program("mariadb-install-db", "mysql_install_db")
    client = find_program("mariadb", "mysql")
    tempdir = out / "tmp"
    tempdir.mkdir()
    socket_path = out / "db.sock"
    password = uuid.uuid4().hex[:12]
    datadir = out / "db"
    env = dict(os.environ)
    env.pop("MYSQL_PWD", None)
    env["TMPDIR"] = str(tempdir)
    user = subprocess.check_output(["id", "-un"], universal_newlines=True).strip()
    db = service = None
    config = out / "config.xml"
    try:
        print("[STEP][DB] Initialize disposable MariaDB data", flush=True)
        with (out / "db-init.log").open("wb") as log:
            subprocess.run(
                [installer, "--no-defaults", "--basedir=/usr",
                 "--datadir=" + str(datadir), "--user=" + user, "--force"],
                cwd=str(out), env=env, stdout=log, stderr=subprocess.STDOUT,
                check=True, timeout=90,
            )
        with (out / "db.log").open("wb") as log:
            db = subprocess.Popen(
                [database, "--no-defaults", "--basedir=/usr", "--datadir=" + str(datadir),
                 "--user=" + user, "--socket=" + str(socket_path), "--pid-file=" + str(out / "db.pid"),
                 "--bind-address=127.0.0.1", "--port=" + str(DB_PORT), "--skip-name-resolve",
                 "--tmpdir=" + str(tempdir), "--innodb-buffer-pool-size=32M"],
                cwd=str(out), env=env, stdout=log, stderr=subprocess.STDOUT,
            )
        sql_client = [client, "--no-defaults", "--protocol=SOCKET", "--socket=" + str(socket_path),
                      "--user=root", "--batch", "--skip-column-names", "--connect-timeout=2"]

        def database_ready():
            result = subprocess.run(sql_client + ["-e", "SELECT 1"], env=env, timeout=4,
                                    stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
            return True if result.returncode == 0 and result.stdout.strip() == b"1" else None

        wait_until(db, "MariaDB on the private socket", database_ready)
        sql = ("CREATE DATABASE p3_smoke;\n"
               "CREATE USER 'p3_smoke'@'127.0.0.1' IDENTIFIED BY '{}';\n"
               "GRANT SELECT ON p3_smoke.* TO 'p3_smoke'@'127.0.0.1';\n").format(password)
        with (out / "db-setup.log").open("wb") as log:
            result = subprocess.run(sql_client, input=sql.encode("ascii"), env=env, timeout=10,
                                    stdout=log, stderr=subprocess.STDOUT)
            if result.returncode:
                raise RuntimeError("Creating the disposable test schema failed; see db-setup.log")
        print("[OK][DB] Temporary database initialized", flush=True)

        (out / "zoneInfo.xml").write_text("<zoneInfo/>\n", encoding="ascii")
        config.write_text(
            "<Zebra><global/><FLServer>"
            "<mysql>mysql://p3_smoke:{}@127.0.0.1:{}/p3_smoke</mysql>"
            "<login_port>7000</login_port><inside_port>7001</inside_port><ping_port>7002</ping_port>"
            "<zoneInfoFile>zoneInfo.xml</zoneInfoFile><threadPoolCapacity>64</threadPoolCapacity>"
            "<log>info</log><logfilename>flserver-file.log</logfilename>"
            "</FLServer></Zebra>\n".format(password, DB_PORT), encoding="ascii",
        )
        with (out / "flserver.log").open("wb") as log:
            service = subprocess.Popen([str(binary)], cwd=str(out), env=env,
                                       stdout=log, stderr=subprocess.STDOUT)
        before = wait_until(service, "FLServer TCP 7001 and 7002", lambda: listener_identity(service, binary))
        print("[OK][LISTENER] port=7001 pid={} start_ticks={} sha256={}".format(
            *before, digest(binary)), flush=True)
        with socket.create_connection(("127.0.0.1", PING_PORT), timeout=5) as conn:
            exercise(conn, out)
        if service.poll() is not None or listener_identity(service, binary) != before:
            raise RuntimeError("FLServer listener changed during the request loop")
        print("[OK][EXCHANGES] port=7002 requests={} replies={} same_pid={}".format(
            COUNT, COUNT, before[0]), flush=True)
        print("[P3_PENDING][ORIGINAL_BYTES] Compare with an independent original PING capture", flush=True)
    finally:
        stop(service)
        stop(db)
        if config.exists():
            config.unlink()


def build_dir(log):
    lines = log.read_text(encoding="utf-8", errors="replace").splitlines()
    for tag in ("[P3_BUILD_READY] ", "[WORKDIR] "):
        candidates = [line[len(tag):] for line in lines if line.startswith(tag)]
        if candidates:
            path = Path(candidates[-1])
            if tag == "[WORKDIR] ":
                path /= "zhengtugf-p3-934bf0d/build/p3-native"
            return path
    raise RuntimeError("No artifact location in " + str(log))


def main(args):
    os.umask(0o077)
    if len(args) == 4 and args[0] == "--inside":
        signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
        run_inside(Path(args[1]), args[2], Path(args[3]))
        return 0
    if len(args) != 1:
        raise RuntimeError("Usage: python3 p3_runtime_smoke.py p3-build-luabind.log")
    binary = (build_dir(Path(args[0])) / "FLServer").resolve(strict=True)
    if not binary.is_file() or not os.access(str(binary), os.X_OK):
        raise RuntimeError("Missing executable FLServer: " + str(binary))
    for tool in ("unshare", "ip", "ss"):
        find_program(tool)
    base = Path(tempfile.mkdtemp(prefix="p3-runtime-", dir=str(Path.cwd())))
    print("[RUNTIME_DIR] {}".format(base), flush=True)
    return subprocess.call(["unshare", "-n", sys.executable, str(Path(__file__).resolve()),
                            "--inside", str(binary), os.readlink("/proc/self/ns/net"), str(base)])


if __name__ == "__main__":
    try:
        sys.exit(main(sys.argv[1:]))
    except (OSError, RuntimeError, subprocess.SubprocessError) as error:
        print("[FAIL][P3_RUNTIME] {}".format(error), file=sys.stderr, flush=True)
        sys.exit(1)
