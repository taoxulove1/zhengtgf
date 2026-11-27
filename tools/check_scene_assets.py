"""Check ScenesServer startup table and map assets against the repository source."""

import argparse
from pathlib import Path, PureWindowsPath
import re
import struct
import sys
import xml.etree.ElementTree as ET


SOURCE = Path(__file__).resolve().parents[1] / "src" / "server"
SCALAR_WIDTH = {"char": 1, "BYTE": 1, "WORD": 2, "DWORD": 4, "QWORD": 8, "uint64_t": 8}
MAP_HEADER = struct.Struct("<4sIII")
TABLE_COUNT = struct.Struct("<I")


def cpp_source(path):
    source = path.read_bytes().decode("gb18030")
    return re.sub(r"/\*.*?\*/|//[^\n]*", "", source, flags=re.DOTALL)


def table_widths():
    base = SOURCE / "base"
    manager = cpp_source(base / "zDatabaseManager.cpp")
    startup = re.search(r"^bool loadAllBM\(\)\s*\{(.*?)^\}", manager, re.MULTILINE | re.DOTALL)
    if not startup:
        raise ValueError("loadAllBM startup loader not found")
    loaders = re.findall(
        r'^\s*if\s*\(\s*!(\w+)\.refresh\(\(Zebra::global\["tbldir"\]\s*\+\s*"/([^/"]+\.tbl)"',
        startup[1],
        re.MULTILINE,
    )
    if not loaders or len(loaders) != len(re.findall(r"\brefresh\s*\(", startup[1])):
        raise ValueError("Unrecognized startup table loader")

    record_names = {
        variable: record
        for record, variable in re.findall(r"\bzDataBM\s*<\s*\w+\s*,\s*(\w+)\s*>\s*&\s*(\w+)", manager)
    }
    types = "\n".join(cpp_source(base / name) for name in ("zDatabase.h", "SkillBase.h", "Object.h"))
    widths = {}
    for variable, filename in loaders:
        record = record_names[variable]
        alias = re.search(rf"\btypedef\s+(\w+)\s+{record}\s*;", types)
        if alias:
            record = alias[1]
        definition = re.search(rf"^struct\s+{record}\s*\{{(.*?)^\}};", types, re.MULTILINE | re.DOTALL)
        if not definition:
            raise ValueError(f"{filename}: packed record {record} not found")
        body = re.sub(r"[^;{}]+\{[^{}]*\}", "", definition[1])
        width = 0
        for declaration in filter(None, (field.strip() for field in body.split(";"))):
            field = re.fullmatch(r"(\w+)\s+\w+\s*(?:\[(\d+)\])?", declaration)
            if not field or field[1] not in SCALAR_WIDTH:
                raise ValueError(f"{filename}: unsupported field in {record}")
            width += SCALAR_WIDTH[field[1]] * int(field[2] or 1)
        if not width or widths.setdefault(filename, width) != width:
            raise ValueError(f"{filename}: conflicting packed record widths")
    return widths


def directory_files(directory):
    # A case-insensitive Windows lookup would hide a filename mismatch on Linux.
    return {entry.name: entry for entry in directory.iterdir() if entry.is_file()} if directory.is_dir() else {}


def xml_root(path):
    source = path.read_bytes()
    encoding = re.search(rb'^\s*<\?xml\b[^?]*\bencoding\s*=\s*["\x27]([^"\x27]+)', source)
    return ET.fromstring(source.decode(encoding[1].decode("ascii"))) if encoding else ET.fromstring(source)


