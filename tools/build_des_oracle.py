"""Build a ground-truth DES oracle with the shipped VC98 toolchain.

Copies the real mydes.cpp + mydes.h into an ASCII path, extracts the two
SPtrans tables verbatim from PackAggregate.cpp, and compiles a small test
driver that prints ciphertext for known inputs. The Python port is then
diffed against this output byte for byte.
"""
import os, re, shutil, subprocess, sys

ROOT = r"E:\源端\zhengtugf\tools\_des_test"
CLIENT = r"E:\源端\tmp-client2022\Client已经升级2022版本"
VCROOT = r"C:\Program Files (x86)\Microsoft Visual Studio"
VC = os.path.join(VCROOT, "VC98")


def extract_table(text, name):
    i = text.index(name)
    i = text.index("{", i)
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                return text[i:j + 1]
    raise RuntimeError(name)


def main():
    os.makedirs(os.path.join(ROOT, "source"), exist_ok=True)
    os.makedirs(os.path.join(ROOT, "include"), exist_ok=True)
    shutil.copyfile(os.path.join(CLIENT, r"EncDec\source\mydes.cpp"),
                    os.path.join(ROOT, r"source\mydes.cpp"))
    shutil.copyfile(os.path.join(CLIENT, r"EncDec\include\mydes.h"),
                    os.path.join(ROOT, r"include\mydes.h"))

    pack = open(os.path.join(CLIENT, r"engine\source\PackAggregate.cpp"), "rb") \
        .read().decode("gbk", "replace")
    t_data = extract_table(pack, "Data_DES_SPtrans")
    t_hdr = extract_table(pack, "HeaderData_DES_SPtrans")

    driver = r'''
#include <stdio.h>
#include <string.h>
#include "mydes.h"
typedef unsigned long DWORD;

void DES_set_key(const_DES_cblock *key, DES_key_schedule *schedule);
void DES_encrypt1(DES_LONG *data, DES_key_schedule *ks1, t_DES_SPtrans * sp, int enc);
void DES_encrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2,
                  DES_key_schedule *ks3, t_DES_SPtrans * sp);
void DES_decrypt3(DES_LONG *data, DES_key_schedule *ks1, DES_key_schedule *ks2,
                  DES_key_schedule *ks3, t_DES_SPtrans * sp);

static t_DES_SPtrans Data_DES_SPtrans_g = __DATA_TABLE__;
static t_DES_SPtrans HeaderData_DES_SPtrans_g = __HDR_TABLE__;

static void hexdump(const char *tag, unsigned char *p, int n)
{
    int i;
    printf("%s ", tag);
    for (i = 0; i < n; i++) printf("%02x", p[i]);
    printf("\n");
}

static void run_set(const char *tag, t_DES_SPtrans *sp, unsigned char *k1,
                    unsigned char *k2, unsigned char *k3)
{
    DES_key_schedule s1, s2, s3;
    unsigned char blk[8];
    unsigned char tmp[8];
    int i;

    DES_set_key((const_DES_cblock *)k1, &s1);
    DES_set_key((const_DES_cblock *)k2, &s2);
    DES_set_key((const_DES_cblock *)k3, &s3);

    for (i = 0; i < 8; i++) blk[i] = (unsigned char)(i * 31 + 7);
    printf("[%s] plain ", tag); hexdump("", blk, 8);

    memcpy(tmp, blk, 8);
    DES_encrypt3((DWORD *)tmp, &s1, &s2, &s3, sp);
    printf("[%s] enc3  ", tag); hexdump("", tmp, 8);

    DES_decrypt3((DWORD *)tmp, &s1, &s2, &s3, sp);
    printf("[%s] dec3  ", tag); hexdump("", tmp, 8);

    /* single DES, classic vector, little-endian byte order */
    {
        unsigned char sk[8] = {0xF1,0xDF,0xBC,0x9B,0x79,0x57,0x34,0x13};
        unsigned char pt[8] = {0xEF,0xCD,0xAB,0x89,0x67,0x45,0x23,0x01};
        DES_key_schedule s;
        DES_set_key((const_DES_cblock *)sk, &s);
        DES_encrypt1((DWORD *)pt, &s, sp, DES_ENCRYPT);
        printf("[%s] des1  ", tag); hexdump("", pt, 8);
    }
}

int main(void)
{
    unsigned char k1[8] = {'f','2','a','2','3','7','f','d'};
    unsigned char k2[8] = {'1','d','b','c','5','5','6','3'};
    unsigned char k3[8] = {'6','d','3','f','d','d','e','e'};
    /* same three keys built from the real md5 split */
    unsigned char d1[8] = {0xf2,0xa2,0x37,0xfd,0x6d,0x3f,0xdd,0xee};
    unsigned char d2[8] = {0x1d,0xbc,0x55,0x63,0x35,0x32,0x55,0x11};
    unsigned char d3[8] = {0x6d,0x3f,0xdd,0xee,0x1d,0xbc,0x55,0x63};

    run_set("data-hex", &Data_DES_SPtrans_g, d1, d2, d3);
    run_set("hdr-hex", &HeaderData_DES_SPtrans_g, d1, d2, d3);
    return 0;
}
'''
    driver = driver.replace("__DATA_TABLE__", t_data).replace("__HDR_TABLE__", t_hdr)
    open(os.path.join(ROOT, r"source\test_des.cpp"), "w", encoding="ascii",
         errors="replace").write(driver)

    env = dict(os.environ)
    env["PATH"] = os.path.join(VC, "Bin") + ";" + os.path.join(VCROOT, "Common", "MSDev98", "Bin") + ";" + env["PATH"]
    env["INCLUDE"] = os.path.join(VC, "Include") + ";" + os.path.join(VC, "MFC", "Include")
    env["LIB"] = os.path.join(VC, "Lib") + ";" + os.path.join(VC, "MFC", "Lib")

    cl = os.path.join(VC, "Bin", "CL.EXE")
    cmd = [cl, "/nologo", "/EHsc", "/D_X86_=1", "/DWIN32", "/I..\\include",
           "mydes.cpp", "test_des.cpp", "/Fe:des_test.exe", "/link", "/SUBSYSTEM:CONSOLE"]
    p = subprocess.run(cmd, cwd=os.path.join(ROOT, "source"), env=env,
                       capture_output=True, text=True, errors="replace")
    print("---- cl stdout ----")
    print(p.stdout)
    if p.returncode != 0:
        print("---- cl stderr ----")
        print(p.stderr)
        print("COMPILE FAILED rc=", p.returncode)
        return 1
    exe = os.path.join(ROOT, "source", "des_test.exe")
    r = subprocess.run([exe], capture_output=True, text=True, errors="replace")
    print("---- des_test ----")
    print(r.stdout)
    return 0


if __name__ == "__main__":
    sys.exit(main())
