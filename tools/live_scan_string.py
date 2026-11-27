"""Start the client, then scan its live memory for a GBK string and the code that
references it. HProtect packs the on-disk image, so runtime memory is the only
place the message and its surrounding logic are visible.
"""
import sys, os, time, struct, subprocess, ctypes, ctypes.wintypes as wt

ROOT = r"E:\源端\源1"
EXE = os.path.join(ROOT, "Data", "zhengtu.dat")
ARGS = [EXE, "-wd:http://101.33.242.142/autopatch", "101.33.242.142", "8000", "710", "0"]

k32 = ctypes.WinDLL("kernel32", use_last_error=True)

PROCESS_QUERY_INFORMATION = 0x0400
PROCESS_VM_READ = 0x0010

MEM_COMMIT = 0x1000
PAGE_READABLE = (0x02, 0x04, 0x20, 0x40)  # READONLY, READWRITE, EXECUTE_READ, EXECUTE_READWRITE


class MEMORY_BASIC_INFORMATION(ctypes.Structure):
    _fields_ = [("BaseAddress", ctypes.c_void_p),
                ("AllocationBase", ctypes.c_void_p),
                ("AllocationProtect", wt.DWORD),
                ("RegionSize", ctypes.c_size_t),
                ("State", wt.DWORD),
                ("Protect", wt.DWORD),
                ("Type", wt.DWORD)]


def regions(handle):
    addr = 0
    mbi = MEMORY_BASIC_INFORMATION()
    while addr < 0x7FFFFFFF:
        if not k32.VirtualQueryEx(handle, ctypes.c_void_p(addr), ctypes.byref(mbi),
                                  ctypes.sizeof(mbi)):
            break
        base = mbi.BaseAddress or 0
        n = mbi.RegionSize
        if mbi.State == MEM_COMMIT and mbi.Protect in PAGE_READABLE and n < (256 << 20):
            yield base, n, mbi.Protect
        addr = base + n if n else addr + 0x1000


def read_mem(handle, base, size):
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t(0)
    if k32.ReadProcessMemory(handle, ctypes.c_void_p(base), buf, ctypes.c_size_t(size),
                             ctypes.byref(got)):
        return buf.raw[:got.value]
    return None


def main():
    needle = sys.argv[1].encode("gbk") if len(sys.argv) > 1 else \
        "不是最新版本或已损坏".encode("gbk")
    p = subprocess.Popen(ARGS, cwd=ROOT)
    print("pid", p.pid, "waiting 11s")
    time.sleep(11)
    h = k32.OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, False, p.pid)
    print("handle", h)
    if not h:
        print("OpenProcess failed", ctypes.get_last_error())
        p.kill()
        return
    snap = []
    total = 0
    for base, size, prot in regions(h):
        if total > 900 << 20:
            break
        d = read_mem(h, base, size)
        if not d:
            continue
        total += len(d)
        snap.append((base, d))
    print("regions:", len(snap), "bytes:", total)

    hits = []
    for base, d in snap:
        start = 0
        while True:
            i = d.find(needle, start)
            if i < 0:
                break
            hits.append((base + i, base, d, i))
            start = i + 1
    print("string hits:", len(hits))
    for va, base, d, i in hits[:4]:
        ctx = d[max(0, i - 60):i + len(needle) + 60]
        print("  va=%08x ctx=%r" % (va, ctx))

    for va, base, d, i in hits[:4]:
        pat = struct.pack("<I", va)
        refs = []
        for rb, rd in snap:
            s = 0
            while True:
                j = rd.find(pat, s)
                if j < 0:
                    break
                refs.append((rb + j, rb, rd, j))
                s = j + 1
        print("  refs to %08x: %d" % (va, len(refs)))
        for rva, rb, rd, j in refs[:6]:
            print("    ref at %08x  bytes=%s" % (rva, rd[max(0, j - 24):j + 12].hex()))

    k32.CloseHandle(h)
    p.terminate()
    time.sleep(0.6)
    if p.poll() is None:
        p.kill()


if __name__ == "__main__":
    main()
