#ifndef _Z_BASE_H
#define _Z_BASE_H

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <memory>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <vector>

#define		SUCCEED		0
#define		FAIL		-1
#define		NOTSUPPORTED	-2
#define		NETWORK_ERROR	-3
#define		TIMEOUT_ERROR	-4
#define		CONFIG_ERROR	-5


#define SUCCEED_OR_FAIL(result) (FAIL != (result) ? SUCCEED : FAIL)

#define Z_KIBIBYTE		1024
#define MAX_ID_LEN		64
#define MAX_FILEPATH_LEN    256	
#define MAX_STRING_LEN		2048	
#define MAX_BUFFER_LEN		65536
#define MAX_ZBX_HOSTNAME_LEN	128
#define MAX_EXECUTE_OUTPUT_LEN	(512 * Z_KIBIBYTE)

#ifndef MAX
#	define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#	define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define z_free(ptr)		\
				\
do				\
{				\
	if (ptr)		\
	{			\
		free(ptr);	\
		ptr = NULL;	\
	}			\
}				\
while (0)

#define z_fclose(file)	\
				\
do				\
{				\
	if (file)		\
	{			\
		fclose(file);	\
		file = NULL;	\
	}			\
}				\
while (0)

#define Z_MIN_PORT 1024u
#define Z_MAX_PORT 65535u

#define ARRSIZE(a)	(sizeof(a) / sizeof(*a))

#ifdef _WIN32
#define DEFAULT_SLASH '\\'
#define DEFAULT_SLASH_STR "\\"
#else
#define DEFAULT_SLASH '/'
#define DEFAULT_SLASH_STR "/"
#endif

#ifndef uint_16
#define uint_16 unsigned short
#endif

#ifndef uint_32
#define uint_16 unsigned short
#endif

//qizc
#ifndef UINT16
typedef unsigned short UINT16;
#endif

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

#ifndef DWORD
typedef unsigned long DWORD;
#endif

#ifndef ULONG32
typedef unsigned int ULONG32;
#endif

#ifndef WORD
typedef unsigned short WORD;
#endif

#ifndef USHORT
typedef unsigned short USHORT;
#endif

#ifndef INT32
typedef int INT32;
#endif

#ifndef UINT32
typedef unsigned int UINT32;
#endif

#ifndef INT64
typedef  long long INT64;
#endif

#ifndef UINT64
typedef unsigned long long UINT64;
#endif

#ifndef MAX_PATH
#define MAX_PATH (260*2)
#endif

#ifndef INT
typedef int INT;
#endif

#ifndef VOID
typedef void VOID;
#endif

#ifndef LONG
typedef long LONG;
#endif

/*
* 动态库形式导出
*/
#ifdef _WIN32
#ifndef BASE_STATIC
#if defined(BASE_DLL_EXPORTS) 
#define BASE_API extern "C" __declspec(dllexport)   
#define BASE_CPLUS_API  __declspec(dllexport)   
#define BASE_CLASS  __declspec(dllexport)  
#else  
#define BASE_API extern "C" __declspec(dllimport)
#define BASE_CPLUS_API  __declspec(dllimport)   
#define BASE_CLASS  __declspec(dllimport)  
#endif  
#endif
#endif
#endif
/*
* 静态库函数说明
*/
#ifndef BASE_API
#define BASE_API  extern "C"
#define BASE_CLASS 
#endif




