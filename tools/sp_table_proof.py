"""Prove whether Data_DES_SPtrans / HeaderData_DES_SPtrans are the standard OpenSSL
des_SPtrans (i.e. plain S-box+P tables). No compiler needed: for every group and
every 6-bit index we rebuild the expected P-permuted nibble from the standard S-box
and compare with the table value.
"""
import sys
sys.path.insert(0, r"E:\源端\zhengtugf\tools")
import des_impl as S
import des_mydes as D

P = S.P
SBOX = S.SBOX

# output bit positions (0 = LSB) that P assigns to each S-box group
POS = []
for g in range(8):
    pos = []
    for k in range(4):
        # P[4*g + k] is a 1-based MSB index -> 0-based LSB index
        pos.append(32 - P[4 * g + k])
    POS.append(pos)


def expected(tbl):
    bad = []
    for g in range(8):
        for idx in range(64):
            row = ((idx >> 4) & 2) | (idx & 1)
            col = (idx >> 1) & 0xF
            nib = SBOX[g][row * 16 + col]
            v = 0
            for k in range(4):
                if (nib >> (3 - k)) & 1:
                    v |= 1 << POS[g][k]
            if v != tbl[g][idx]:
                bad.append((g, idx, v, tbl[g][idx]))
    return bad


for name in ("SP_DATA", "SP_HDR"):
    tbl = getattr(D, name)
    bad = expected(tbl)
    print(f"{name}: standard-openSSL={not bad}  mismatches={len(bad)}")
    for row in bad[:6]:
        print("   group=%d idx=0x%02x expect 0x%08x table 0x%08x" % row)

print("SP_DATA == SP_HDR :", D.SP_DATA == D.SP_HDR)
