#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>

// ---- 独立 MD5（RFC1321 风格紧凑实现）----
typedef unsigned int u32;
static u32 MD5K[64] = {
0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
static int MD5R[64] = {7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21};
static void md5_one(const unsigned char* msg, size_t len, unsigned char out[16])
{
    u32 h[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    size_t total = ((len + 8) / 64 + 1) * 64;
    std::vector<unsigned char> m(total, 0);
    memcpy(&m[0], msg, len);
    m[len] = 0x80;
    u64bits_bits: ;
    unsigned long long bl = (unsigned long long)len * 8;
    memcpy(&m[total - 8], &bl, 8);
    for (size_t off = 0; off < total; off += 64)
    {
        u32 w[16], a = h[0], b = h[1], cc = h[2], d = h[3];
        memcpy(w, &m[off], 64);
        for (int i = 0; i < 64; i++)
        {
            u32 f, g;
            if (i < 16) { f = (b & cc) | (~b & d); g = i; }
            else if (i < 32) { f = (d & b) | (~d & cc); g = (5*i + 1) & 15; }
            else if (i < 48) { f = b ^ cc ^ d; g = (3*i + 5) & 15; }
            else { f = cc ^ (b | ~d); g = (7*i) & 15; }
            u32 tmp = d; d = cc; cc = b;
            b = b + ((a + f + MD5K[i] + w[g]) << MD5R[i] | (a + f + MD5K[i] + w[g]) >> (32 - MD5R[i]));
            a = tmp;
        }
        h[0] += a; h[1] += b; h[2] += cc; h[3] += d;
    }
    memcpy(out, h, 16);
}
#define u64bits_bits_done
static void MD5String(const char* s, BYTE out[16]) { md5_one((const unsigned char*)s, strlen(s), out); }

// u-sweep：k1=pw[4:12], k2=pw[12:16]+md5[0:4], k3=pw[8:16]（pw[15]=u 未知）
// 对每个 u 解密首块查 "XPAK"；两种密钥顺序都试。
int main(int argc, char** argv)
{
    if (argc < 2) { printf("usage: usweep <file.co>\n"); return 1; }
    FILE* f = fopen(argv[1], "rb");
    if (!f) { printf("open fail\n"); return 1; }
    unsigned char buf[4096];
    long sz = (long)fread(buf, 1, sizeof(buf), f); fclose(f);
    printf("file: %s size=%ld\nhead32: ", argv[1], sz);
    for (int i = 0; i < 32 && i < sz; i++) printf("%02x", buf[i]);
    printf("\n");

    const char* pw = "xuzhao20041108";
    BYTE md5[16];
    MD5String(pw, md5);
    printf("md5: "); for (int i = 0; i < 16; i++) printf("%02x", md5[i]); printf("\n");

    for (int order = 0; order < 2; order++)
    for (int u = 0; u < 256; u++)
    {
        BYTE k1[8], k2[8], k3[8];
        memcpy(k1, pw + 4, 8);                        // ao200411
        memcpy(k2, pw + 12, 4);                       // '0','8',0x00,u
        k2[3] = (BYTE)u;
        memcpy(k2 + 4, md5, 4);                       // md5[0:4]
        memcpy(k3, pw + 8, 8);                        // 041108 00 u
        k3[7] = (BYTE)u;
        BYTE *a, *b, *c;
        if (order == 0) { a = k1; b = k2; c = k3; }
        else { a = k3; b = k2; c = k1; }
        std::vector<BYTE> blk(buf, buf + 8);
        DecryptHeaderData(&blk[0], 8, a, b, c);
        if (memcmp(&blk[0], "XPAK", 4) == 0)
        {
            printf("HIT order=%d u=0x%02x: XPAK! head16: ", order, u);
            std::vector<BYTE> more(buf, buf + (sz & ~7L));
            DecryptHeaderData(&more[0], (long)more.size(), a, b, c);
            for (int i = 0; i < 16; i++) printf("%c", (more[i] >= 32 && more[i] < 127) ? more[i] : '.');
            printf("  hex: ");
            for (int i = 0; i < 16; i++) printf("%02x", more[i]);
            printf("\n");
            char nm[128]; snprintf(nm, sizeof(nm), "uXXXX_hit_o%d_u%02x.bin", order, u);
            FILE* o = fopen(nm, "wb");
            fwrite(&more[0], 1, more.size(), o); fclose(o);
            printf("saved %s\n", nm);
        }
        // 也试 DAT 表方向
        std::vector<BYTE> blk2(buf, buf + 8);
        DecryptData(&blk2[0], 8, a, b, c);
        if (memcmp(&blk2[0], "XPAK", 4) == 0)
            printf("HIT-DAT order=%d u=0x%02x: XPAK!\n", order, u);
    }
    printf("sweep done\n");
    return 0;
}
