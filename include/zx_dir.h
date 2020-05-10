#ifndef _ZX_DIRENT_H
#define _ZX_DIRENT_H
	
#ifdef _WIN32

#include <sys/types.h>
#include "zx_base.h"

struct dirent
{
	long d_ino;              /* inode number*/
	off_t d_off;             /* offset to this dirent*/
	unsigned short d_reclen; /* length of this d_name*/
	unsigned char d_type;    /* the type of d_name*/
	char d_name[256];          /* file name (null-terminated)*/
};

typedef struct _dirdesc {
	int     dd_fd;      /** file descriptor associated with directory */
	long    dd_loc;     /** offset in current buffer */
	long    dd_size;    /** amount of data returned by getdirentries */
	char    *dd_buf;    /** data buffer */
	int     dd_len;     /** size of data buffer */
	long    dd_seek;    /** magic cookie returned by getdirentries */
	int finished;
	HANDLE handle;
	struct dirent entry;
} DIR;
#endif

# define __dirfd(dp)    ((dp)->dd_fd)

#ifdef _WIN32
BASE_API DIR *z_opendir(const char *name);
BASE_API struct dirent *z_readdir(DIR *dp);
BASE_API int z_closedir(DIR *d);
BASE_API int z_readdir_r(DIR *dp, struct dirent* entry, struct dirent **result);
#else
#define z_opendir opendir
#define z_readdir readdir
#define z_closedir closedir
#define z_readdir_r readdir_r
#endif

/*
* @brief 获取当前目录下的所有文件及目录，不包括子目录
   @return int  文件及目录数目，-1-失败
*/
BASE_API int z_scandir(const char *dirname, struct dirent **namelist[], int *pfiles, int *pvector_size, int(*selector)(const char *dirname, const struct dirent *entry), int(*compare)(const struct dirent **a, const struct dirent **b));
/*
* @brief 递归扫描目录
  @return int  文件及目录数目
*/
BASE_API int z_scandir_recursive(const char *dirname, struct dirent **namelist[], int *nfiles, int *vector_size, int(*selector)(const char *dirname, const struct dirent *entry), int(*compare)(const struct dirent **a, const struct dirent **b));
/*
* @brief 释放dirent数组
*/
BASE_API void free_dirent_list(struct dirent **namelist[], int nfiles);
/*
* @brief 获取当前进程的主程序路径
   @return int  文件及目录数目
   @return int  0 - 成功
*/
BASE_API int z_get_exec_path(char *dir, int len);
/*
* @brief 设置当前工作目录
  @return int  0 - 成功
*/
BASE_API int z_chdir(const char *dir);

/**
* @brief 判断目录是否存在
*
* @param dirname 要目录名称
* @return bool  0-创建成功 ，-1-创建失败
*/
BASE_API int z_dir_access(const char *dirname);
/**
* @brief 创建目录, 如果目录已经存在返回0
*
* @param sFullPath 要创建的目录名称
* @return int  0-成功 ，-1-失败
*/
BASE_API int z_mkdir(const char *dirname);

/**
* @brief 循环创建目录, 如果目录已经存在
*
* @param sFullPath 要创建的目录名称
* @return int  0-成功 ，-1-失败
*/
BASE_API int z_mkdir_recursive(const char *path);

/**
* @brief 删除目录
*
* @param dir 要删除的目录名称
* @return int  0-成功 ，-1-失败
*/
BASE_API int z_remove_dir(const  char *dir);
/**
* @brief 递归删除目录及其目录下的所有文件、子目录
*
* @param dir 要删除的目录名称
* @return int  0-成功 ，-1-失败
*/
BASE_API int z_delete_dir_recursive(const char *dirname);

#endif
