"""Verify a Zebra MyISAM export and restore on an offline MySQL 4.0 instance."""

import argparse
from collections import Counter
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile


SCHEMA = Path(__file__).resolve().parents[1] / "sql" / "schema.sql"
CREATE_TABLE = re.compile(
    r"^CREATE TABLE `([a-z0-9_]+)` \(.*?^\) ENGINE=MyISAM;", re.MULTILINE | re.DOTALL
)
COLUMN = re.compile(r"^ +`([a-zA-Z0-9_]+)` +[a-zA-Z]+", re.MULTILINE)
LEGACY_TIMESTAMP = ("charbase", "LASTACTIVEDATE")


def bootstrap(legacy_root, data_dir, label, statements):
    process = subprocess.run(
        [
            str(legacy_root / "bin" / "mysqld.exe"),
            "--no-defaults",
            "--console",
            "--bootstrap",
            "--skip-networking",
            "--skip-innodb",
            "--default-character-set=latin1",
            f"--basedir={legacy_root}",
            f"--datadir={data_dir}",
            f"--tmpdir={data_dir}",
            f"--language={legacy_root / 'share' / 'english'}",
            f"--pid-file={data_dir / 'bootstrap.pid'}",
        ],
        cwd=data_dir,
        env={**os.environ, "TZ": "UTC0"},
        input=("\n".join(statements) + "\n").encode("ascii"),
        capture_output=True,
        timeout=60,
        check=False,
    )
    if process.returncode:
        # An SQL error can reproduce input values. Report only the diagnostic code.
        error_codes = re.findall(rb"ERROR:\s*(\d+)", process.stdout + process.stderr)
        codes = ", ".join(code.decode("ascii") for code in error_codes) or "none reported"
        raise RuntimeError(f"{label}: exit {process.returncode}, SQL errors {codes}")


def copy_snapshot(snapshot, target, table_names):
    target.mkdir()
    for suffix in (".frm", ".myd", ".myi"):
        files = [
            path for path in snapshot.iterdir() if path.is_file() and path.suffix.lower() == suffix
        ]
        if {path.stem.lower() for path in files} != table_names or len(files) != len(table_names):
            raise ValueError(f"Snapshot {suffix} files differ from the schema table list")
        for path in files:
            shutil.copyfile(path, target / path.name)
    database_options = snapshot / "db.opt"
    if database_options.is_file():
        shutil.copyfile(database_options, target / database_options.name)


def verification_queries(table_columns):
    for table, columns in table_columns.items():
        fields = []
        for column in columns:
            field = f"`{column}`"
            if (table, column) == LEGACY_TIMESTAMP:
                field = f"UNIX_TIMESTAMP({field})"
            fields.append(f"IF({field} IS NULL, 'N', CONCAT('V', HEX(CONCAT({field}))))")
        yield f"SELECT {', '.join(fields)} INTO OUTFILE 'verify_{table}.tsv' FROM `{table}`;"
    yield "SELECT CHARID, HEX(NAME) INTO OUTFILE 'role_names.tsv' FROM charbase ORDER BY CHARID;"


def transfer_queries(table_columns):
    for table, columns in table_columns.items():
        fields = []
        for column in columns:
            if (table, column) == LEGACY_TIMESTAMP:
                fields.append("DATE_FORMAT(`LASTACTIVEDATE`, '%Y-%m-%d %H:%i:%s')")
            else:
                fields.append(f"`{column}`")
        yield f"SELECT {', '.join(fields)} INTO OUTFILE 'transfer_{table}.tsv' FROM `{table}`;"


def verify(legacy_root, snapshot, scratch):
    definitions = list(CREATE_TABLE.finditer(SCHEMA.read_text(encoding="ascii")))
    table_columns = {match[1]: COLUMN.findall(match[0]) for match in definitions}
    if (
        len(definitions) != len(table_columns)
        or not all(table_columns.values())
        or LEGACY_TIMESTAMP[1] not in table_columns.get(LEGACY_TIMESTAMP[0], [])
    ):
        raise ValueError("Unexpected Zebra schema or missing role timestamp column")
    ddl = [" ".join(match[0].replace("ENGINE=MyISAM", "TYPE=MyISAM").splitlines()) for match in definitions]
    queries = list(verification_queries(table_columns))

    with tempfile.TemporaryDirectory(prefix="zebra-restore-", dir=scratch) as temporary:
        source = Path(temporary).resolve() / "source"
        restored = source.with_name("restored")
        source.mkdir()
        restored.mkdir()
        copy_snapshot(snapshot, source / "Zebra", set(table_columns))

        use_zebra = "USE Zebra;"
        bootstrap(legacy_root, source, "source export", [use_zebra, *queries, *transfer_queries(table_columns)])
        bootstrap(legacy_root, restored, "schema creation", ["CREATE DATABASE Zebra;", use_zebra, *ddl])

        imports = []
        for table, columns in table_columns.items():
            name = f"transfer_{table}.tsv"
            shutil.copyfile(source / "Zebra" / name, restored / "Zebra" / name)
            destination_columns = ", ".join(f"`{column}`" for column in columns)
            imports.append(f"LOAD DATA INFILE '{name}' INTO TABLE `{table}` ({destination_columns});")
        bootstrap(legacy_root, restored, "restore", [use_zebra, *imports, *queries])

        row_count = 0
        for table in table_columns:
            name = f"verify_{table}.tsv"
            before = (source / "Zebra" / name).read_bytes().splitlines()
            after = (restored / "Zebra" / name).read_bytes().splitlines()
            if Counter(before) != Counter(after):
                raise ValueError(f"{table}: restored rows differ from the source")
            row_count += len(before)
        role_names = (source / "Zebra" / "role_names.tsv").read_bytes().splitlines()
        if role_names != (restored / "Zebra" / "role_names.tsv").read_bytes().splitlines():
            raise ValueError("Restored role name bytes differ from the source")

    print(f"Verified {len(table_columns)} tables, {row_count} rows and {len(role_names)} role names.")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--legacy-root", required=True, type=Path, help="Local MySQL 4.0 distribution")
    parser.add_argument("--snapshot", type=Path, help="Zebra data directory; defaults to LEGACY_ROOT/data/Zebra")
    parser.add_argument("--scratch", type=Path, help="Parent directory for disposable database files")
    arguments = parser.parse_args()
    legacy_root = arguments.legacy_root.resolve()
    snapshot = arguments.snapshot or legacy_root / "data" / "Zebra"
    try:
        verify(legacy_root, snapshot, arguments.scratch)
    except (OSError, ValueError, RuntimeError, subprocess.TimeoutExpired) as error:
        parser.exit(1, f"Zebra verification failed: {error}\n")


if __name__ == "__main__":
    main()
