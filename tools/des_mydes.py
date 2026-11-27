"""1:1 port of EncDec/source/mydes.cpp (OpenSSL-style DES, pluggable SPtrans).

The pak code only ever calls DES_encrypt3 / DES_decrypt3, so those are the entry
points that matter; DES_encrypt2 is ported verbatim (rotations inside, no IP/FP).

Macro binding notes (this is where earlier ports went wrong):
  IP(l,r) / FP(r,l) are textual macros, so the *caller's* expressions are renamed
  inside them. In DES_encrypt3 the IP call ends up with macro-l = data[0] and the
  FP call with macro-l = data[1].  ip()/fp() below take the macro parameters.
"""
import re, hashlib

SRC_MYDES = r"E:\源端\tmp-client2022\Client已经升级2022版本\EncDec\source\mydes.cpp"
SRC_PACK  = r"E:\源端\tmp-client2022\Client已经升级2022版本\engine\source\PackAggregate.cpp"

M = 0xFFFFFFFF


def _nums(text, marker, count=512):
    i = text.find(marker)
    assert i > 0, marker
    j = text.find("};", i)
    body = text[i:j]
    vals = [int(x, 16) for x in re.findall(r"0x([0-9A-Fa-f]{8})L", body)]
    assert len(vals) == count, (marker, len(vals))
    return vals


_md = open(SRC_MYDES, "rb").read().decode("gbk", "replace")
_pk = open(SRC_PACK, "rb").read().decode("gbk", "replace")

DES_SKB = [_nums(_md, "des_skb[8][64]", 512)[k * 64:(k + 1) * 64] for k in range(8)]
SP_DATA = [_nums(_pk, "Data_DES_SPtrans", 512)[k * 64:(k + 1) * 64] for k in range(8)]
SP_HDR  = [_nums(_pk, "HeaderData_DES_SPtrans", 512)[k * 64:(k + 1) * 64] for k in range(8)]


# ---------- primitives -------------------------------------------------------
def ror(a, n):                       # ROTATE(a,n) == _lrotr over 32 bits
    n &= 31
    a &= M
    return ((a >> n) | (a << (32 - n))) & M


def ror28(a, n):                     # C/D halves are masked to 28 bits before rotating
    a &= 0x0FFFFFFF
    return ((a >> n) | (a << (28 - n))) & 0x0FFFFFFF


def c2l(buf, off):
    return buf[off] | (buf[off + 1] << 8) | (buf[off + 2] << 16) | (buf[off + 3] << 24)


def l2c(v):
    v &= M
    return bytes((v & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF, (v >> 24) & 0xFF))


def perm_op(a, b, n, m):
    """PERM_OP(a,b,t,n,m): t=((a>>n)^b)&m ; b^=t ; a^=t<<n  -> (a,b)"""
    t = (((a >> n) ^ b) & m) & M
    b = (b ^ t) & M
    a = (a ^ ((t << n) & M)) & M
    return a, b


def hperm_op(a, n, m):
    """HPERM_OP(a,t,n,m): t=((a<<(16-n))^a)&m ; a = a^t^(t>>(16-n))"""
    s = 16 - n
    t = (((a << s) ^ a) & m) & M
    return (a ^ t ^ (t >> s)) & M


# ---------- key schedule -----------------------------------------------------
def des_set_key(key8):
    c = c2l(key8, 0)
    d = c2l(key8, 4)
    d, c = perm_op(d, c, 4, 0x0F0F0F0F)
    c = hperm_op(c, -2, 0xCCCC0000)
    d = hperm_op(d, -2, 0xCCCC0000)
    d, c = perm_op(d, c, 1, 0x55555555)
    c, d = perm_op(c, d, 8, 0x00FF00FF)
    d, c = perm_op(d, c, 1, 0x55555555)
    d = (((d & 0x000000FF) << 16) | (d & 0x0000FF00) |
         ((d & 0x00FF0000) >> 16) | ((c & 0xF0000000) >> 4)) & M
    c &= 0x0FFFFFFF
    shifts2 = [0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0]
    k = []
    for i in range(16):
        if shifts2[i]:
            c = ror28(c, 2)
            d = ror28(d, 2)
        else:
            c = ror28(c, 1)
            d = ror28(d, 1)
        c &= 0x0FFFFFFF
        d &= 0x0FFFFFFF
        s = (DES_SKB[0][(c) & 0x3F]
             | DES_SKB[1][((c >> 6) & 0x03) | ((c >> 7) & 0x3C)]
             | DES_SKB[2][((c >> 13) & 0x0F) | ((c >> 14) & 0x30)]
             | DES_SKB[3][((c >> 20) & 0x01) | ((c >> 21) & 0x06) | ((c >> 22) & 0x38)]) & M
        t = (DES_SKB[4][(d) & 0x3F]
             | DES_SKB[5][((d >> 7) & 0x03) | ((d >> 8) & 0x3C)]
             | DES_SKB[6][(d >> 15) & 0x3F]
             | DES_SKB[7][((d >> 21) & 0x0F) | ((d >> 22) & 0x30)]) & M
        t2 = (((t << 16) & M) | (s & 0x0000FFFF)) & M
        k.append(ror(t2, 30))
        t2 = ((s >> 16) | (t & 0xFFFF0000)) & M
        k.append(ror(t2, 26))
    return k


