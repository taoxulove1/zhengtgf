"""Empirical test: is the pak crypto plain standard 3DES-EDE?

Takes the tail headerSize bytes of datas.pak, decrypts them with the MD5-split
keys using the plain standard DES implementation (des_impl, validated against the
classic test vector), and reports how much of the result looks like ASCII paths.
"""
import sys, struct, hashlib
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import des_impl as DES

PAK = r"E:\源端\源1\Data\datas.pak"
PASSWORD = b"xuzhao20041108"


def md5_keys(pw):
    m = hashlib.md5(pw).digest()
    return m[0:8], m[8:16], m[4:12]


def des3_dec(block, k1, k2, k3):
    x = DES.crypt_block(block, k3, decrypt=True)
    x = DES.crypt_block(x, k2, decrypt=False)
    x = DES.crypt_block(x, k1, decrypt=True)
    return x


def printable_ratio(buf):
    if not buf:
        return 0.0
    ok = sum(1 for b in buf if 32 <= b < 127 or b in (0, 9, 10, 13))
    return ok / len(buf)


def main():
    data = open(PAK, "rb").read()
    magic, ver, filenum, header_size, flag = struct.unpack_from("<4siiii", data, 0)
    print("magic=%r ver=%d files=%d headerSize=%d flag=0x%x size=%d"
          % (magic, ver, filenum, header_size, flag, len(data)))
    tail = data[len(data) - header_size:]
    k1, k2, k3 = md5_keys(PASSWORD)
    print("keys", k1.hex(), k2.hex(), k3.hex())

    for label, fn in (
        ("D(k3)E(k2)D(k1)", des3_dec),
        ("D(k1)E(k2)D(k3)", lambda b, a, c, d: des3_dec(b, d, c, a)),
    ):
        out = bytearray()
        for i in range(0, len(tail) - 7, 8):
            out += fn(tail[i:i + 8], k1, k2, k3)
        r = printable_ratio(bytes(out))
        print(f"{label}: printable={r:.3f} head={bytes(out[:48])!r}")


if __name__ == "__main__":
    main()
