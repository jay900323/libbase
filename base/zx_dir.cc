#include <stdio.h>
#include <string.h>
#include "zx_base.h"
#include "zx_dir.h"
#include "zx_file.h"

#ifdef _WIN32
DIR *z_opendir(const char *name)
{
	DIR *dir;
	WIN32_FIND_DATA FindData = {0};
	char dirname[512];
	HANDLE handle;

	dir = (DIR *)malloc(sizeof(DIR));
	if (!dir) {
		return 0;
	}
	memset(dir, 0, sizeof(DIR));
	strcpy(dirname, name);
	strcat(dirname, "\\*.*");

	//if (handle = FindFirstFileEx(dirname, FindExInfoBasic, &FindData, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH) == INVALID_HANDLE_VALUE) {
	if ((handle = FindFirstFile(dirname, &FindData)) == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		if (err == ERROR_NO_MORE_FILES || err == ERROR_FILE_NOT_FOUND) {
			dir->finished = 1;
		}
		else {
			free(dir);
			return NULL;
		}
	}

	dir->dd_fd = 0;
	dir->handle = handle;
	return dir;
}

struct dirent *z_readdir(DIR *dp)
{
	int i;
	BOOL bf;
	WIN32_FIND_DATA FileData;

	if (!dp || dp->finished)
		return NULL;

	bf = FindNextFile(dp->handle, &FileData);
	if (!bf)
	{
		dp->finished = 1;
		return 0;
	}

	for (i = 0; i < 256; i++)
	{
		dp->entry.d_name[i] = FileData.cFileName[i];
		if (FileData.cFileName[i] == '\0') break;
	}
	dp->entry.d_reclen = i;
	dp->entry.d_reclen = FileData.nFileSizeLow;

	if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		dp->entry.d_type = 2; /*目录*/
	}
	else
	{
		dp->entry.d_type = 1; /*文件*/
	}

	return (&dp->entry);
}

int z_readdir_r(DIR *dp, struct dirent* entry, struct dirent **result)
{
	int i ;
	BOOL bf ;
	WIN32_FIND_DATA FileData ;

	if(!dp || dp->finished) {
		*result = NULL ;
		return 0 ;
	}
	
	bf = FindNextFile(dp->handle, &FileData) ;
	if(!bf) {
		dp->finished = 1 ;
		*result = NULL ;
		return 0 ;
	}

	for(i = 0 ; i < 256 ; i++) {
		dp->entry.d_name[i] = FileData.cFileName[i] ;
		if(FileData.cFileName[i] == '\0') break ;
	}
	dp->entry.d_reclen = i ;
	dp->entry.d_reclen = FileData.nFileSizeLow ;

	if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		dp->entry.d_type = 2 ;  /*目录*/
	}
	else {
		dp->entry.d_type = 1 ; /*文件*/
	}
	memcpy(entry, &dp->entry, sizeof(*entry)) ;

	*result = &dp->entry ;

	return 0 ;
}

int z_closedir(DIR *d)
{
	if(!d) return - 1 ;
	FindClose(d->handle) ;
	free(d) ;
	return 0 ;
}
#endif

int z_scandir(const char *dirname, struct dirent **namelist[], int *pfiles, int *pvector_size, int(*selector)(const char *dirname, const struct dirent *entry), int(*compare)(const struct dirent **a, const struct dirent **b))
{
	DIR *dirp = NULL ;
	struct dirent **vector = NULL ;
	int vector_size = *pvector_size;
	int nfiles = *pfiles;
	char entry[sizeof(struct dirent)] ;
	struct dirent *dp = (struct dirent *)&entry ;

	if(namelist == NULL) {
		return - 1 ;
	}

	if(!(dirp = z_opendir(dirname))) {
		return -1 ;
	}

	while(!z_readdir_r(dirp, (struct dirent *)entry, &dp) && dp) {
		int dsize = 0 ;
		struct dirent *newdp = NULL ;

		if(selector && (*selector)(dirname, dp) == 0) {
			continue ;
		}

		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
			continue;
		}

		if(nfiles == vector_size) {
			struct dirent **newv ;
			if(vector_size == 0) {
				vector_size = 10 ;
			}
			else {
				vector_size *= 2 ;
			}


			newv = (struct dirent **) realloc(vector, vector_size * sizeof(struct dirent *));
			if (!newv) {
				return -1;
			}
			vector = newv;
		}

		dsize = sizeof(struct dirent) ;
		newdp = (struct dirent *) malloc(dsize) ;

		if(newdp == NULL) {
			goto fail ;
		}
		vector[nfiles++] = (struct dirent *) memcpy(newdp, dp, dsize) ;
	}

	z_closedir(dirp) ;

	*namelist = vector ;
	*pvector_size = vector_size;
	*pfiles = nfiles;

	if(compare) {
		qsort(*namelist, nfiles, sizeof(struct dirent *), (int(*)(const void *, const void *)) compare) ;
	}

	return nfiles ;