# ---------- permutations (arguments == macro parameters) ---------------------
def ip(l, r):
    """IP(l,r) with l,r being the macro's own parameters."""
    r, l = perm_op(r, l, 4, 0x0F0F0F0F)
    l, r = perm_op(l, r, 16, 0x0000FFFF)
    r, l = perm_op(r, l, 2, 0x33333333)
    l, r = perm_op(l, r, 8, 0x00FF00FF)
    r, l = perm_op(r, l, 1, 0x55555555)
    return l, r


def fp(l, r):
    """FP(l,r) with l,r being the macro's own parameters."""
    l, r = perm_op(l, r, 1, 0x55555555)
    r, l = perm_op(r, l, 8, 0x00FF00FF)
    l, r = perm_op(l, r, 2, 0x33333333)
    r, l = perm_op(r, l, 16, 0x0000FFFF)
    l, r = perm_op(l, r, 4, 0x0F0F0F0F)
    return l, r


def d_encrypt(LL, R, s, S, sp):
    """D_ENCRYPT(LL,R,S)"""
    u = (R ^ s[S]) & M
    t = ror((R ^ s[S + 1]) & M, 4)
    LL ^= (sp[0][(u >> 2) & 0x3F] ^ sp[2][(u >> 10) & 0x3F] ^
           sp[4][(u >> 18) & 0x3F] ^ sp[6][(u >> 26) & 0x3F] ^
           sp[1][(t >> 2) & 0x3F] ^ sp[3][(t >> 10) & 0x3F] ^
           sp[5][(t >> 18) & 0x3F] ^ sp[7][(t >> 26) & 0x3F])
    return LL & M


# ---------- DES_encrypt2 -----------------------------------------------------
def encrypt2(data2, ks, sp, enc):
    r = data2[0]
    l = data2[1]
    r = ror(r, 29)
    l = ror(l, 29)
    if enc:
        for i in range(0, 32, 8):
            l = d_encrypt(l, r, ks, i + 0, sp)
            r = d_encrypt(r, l, ks, i + 2, sp)
            l = d_encrypt(l, r, ks, i + 4, sp)
            r = d_encrypt(r, l, ks, i + 6, sp)
    else:
        for i in range(30, 0, -8):
            l = d_encrypt(l, r, ks, i - 0, sp)
            r = d_encrypt(r, l, ks, i - 2, sp)
            l = d_encrypt(l, r, ks, i - 4, sp)
            r = d_encrypt(r, l, ks, i - 6, sp)
    return [ror(l, 3), ror(r, 3)]


# ---------- DES_encrypt3 / DES_decrypt3 --------------------------------------
def encrypt3(block8, ks1, ks2, ks3, sp):
    l = c2l(block8, 0)
    r = c2l(block8, 4)
    l, r = ip(l, r)                     # IP(l,r)
    d = encrypt2([l, r], ks1, sp, True)
    d = encrypt2(d, ks2, sp, False)
    d = encrypt2(d, ks3, sp, True)
    l, r = d[0], d[1]
    ml, mr = fp(r, l)                   # FP(r,l) -> macro-l=r, macro-r=l
    r, l = ml, mr
    return l2c(l) + l2c(r)


def decrypt3(block8, ks1, ks2, ks3, sp):
    l = c2l(block8, 0)
    r = c2l(block8, 4)
    l, r = ip(l, r)
    d = encrypt2([l, r], ks3, sp, False)
    d = encrypt2(d, ks2, sp, True)
    d = encrypt2(d, ks1, sp, False)
    l, r = d[0], d[1]
    ml, mr = fp(r, l)
    r, l = ml, mr
    return l2c(l) + l2c(r)


def password_to_keys(pw: bytes):
    """PasswordToDesKey: md5 split into three 8-byte keys."""
    md5 = hashlib.md5(pw).digest()
    return md5[0:8], md5[8:16], md5[4:12]


def make_key3(pw: bytes):
    k1, k2, k3 = password_to_keys(pw)
    return des_set_key(k1), des_set_key(k2), des_set_key(k3)


if __name__ == "__main__":
    pw = b"xuzhao20041108"
    k1, k2, k3 = password_to_keys(pw)
    print("keys:", k1.hex(), k2.hex(), k3.hex())
    ks = make_key3(pw)
    blk = bytes(range(8))
    for name, sp in (("SP_HDR", SP_HDR), ("SP_DATA", SP_DATA)):
        c = encrypt3(blk, ks[0], ks[1], ks[2], sp)
        d = decrypt3(c, ks[0], ks[1], ks[2], sp)
        print(f"roundtrip {name}: {d == blk}  ct={c.hex()}")
