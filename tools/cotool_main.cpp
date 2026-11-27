#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 2) { printf("usage: cotool <file.co>\n"); return 1; }
    FILE* f = fopen(argv[1], "rb");
    if (!f) { printf("open fail\n"); return 1; }
    fseek(f, 0, SEEK_END); long sz = ftell(f); fseek(f, 0, SEEK_SET);
    unsigned char* buf = (unsigned char*)malloc(sz + 64);
    fread(buf, 1, sz, f); fclose(f);

    BYTE md5[16]; BYTE key1[8], key2[8], key3[8];
    MD5String("20210112taozhi", md5);
    memcpy(key1, md5, 8); memcpy(key2, md5+8, 8); memcpy(key3, md5+4, 8);
    printf("md5: "); for (int i=0;i<16;i++) printf("%02x", md5[i]); printf("\n");
    printf("co size: %ld\n", sz);

    long sz8 = sz - (sz % 8);
    printf("raw head32: "); for (int i=0;i<32;i++) printf("%02x", buf[i]); printf("\n");

    for (int which = 0; which < 2; which++)
    {
        for (long skip = 0; skip <= 16 && skip + 8 <= sz8; skip += 4)
        {
            std::vector<BYTE> b(buf + skip, buf + sz8);
            long n = (long)b.size(); n -= n % 8;
            if (which == 0) DecryptHeaderData(&b[0], n, key1, key2, key3);
            else DecryptData(&b[0], n, key1, key2, key3);
            const char* tag = (which == 0) ? "HDR" : "DAT";
            if (memcmp(&b[0], "XPAK", 4) == 0)
                printf("%s: XPAK at skip %ld\n", tag, skip);
            else if (b[0] == 0x1f && b[1] == 0x8b)
                printf("%s: GZIP at skip %ld\n", tag, skip);
            else if (b[0] == 0x78 && (b[1] & 0x0f) == 8)
                printf("%s: ZLIB at skip %ld\n", tag, skip);
        }
    }
    {
        std::vector<BYTE> a(buf, buf + (sz8 & ~7L));
        std::vector<BYTE> b(a);
        DecryptHeaderData(&a[0], (long)a.size(), key1, key2, key3);
        DecryptData(&b[0], (long)b.size(), key1, key2, key3);
        printf("HDR-dec head48: ");
        for (int i = 0; i < 48; i++) printf("%c", (a[i] >= 32 && a[i] < 127) ? a[i] : '.');
        printf("\n");
        printf("DAT-dec head48: ");
        for (int i = 0; i < 48; i++) printf("%c", (b[i] >= 32 && b[i] < 127) ? b[i] : '.');
        printf("\n");
        FILE* o = fopen("dec_hdr.bin", "wb"); fwrite(&a[0], 1, (long)a.size(), o); fclose(o);
        FILE* o2 = fopen("dec_dat.bin", "wb"); fwrite(&b[0], 1, (long)b.size(), o2); fclose(o2);
    }
    free(buf);
    return 0;
}