fail :
	while (nfiles-- > 0) {
		free(vector[nfiles]);
	}
	free(vector);
	return -1 ;
}

int z_scandir_recursive(const char *dirname, struct dirent **namelist[], int *nfiles, int *vector_size, int(*selector)(const char *dirname, const struct dirent *entry), int(*compare)(const struct dirent **a, const struct dirent **b))
{
	int rv = z_scandir(dirname, namelist, nfiles, vector_size, selector, compare);
	if (rv > 0) {
		for (int i = 0; i < rv; i++) {
			if ((*namelist)[i]->d_type == 2) {
				char path[256] = { 0 };
				sprintf(path, "%s%c%s", dirname, DEFAULT_SLASH, (*namelist)[i]->d_name);
				z_scandir_recursive(path, namelist, nfiles, vector_size, selector, compare);
			}
		}
	}
	return *nfiles;
}

int z_is_file_exist_ex(const char *filename, int iFileType)
{
	struct stat f_stat;

	if (stat(filename, &f_stat) == -1)
	{
		return -1;
	}

	if (!(f_stat.st_mode & iFileType))
	{
		return -1;
	}

	return 0;
}

int z_delete_dir_recursive(const char *dirname)
{
	if (!z_is_file_exist_ex(dirname, S_IFDIR)) {
		struct dirent **namelist;
		int nfiles = 0,vector_size = 0;
		int rv = z_scandir(dirname, &namelist, &nfiles, &vector_size, NULL, NULL);
		for (int i = 0; i < rv; i++) {
			char path[256] = { 0 };
			sprintf(path, "%s%c%s", dirname, DEFAULT_SLASH, namelist[i]->d_name);
			z_delete_dir_recursive(path);
		}
		if (z_remove_dir(dirname) == -1) {
			return -1;
		}
		return 0;
	}
	else {
		if (!z_is_file_exist_ex(dirname, S_IFREG)) {
			z_delete_file(dirname);
			return 0;
		}
		else {
			return -1;
		}
	}
}

void free_dirent_list(struct dirent **namelist[], int nfiles)
{
	while (nfiles-- > 0) {
		if ((*namelist)[nfiles]) {
			free((*namelist)[nfiles]);
			(*namelist)[nfiles] = NULL;
		}
	}
	free(*namelist);
	*namelist = NULL;
}


int z_get_exec_path(char *dir, int len)
{
	int i = 0;
	
#ifdef _WIN32
	GetModuleFileName(NULL, dir, len);
#else
	readlink("/proc/self/exe", dir, len);
#endif
	for (i = (int)strlen(dir); i > 0; i--)
	{
		if (dir[i] == DEFAULT_SLASH)
		{
			dir[i + 1] = '\0';
			break;
		}
	}
	return 0;
}

int z_remove_dir(const  char *dir)
{
#ifdef _WIN32
	return RemoveDirectory(dir)?0:-1;
#else
	return rmdir(dir);
#endif
}

int z_chdir(const char *dir) 
{
#ifdef _WIN32
	SetCurrentDirectory(dir);
#else
	chdir(dir);
#endif
	return 0;
}

int z_dir_access(const char *dirname)
{
#ifdef _WIN32
	DWORD dwAttrib = GetFileAttributes(dirname);
	if (INVALID_FILE_ATTRIBUTES != dwAttrib && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
		return 0;
	}
#else
	struct stat st;
	stat(dirname, &st);
	if (S_ISDIR(st.st_mode)) {
		return 0;
	}
#endif
	return -1;
}

int z_mkdir(const char *dirname)
{
	if (!z_dir_access(dirname)) {
		return 0;
	}
#ifdef _WIN32
	return CreateDirectory(dirname, NULL) ? 0 : -1;
#else
	return mkdir(dirname, 0777);
#endif
}

/**
*  brief：创建父目录
*  param  path 文件或目录路径路径
**/
static int dir_make_parent(char *path)
{
	char *ch = strrchr(path, DEFAULT_SLASH);
	if (!ch) {
		return -1;
	}

	*ch = '\0';
	int rv = z_mkdir(path); /* Try to make straight off */

	if (rv) { /* Missing an intermediate dir */
		rv = dir_make_parent(path);

		if (!rv) {
			rv = z_mkdir(path); /* And complete the path */
		}
	}

	*ch = DEFAULT_SLASH; /* Always replace the slash before returning */
	return rv;
}

int z_mkdir_recursive(const char *path)
{
	int rv = z_mkdir(path); /* Try to make PATH right out */

	if (rv) { /* Missing an intermediate dir */
		char dir[256] = {0};
		strcpy(dir, path);

		rv = dir_make_parent(dir); /* Make intermediate dirs */

		if (!rv) {
			rv = z_mkdir(dir);   /* And complete the path */
			return rv;
		}
	}

	return rv;
}