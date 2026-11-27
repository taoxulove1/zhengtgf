"""Try candidate datas.pak variants against the real client and report the dialog text.

Each variant is built into a temp pak, copied over Data\\datas.pak, the client is
started for N seconds, its windows/child controls are read out, then the pristine
pak is restored no matter what.
"""
import sys, os, io, time, shutil, subprocess, ctypes, ctypes.wintypes as wt
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import pak_open, pak_repack

ROOT = r"E:\源端\源1"
DATA = os.path.join(ROOT, "Data")
PAK = os.path.join(DATA, "datas.pak")
BACKUP = os.path.join(DATA, "datas.pak.pristine")
EXE = os.path.join(DATA, "zhengtu.dat")
ARGS = [EXE, "-wd:http://101.33.242.142/autopatch", "101.33.242.142", "8000", "710", "0"]
TMP = r"E:\源端\tmp-paktest"

user32 = ctypes.WinDLL("user32", use_last_error=True)
WNDENUMPROC = ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)


def get_text(hwnd):
    n = user32.GetWindowTextLengthW(hwnd)
    b = ctypes.create_unicode_buffer(n + 1)
    user32.GetWindowTextW(hwnd, b, n + 1)
    return b.value


def get_class(hwnd):
    b = ctypes.create_unicode_buffer(256)
    user32.GetClassNameW(hwnd, b, 256)
    return b.value


def windows_of(pid):
    out = []

    def cb(hwnd, lp):
        p = wt.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(p))
        if p.value == pid:
            out.append(hwnd)
        return True

    user32.EnumWindows(WNDENUMPROC(cb), 0)
    return out


def children_of(hwnd):
    out = []

    def cb(h, lp):
        out.append(h)
        return True

    user32.EnumChildWindows(hwnd, WNDENUMPROC(cb), 0)
    return out


def run_client(seconds=14):
    p = subprocess.Popen(ARGS, cwd=ROOT,
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    time.sleep(seconds)
    alive = p.poll() is None
    lines = []
    for w in windows_of(p.pid):
        cls, title = get_class(w), get_text(w)
        lines.append("WIN[%s] %r" % (cls, title))
        for k in children_of(w):
            t = get_text(k)
            if t.strip():
                lines.append("   CHILD[%s] %r" % (get_class(k), t))
    if alive:
        p.terminate()
        time.sleep(0.8)
        if p.poll() is None:
            p.kill()
    else:
        lines.append("EXITED rc=%s" % p.poll())
    return alive, lines


def variants():
    yield "stub-1record", {"datas/tables/characterstatebase.tbl":
                           (1).to_bytes(4, "little") + b"\x01" + b"\0" * 571}
    yield "empty-0record", {"datas/tables/characterstatebase.tbl": (0).to_bytes(4, "little")}


def main():
    if not os.path.exists(BACKUP):
        shutil.copyfile(PAK, BACKUP)
        print("backup ->", BACKUP, os.path.getsize(BACKUP))
    try:
        print("== baseline (pristine pak) ==")
        shutil.copyfile(BACKUP, PAK)
        alive, lines = run_client()
        print("  alive=%s" % alive)
        for l in lines:
            print("   ", l)

        for name, rep in variants():
            dst = os.path.join(TMP, "v-%s.pak" % name)
            print("== variant %s ==" % name)
            pak_repack.repack(BACKUP, dst, rep, verbose=False)
            print("  built", dst, os.path.getsize(dst))
            shutil.copyfile(dst, PAK)
            alive, lines = run_client()
            print("  alive=%s" % alive)
            for l in lines:
                print("   ", l)
    finally:
        shutil.copyfile(BACKUP, PAK)
        print("restored pristine pak:", os.path.getsize(PAK))


if __name__ == "__main__":
    main()
