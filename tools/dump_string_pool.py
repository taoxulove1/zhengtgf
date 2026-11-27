"""Dump the client's string pool around the 'Data\\Datas.pak ...' message and list
every datas\\* path it mentions, so we can diff it against the pack contents.
"""
import sys, os, time, re, subprocess, ctypes, ctypes.wintypes as wt

ROOT = r"E:\源端\源1"
EXE = os.path.join(ROOT, "Data", "zhengtu.dat")
ARGS = [EXE, "-wd:http://101.33.242.142/autopatch", "101.33.242.142", "8000", "710", "0"]
k32 = ctypes.WinDLL("kernel32", use_last_error=True)


class MBI(ctypes.Structure):
    _fields_ = [("BaseAddress", ctypes.c_void_p), ("AllocationBase", ctypes.c_void_p),
                ("AllocationProtect", wt.DWORD), ("RegionSize", ctypes.c_size_t),
                ("State", wt.DWORD), ("Protect", wt.DWORD), ("Type", wt.DWORD)]


def main():
    p = subprocess.Popen(ARGS, cwd=ROOT)
    time.sleep(11)
    h = k32.OpenProcess(0x0410, False, p.pid)
    needle = "不是最新版本或已损坏".encode("gbk")
    addr = 0
    mbi = MBI()
    found = None
    while addr < 0x7FFFFFFF:
        if not k32.VirtualQueryEx(h, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi)):
            break
        base, size, state, prot = mbi.BaseAddress or 0, mbi.RegionSize, mbi.State, mbi.Protect
        if state == 0x1000 and prot in (2, 4, 0x20, 0x40) and size < (256 << 20):
            buf = ctypes.create_string_buffer(size)
            got = ctypes.c_size_t()
            if k32.ReadProcessMemory(h, ctypes.c_void_p(base), buf, ctypes.c_size_t(size),
                                     ctypes.byref(got)):
                d = buf.raw[:got.value]
                i = d.find(needle)
                if i >= 0:
                    found = (base, d, i)
                    break
        addr = base + size if size else addr + 0x1000
    if not found:
        print("not found")
        p.kill()
        return
    base, d, i = found
    va = base + i
    print("pool base=%08x size=%d  string va=%08x" % (base, len(d), va))
    lo = 0
    hi = len(d)
    chunk = d[lo:hi]
    # extract ascii strings
    for m in re.finditer(rb"[\x20-\x7e]{4,}", chunk):
        s = m.group().decode("latin1")
        if re.search(r"datas|data\\|\\.pak|\.xml|\.tbl|\.ini", s, re.I):
            print("   %08x  %s" % (base + lo + m.start(), s))
    k32.CloseHandle(h)
    p.terminate()
    time.sleep(0.5)
    if p.poll() is None:
        p.kill()


if __name__ == "__main__":
    main()
