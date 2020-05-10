#ifndef _PROC_H
#define _PROC_H
	
#include "zx_base.h"

#ifdef _WIN32
typedef DWORD pid_t;
typedef HANDLE fid_t;
#else
#include <sys/unistd.h>
#include <sys/wait.h>
typedef int fid_t;
#endif

#define Z_PROC_NOWAIT		0
#define Z_PROC_WAIT		1

#define Z_PROGRAM  1
#define Z_PROGRAM_ENV  2

typedef struct {
    fid_t child_in;
    fid_t child_out;
    fid_t child_err;
    char *currdir;
	int detached;	/*create new console*/
	int cmdtype;
#ifndef _WIN32
	const char * const *env;	/*env {"PATH=/usr/bin", "USER=lei"}*/
#endif
}z_procattr_t;

typedef struct{
	pid_t pid;

#if defined(_WIN32)	
	HANDLE hproc;
#endif
}z_proc_t;

/*
	参数说明
	progname: 进程路径 /bin/ls
	args: 命令选项  windows 如 -t a.xml   linux :  ls /home
*/
BASE_API  int z_proc_create(z_proc_t *proc_t, const char *progname, const char * const *args, z_procattr_t *attr);
BASE_API  int z_proc_wait(z_proc_t *proc, int waithow);


#endif
