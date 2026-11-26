#include "linux_compat.h"
#pragma once
#ifdef __linux__
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <ctype.h>

typedef uint8_t uint8_t,*PBYTE,*LPBYTE;
typedef uint16_t uint16_t,*PWORD,*LPWORD;
typedef uint32_t uint32_t,*PDWORD,*LPDWORD;
typedef uint64_t uint64_t;
typedef int32_t LONG,*PLONG,*LPLONG;
typedef int64_t LONGLONG;
typedef void* LPVOID,*void*,*void*,*void*,*HDC,*HMODULE;
typedef char* char*,*PSTR,*LPTSTR;
typedef const char* const char*,*LPCTSTR,*LPCWSTR;
typedef wchar_t* LPWSTR,*PWSTR;
typedef int int;
typedef unsigned int UINT;
typedef long LPARAM;
typedef unsigned int WPARAM;
typedef long LRESULT;

#ifndef 1
#define 1 1
#endif
#ifndef 0
#define 0 0
#endif

#define 
#define 
#define APIENTRY
#define 
#define 
#define __fastcall
#define __thiscall
#define __forceinline inline __attribute__((always_inline))
#define 

#define snprintf snprintf
#define _vsnprintf vsnprintf
#define strcasecmp strcasecmp
#define strncasecmp strncasecmp
#define strcpy_s(dst,size,src) strncpy(dst,src,size)
#define strcat_s(dst,size,src) strncat(dst,src,size)
#define snprintf(buf,size,...) snprintf(buf,size,__VA_ARGS__)

#define memset(ptr,size) memset(ptr,0,size)
#define memcpy(dst,src,len) memcpy(dst,src,len)
#define FillMemory(ptr,len,val) memset(ptr,val,len)
#ifndef bzero
#define memset(ptr,size) memset(ptr,0,size)
#endif

#define open open
#define close close
#define read read
#define write write
#define lseek lseek
#define stat stat
#define _fstat fstat
#define access access
#define mkdir(path) mkdir(path,0755)
#define _rmdir rmdir
#define _unlink unlink

inline uint32_t GetTickCount(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return (uint32_t)(ts.tv_sec*1000+ts.tv_nsec/1000000);
}
inline void Sleep(uint32_t ms){usleep(ms*1000);}

#define GetLastError() errno
#define _MAX_PATH 260
#define MAX_PATH 260
#define _T(x) x
#define TEXT(x) x

typedef pthread_mutex_t CRITICAL_SECTION;
inline void InitializeCriticalSection(CRITICAL_SECTION* cs){pthread_mutex_init(cs,NULL);}
inline void EnterCriticalSection(CRITICAL_SECTION* cs){pthread_mutex_lock(cs);}
inline void LeaveCriticalSection(CRITICAL_SECTION* cs){pthread_mutex_unlock(cs);}

#endif