def scene_map_names(scenes):
    root = xml_root(scenes)
    if root.tag != "ScenesInfo":
        raise ValueError(f"{scenes}: expected <ScenesInfo>")
    assignments = root.findall("./server/country/map")
    if not assignments:
        raise ValueError(f"{scenes}: no static map assignments")
    definitions = {}
    for node in root.findall("./mapinfo/map"):
        # SceneManager uses map insertion. The first definition wins for duplicate IDs.
        definitions.setdefault(node.get("mapID"), node.get("fileName"))

    names = set()
    errors = []
    for map_id in sorted({entry.get("mapID", "") for entry in assignments}):
        name = definitions.get(map_id)
        if not name:
            errors.append(f"map ID {map_id or '<empty>'}: no fileName in mapinfo")
            continue
        path = PureWindowsPath(name)
        if path.name != name or name in (".", "..") or not path.name:
            errors.append(f"map ID {map_id}: invalid fileName {name}")
            continue
        names.add(name)
    return names, errors


def table_problems(widths, files):
    for name, width in sorted(widths.items()):
        if name not in files:
            yield f"{name}: missing exact filename"
            continue
        with files[name].open("rb") as stream:
            data = stream.read(TABLE_COUNT.size)
        if len(data) != TABLE_COUNT.size:
            yield f"{name}: incomplete record count"
            continue
        count, = TABLE_COUNT.unpack(data)
        length = files[name].stat().st_size
        expected = TABLE_COUNT.size + count * width
        if length != expected:
            yield f"{name}: {length} bytes, expected {expected} for {count} records of {width} bytes"


def map_problems(names, files):
    header = cpp_source(SOURCE / "ScenesServer" / "LoadMap.h")
    version = int(re.search(r"(?m)^#define MAP_VERSION\s+(\d+)", header)[1])
    for name in sorted(names):
        terrain, definition = files.get(name + ".mps"), files.get(name + ".xml")
        if terrain is None or definition is None:
            missing = ".mps and .xml" if terrain is None and definition is None else (
                ".mps" if terrain is None else ".xml"
            )
            yield f"{name}: missing {missing}"
            continue
        with terrain.open("rb") as stream:
            data = stream.read(MAP_HEADER.size)
        if len(data) != MAP_HEADER.size:
            yield f"{name}: incomplete MPS header"
            continue
        magic, actual_version, width, height = MAP_HEADER.unpack(data)
        if magic != b"XMPS" or not width or not height or terrain.stat().st_size != MAP_HEADER.size + 2 * width * height:
            yield f"{name}: MPS header or tile length mismatch"
            continue
        if actual_version != version:
            yield f"{name}: MPS version {actual_version}, expected {version}"
            continue
        try:
            if xml_root(definition).tag != "map":
                yield f"{name}: expected <map> in map XML"
        except (ET.ParseError, UnicodeError, LookupError) as error:
            yield f"{name}: invalid map XML ({error})"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("server_dir", type=Path, help="Server directory containing scenesinfo.xml")
    parser.add_argument("--tables", type=Path, help="TBL directory, defaults to SERVER_DIR/tbl")
    parser.add_argument("--maps", type=Path, help="Map directory, defaults to SERVER_DIR/map")
    parser.add_argument("--summary", action="store_true", help="Show counts without individual problems")
    arguments = parser.parse_args()
    directory = arguments.server_dir
    try:
        widths = table_widths()
        names, unresolved = scene_map_names(directory / "scenesinfo.xml")
        tables = list(table_problems(widths, directory_files(arguments.tables or directory / "tbl")))
        maps = list(map_problems(names, directory_files(arguments.maps or directory / "map")))
    except (OSError, ValueError, KeyError, ET.ParseError, LookupError) as error:
        parser.exit(2, f"Asset check failed: {error}\n")

    print(f"TBL layout: {len(widths) - len(tables)}/{len(widths)} match.")
    print(f"MPS/XML pairs: {len(names) - len(maps)}/{len(names)} match; {len(unresolved)} unresolved map IDs.")
    if not arguments.summary:
        print("\n".join([*tables, *unresolved, *maps]), end="\n" if tables or unresolved or maps else "")
    return 1 if tables or unresolved or maps else 0


if __name__ == "__main__":
    sys.exit(main())
