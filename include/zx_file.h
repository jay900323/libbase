#ifndef _ZFILE_H
#define _ZFILE_H

#include "zx_base.h"

/**
* @ 判断文件是否存在 存在返回0 否则返回-1
*/
BASE_API int z_is_file_exist(const char *file);
/**
* @ 判断文件或目录是否存在 存在返回0 否则返回-1
*/
BASE_API int z_is_file_or_dir_exist(const char *file);
BASE_API int z_file_read(const char *filename, char **buf, int *len);
BASE_API int z_file_size(const char *filename);
BASE_API int z_file_write(const char *filename, const char *buf, int len);
BASE_API int z_delete_file(const char *file);
BASE_API char* z_full_path(const char *relative_path, char *fullpath, int full_len);
BASE_API int z_file_rename(const char *oldname, const char *newname);


#endif