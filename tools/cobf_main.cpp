#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <zlib.h>

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

// ---- 明文魔数判定 ----
static int magic_score(const unsigned char* p, long n)
{
    if (n < 16) return 0;
    if (memcmp(p, "XPAK", 4) == 0) return 100;
    if (p[0] == 0x1f && p[1] == 0x8b) return 90;
    if (p[0] == 0x78 && (p[1] & 0x0f) == 8) return 80;
    if (memcmp(p, "PK\x03\x04", 4) == 0) return 70;
    if (memcmp(p, "<?xml", 5) == 0 || memcmp(p, "<files", 6) == 0) return 95;
    // 大段可打印 ASCII 也算命中（flist/ini 明文）
    int printable = 0;
    for (int i = 0; i < 64 && i < n; i++)
        if (p[i] >= 0x20 && p[i] < 0x7f) printable++;
    if (printable >= 58) return 60;
    return 0;
}

struct Res { int score; const char* pw; const char* tag; long skip; };

int main(int argc, char** argv)
{
    if (argc < 2) { printf("usage: cobf <file.co> [more.co...]\n"); return 1; }

    const char* pws[] = {
        "20210112taozhi", "xuzhao20041108", "", "taozhi", "zhengtu",
        "ztgame", "ztgame.com.cn", "giant", "patchupdate", "20041108",
        "20210112", "autoupdate", "zgupd001", "ztgame2004", "shanghai",
        "xuzhao", "zhengtuonline", "zt2", "123456", "giant111",
    };
    int npw = (int)(sizeof(pws) / sizeof(pws[0]));

    for (int a = 1; a < argc; a++)
    {
        FILE* f = fopen(argv[a], "rb");
        if (!f) { printf("open fail %s\n", argv[a]); continue; }
        fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
        unsigned char* buf = (unsigned char*)malloc(sz + 64);
        fread(buf, 1, sz, f); fclose(f);
        printf("==== %s size=%ld\n", argv[a], sz);

        // 0) 裸 zlib/gzip 偏移扫描（不加解密）
        for (long off = 0; off < 256 && off + 16 < sz; off++)
        {
            if (buf[off] == 0x1f && buf[off+1] == 0x8b)
                printf("  RAW-GZIP at off %ld\n", off);
            if (buf[off] == 0x78 && (buf[off+1] & 0x0f) == 8)
            {
                // 试 inflate
                unsigned char out[1 << 20];
                uLongf outn = sizeof(out);
                if (uncompress(out, &outn, buf + off, (uLong)(sz - off)) == Z_OK)
                    printf("  RAW-ZLIB OK at off %ld -> %lu bytes: %.40s\n", off, outn, out);
            }
        }

        // 1) 密码 × 表 × skip 爆破
        std::vector<Res> hits;
        for (int pi = 0; pi < npw; pi++)
        {
            BYTE md5[16]; BYTE key1[8], key2[8], key3[8];
            MD5String((char*)pws[pi], md5);
            memcpy(key1, md5, 8); memcpy(key2, md5 + 8, 8); memcpy(key3, md5 + 4, 8);
            long sz8 = sz & ~7L;
            for (int which = 0; which < 2; which++)
            for (long skip = 0; skip <= 24 && skip + 8 <= sz8; skip++)
            {
                std::vector<BYTE> b(buf + skip, buf + sz8);
                long n = (long)b.size(); n -= n % 8;
                if (which == 0) DecryptHeaderData(&b[0], n, key1, key2, key3);
                else DecryptData(&b[0], n, key1, key2, key3);
                int sc = magic_score(&b[0], n);
                if (sc >= 60)
                {
                    printf("  HIT pw='%s' %s skip=%ld score=%d head=%.24s\n",
                        pws[pi], which ? "DAT" : "HDR", skip, sc, &b[0]);
                    char nm[256]; snprintf(nm, sizeof(nm), "hit_%s_%s_%ld.bin",
                        pws[pi][0] ? pws[pi] : "empty", which ? "DAT" : "HDR", skip);
                    FILE* o = fopen(nm, "wb"); fwrite(&b[0], 1, n, o); fclose(o);
                }
            }
        }
        if (hits.empty()) printf("  (no hits this file)\n");
        free(buf);
    }
    return 0;
}
