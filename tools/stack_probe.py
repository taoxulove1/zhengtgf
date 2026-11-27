"""Grab the stack of the thread that owns the error dialog, then disassemble the
call sites on it. The dialog is modal, so the caller chain is still on the stack.
"""
import sys, os, time, struct, subprocess, ctypes, ctypes.wintypes as wt
import capstone

ROOT = r"E:\源端\源1"
EXE = os.path.join(ROOT, "Data", "zhengtu.dat")
ARGS = [EXE, "-wd:http://101.33.242.142/autopatch", "101.33.242.142", "8000", "710", "0"]

k32 = ctypes.WinDLL("kernel32", use_last_error=True)
u32 = ctypes.WinDLL("user32", use_last_error=True)
WNDENUMPROC = ctypes.WINFUNCTYPE(wt.BOOL, wt.HWND, wt.LPARAM)


class MBI(ctypes.Structure):
    _fields_ = [("BaseAddress", ctypes.c_void_p), ("AllocationBase", ctypes.c_void_p),
                ("AllocationProtect", wt.DWORD), ("RegionSize", ctypes.c_size_t),
                ("State", wt.DWORD), ("Protect", wt.DWORD), ("Type", wt.DWORD)]


class CONTEXT(ctypes.Structure):
    _fields_ = [("ContextFlags", wt.DWORD), ("Dr0", wt.DWORD), ("Dr1", wt.DWORD),
                ("Dr2", wt.DWORD), ("Dr3", wt.DWORD), ("Dr6", wt.DWORD), ("Dr7", wt.DWORD),
                ("FloatingSave", ctypes.c_byte * 112),
                ("SegGs", wt.DWORD), ("SegFs", wt.DWORD), ("SegEs", wt.DWORD),
                ("SegDs", wt.DWORD),
                ("Edi", wt.DWORD), ("Esi", wt.DWORD), ("Ebx", wt.DWORD), ("Edx", wt.DWORD),
                ("Ecx", wt.DWORD), ("Eax", wt.DWORD), ("Ebp", wt.DWORD), ("Eip", wt.DWORD),
                ("SegCs", wt.DWORD), ("EFlags", wt.DWORD), ("Esp", wt.DWORD), ("SegSs", wt.DWORD),
                ("ExtendedRegisters", ctypes.c_byte * 512)]


def win_text(h):
    n = u32.GetWindowTextLengthW(h)
    b = ctypes.create_unicode_buffer(n + 1)
    u32.GetWindowTextW(h, b, n + 1)
    return b.value


def win_class(h):
    b = ctypes.create_unicode_buffer(256)
    u32.GetClassNameW(h, b, 256)
    return b.value


def find_dialog(pid):
    out = []

    def cb(h, lp):
        p = wt.DWORD()
        u32.GetWindowThreadProcessId(h, ctypes.byref(p))
        if p.value == pid and win_class(h) == "#32770":
            out.append((h, p.value))
        return True

    u32.EnumWindows(WNDENUMPROC(cb), 0)
    return out


def read(h, addr, n):
    buf = ctypes.create_string_buffer(n)
    got = ctypes.c_size_t()
    if k32.ReadProcessMemory(h, ctypes.c_void_p(addr), buf, ctypes.c_size_t(n), ctypes.byref(got)):
        return buf.raw[:got.value]
    return b""


def region_of(h, addr):
    mbi = MBI()
    if k32.VirtualQueryEx(h, ctypes.c_void_p(addr), ctypes.byref(mbi), ctypes.sizeof(mbi)):
        return mbi.BaseAddress, mbi.RegionSize, mbi.Protect
    return None, 0, 0


def main():
    p = subprocess.Popen(ARGS, cwd=ROOT)
    print("pid", p.pid)
    dlgs = []
    for _ in range(20):
        time.sleep(1)
        dlgs = find_dialog(p.pid)
        if dlgs:
            break
    if not dlgs:
        print("no dialog"); p.kill(); return
    hwnd, tid = dlgs[0]
    print("dialog hwnd=%x tid=%d title=%r" % (hwnd, tid, win_text(hwnd)))

    h = k32.OpenProcess(0x1F0FFF, False, p.pid)
    print("OpenProcess err", ctypes.get_last_error())
    th = None
    for mask in (0x1FFFFF, 0x1F03FF, 0x004A, 0x0002 | 0x0008 | 0x0010 | 0x0020 | 0x0040):
        th = k32.OpenThread(mask, False, tid)
        if th:/n            print("OpenThread ok mask=%x" % mask)
            break
        print("OpenThread fail mask=%x err=%d" % (mask, ctypes.get_last_error()))
    print("proc h=%s thread h=%s" % (h, th))
    k32.SuspendThread(th)
    ctx = CONTEXT()
    ctx.ContextFlags = 0x00010007  # CONTROL|INTEGER|SEGMENTS
    ok = k32.GetThreadContext(th, ctypes.byref(ctx))
    print("GetThreadContext", ok, "eip=%08x esp=%08x ebp=%08x" % (ctx.Eip, ctx.Esp, ctx.Ebp))
    stack = read(h, ctx.Esp, 4096)
    addrs = []
    for i in range(0, len(stack) - 3, 4):
        v = struct.unpack_from("<I", stack, i)[0]
        if 0x400000 <= v < 0x00F00000:
            addrs.append((ctx.Esp + i, v))
    print("stack candidates:", len(addrs))
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for sp, v in addrs[:40]:
        base, size, prot = region_of(h, v - 64)
        code = read(h, v - 48, 64)
        if len(code) < 64:
            continue
        text = []
        for insn in md.disasm(code, v - 48):
            mark = ">>" if insn.address == v else "  "
            text.append("%s%08x %-24s %s %s" % (mark, insn.address, insn.bytes.hex(),
                                                insn.mnemonic, insn.op_str))
        print("---- stack+%04x -> %08x (prot=%x)" % (sp - ctx.Esp, v, prot))
        for t in text[-14:]:
            print("     ", t)

    k32.ResumeThread(th)
    k32.CloseHandle(th)
    k32.CloseHandle(h)
    p.terminate()
    time.sleep(0.5)
    if p.poll() is None:
        p.kill()


if __name__ == "__main__":
    main()
