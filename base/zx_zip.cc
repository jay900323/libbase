#include "zx_zip.h"
#include "zx_file.h"
#include "zx_str.h"
#include "zx_dir.h"

extern "C"
{
#include "minizip/unzip.h"
#include "minizip/zip.h"
}

#include <fstream>
#include <algorithm>
#pragma comment(lib,"zlibstatic.lib")

/*
@ 将文件或者文件夹添加到zip压缩包中
@ ZipFile zipFile 句柄
@ strFileInZipName 文件或者文件夹名称
@ strPath 如果 strPath 为null表示 是文件夹 否则是文件
*/
bool addFileZip(zipFile ZipFile, const std::string strFileInZipName, const std::string strPath);
/*
@ 递归式遍历文件夹下所有文件并添加到zip压缩包
@ ZipFile zipFile 句柄
@ strFileInZipName 文件或者文件夹名称
@ strPath 相对zip根目录的路径
*/
bool  packFolderToZip(zipFile ZipFile, const std::string strFileInZipName, const std::string strPath);

/*
@ 从内存中写文件数据到本地
@ szFileNmae 写出文件的全路径
@ pFileBuffer 写出文件的数据指针
@ dwFileSize 欲写出文件的大小长度
@ 失败返回 -1 成功 返回实际写出文件大小
*/
unsigned long WriteFileBuffer(std::string szFileNmae, void* pFileBuffer, unsigned long dwFileSize);

ZxZip::ZxZip()
	:file_status(""),
	global_comment("")
{
}


ZxZip::~ZxZip()
{
}


std::string ZxZip::get_file_status() const
{
	return file_status;
}

std::string ZxZip::get_global_comment() const
{
	return global_comment;
}

bool addFileZip(zipFile ZipFile, const std::string strFileInZipName, const std::string strPath)
{
	//初始化写入zip的文件信息  
	bool bResult = false;
	zip_fileinfo zi;
	memset(&zi, 0, sizeof(zip_fileinfo));
	std::string strFIlename = strFileInZipName;
	std::string strReadFile;
	if (strPath.empty())
		strFIlename += "/";
	//在zip文件中创建新文件  
	zipOpenNewFileInZip(ZipFile, strFIlename.c_str(), &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
	if (!strPath.empty())
	{
		char* pBuffer = nullptr;
		int filesize = 0;
		if (!z_file_read(strPath.c_str(), &pBuffer, &filesize))
		{
			zipWriteInFileInZip(ZipFile, pBuffer, filesize);
			delete pBuffer;
			pBuffer = nullptr;
			bResult = true;
		}
	}
	zipCloseFileInZip(ZipFile);
	return bResult;
}
#ifdef _WIN32
bool packFolderToZip(zipFile ZipFile, const std::string strFileInZipName, const std::string strPath)
{
	WIN32_FIND_DATAA file_dta;
	std::string filePath = strFileInZipName;
	char szFullFilePath[MAX_PATH];
	char szTmpFolderPath[MAX_PATH];
	char szTmpFileName[MAX_PATH];
	unsigned long dwEerror = NULL;
	char* findStr = nullptr;
	int lens = NULL;
	ZeroMemory(&file_dta, sizeof(WIN32_FIND_DATAA));
	ZeroMemory(szFullFilePath, MAX_PATH);
	ZeroMemory(szTmpFolderPath, MAX_PATH);
	ZeroMemory(szTmpFileName, MAX_PATH);
	strcpy(szFullFilePath, filePath.c_str());
	size_t position = filePath.find_last_of("\\");
	if (position == std::string::npos)
		return dwEerror;

	lens = filePath.length() - position;
	if (lens > 1)
	{
		strcat(szFullFilePath, "\\*.*");
	}
	else if (lens == 1)
	{
		strcat(szFullFilePath, "*.*");
	}
	else
	{
		return dwEerror;
	}
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = FindFirstFileA(szFullFilePath, &file_dta);
	dwEerror = ::GetLastError();
	if (INVALID_HANDLE_VALUE == hFile)
		return dwEerror;
	while (::FindNextFileA(hFile, &file_dta))
	{
		if (!lstrcmpi(file_dta.cFileName, ".") || !lstrcmpi(file_dta.cFileName, ".."))
			continue;
		sprintf_s(szFullFilePath, "%s\\%s", filePath.c_str(), file_dta.cFileName);
		if (file_dta.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ZeroMemory(szTmpFolderPath, MAX_PATH);
			if (strPath.empty())
			{
				sprintf_s(szTmpFolderPath, "%s", file_dta.cFileName);		//如果目录是空的话就添加到根目录
			}
			else
			{

				sprintf_s(szTmpFolderPath, "%s/%s", strPath.c_str(), file_dta.cFileName);	//如果 目录不是空添加到当前目录
			}
			addFileZip(ZipFile, szTmpFolderPath, "");
			packFolderToZip(ZipFile, szFullFilePath, szTmpFolderPath);
			continue;
		}
		else
		{
			if (strPath.empty())
				addFileZip(ZipFile, file_dta.cFileName, szFullFilePath);
			else
			{
				ZeroMemory(szTmpFileName, MAX_PATH);
				if (strPath.empty())
				{
					addFileZip(ZipFile, file_dta.cFileName, szFullFilePath);
				}
				else
				{
					sprintf_s(szTmpFileName, "%s/%s", strPath.c_str(), file_dta.cFileName);
					addFileZip(ZipFile, szTmpFileName, szFullFilePath);
				}
			}
		}
	}
	::FindClose(hFile);
	return dwEerror;
}

#else
bool packFolderToZip(zipFile ZipFile, const std::string strFileInZipName, const std::string strPath)
{
	std::string filePath = strFileInZipName;
	char szFullFilePath[MAX_PATH];
	char szTmpFolderPath[MAX_PATH];
	char szTmpFileName[MAX_PATH];
	unsigned long dwEerror = NULL;
	char* findStr = nullptr;
	int lens = NULL;

	memset(szFullFilePath, 0, MAX_PATH);
	memset(szTmpFolderPath, 0, MAX_PATH);
	memset(szTmpFileName, 0, MAX_PATH);
	strcpy(szFullFilePath, filePath.c_str());
	int position = filePath.find_last_of(DEFAULT_SLASH_STR);
	printf("%d\n", position);
	if (position == -1)
		return dwEerror;

	struct dirent *direntp;
	DIR *dir = opendir(szFullFilePath);
	if (NULL == dir)
		return dwEerror;
	
	while ((direntp = readdir(dir)) != NULL)
	{
		if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
			continue;
		
		sprintf(szFullFilePath, "%s%c%s", filePath.c_str(), DEFAULT_SLASH, direntp->d_name);
		
		if (direntp->d_type & DT_DIR)
		{
			memset(szTmpFolderPath, 0, MAX_PATH);
			if (strPath.empty())
			{
				sprintf(szTmpFolderPath, "%s", direntp->d_name);  		//如果目录是空的话就添加到根目录
			}
			else
			{

				sprintf(szTmpFolderPath, "%s%c%s", strPath.c_str(), DEFAULT_SLASH, direntp->d_name);   	//如果 目录不是空添加到当前目录
			}
			addFileZip(ZipFile, szTmpFolderPath, "");
			packFolderToZip(ZipFile, szFullFilePath, szTmpFolderPath);
			continue;
		}
		else
		{
			if (strPath.empty())
				addFileZip(ZipFile, direntp->d_name, szFullFilePath);
			else
			{
				memset(szTmpFileName, 0, MAX_PATH);
				if (strPath.empty())
				{
					addFileZip(ZipFile, direntp->d_name, szFullFilePath);
				}
				else
				{
					sprintf(szTmpFileName, "%s%c%s", strPath.c_str(), DEFAULT_SLASH, direntp->d_name);
					addFileZip(ZipFile, szTmpFileName, szFullFilePath);
				}
			}
		}
	}
	closedir(dir);
	return dwEerror;
}
#endif

bool ZxZip::compress_zip_package(const std::string strSourceFile, const std::string strSaveFile)
{
	bool bResult = false;
	
	if (strSourceFile.empty() || strSaveFile.empty()) return bResult;
	if (z_is_file_or_dir_exist(strSourceFile.c_str())) {
		return bResult;
	}

	if (!z_dir_access(strSourceFile.c_str()))
	{
		//表明是一个文件夹
		bResult = compress_zip_package_ex(strSourceFile, strSaveFile, 1);
	}
	else
	{
		//表明是一个文件
		bResult = compress_zip_package_ex(strSourceFile, strSaveFile, 0);
	}

	return bResult;
}

bool ZxZip::compress_zip_package_ex(const std::string strSourceFile, const std::string strSaveFile, int nMode)
{
	bool bResult = false;
	zipFile zipFileHandle = NULL;
	//创建一个zip压缩包文件 失败返回null
	zipFileHandle = zipOpen(strSaveFile.c_str(), APPEND_STATUS_CREATE);
	if (!zipFileHandle) return bResult;
	if (nMode)
	{
		bResult = packFolderToZip(zipFileHandle, strSourceFile, "");
	}
	else
	{
		char szFileName[MAX_PATH];
		memset(szFileName, 0, MAX_PATH);
		strcpy(szFileName, strSourceFile.c_str());
		bResult = addFileZip(zipFileHandle, z_filepath_name_get(szFileName), strSourceFile);
	}
	zipClose(zipFileHandle, nullptr);
	return bResult;
}

DWORD ZxZip::uncompress_zip_package(const std::string strSourceZipPath, const std::string strDestZipPath)
{
	DWORD dwResult = NULL;
	std::string rootPath;
	unzFile unZipFileHandle;
	if (strSourceZipPath.empty() || strDestZipPath.empty()) return dwResult;
	if (z_is_file_or_dir_exist(strDestZipPath.c_str()) || z_is_file_or_dir_exist(strSourceZipPath.c_str())) {
		return dwResult;
	}
	unZipFileHandle = unzOpen(strSourceZipPath.c_str());
	if (!unZipFileHandle)return dwResult;
	rootPath = strDestZipPath;
	if (strDestZipPath.at(strDestZipPath.length() - 1) != DEFAULT_SLASH)
		rootPath += DEFAULT_SLASH;
	unz_global_info global_info;
	if (UNZ_OK != unzGetGlobalInfo(unZipFileHandle, &global_info))
	{
		file_status = "获取全局zip信息失败!";
		unzClose(unZipFileHandle);
		unZipFileHandle = NULL;
		return dwResult;
	}
	/*获取zip注释内容*/
	global_comment = "";
	if (global_info.size_comment > 0)
	{
		char* pComment = new char[global_info.size_comment + 1];
		if (pComment)
		{
			memset(pComment, 0, global_info.size_comment + 1);
			int nResult = unzGetGlobalComment(unZipFileHandle, pComment, global_info.size_comment + 1);
			if (nResult > 0)
				global_comment = pComment;
			delete[] pComment;
			pComment = nullptr;
		}
	}
	int zipinfo = unzGoToFirstFile(unZipFileHandle);
	if (UNZ_OK != zipinfo)
	{
		file_status = "无法获取zip包内文件信息zip包可能是null的!";
		unzClose(unZipFileHandle);
		unZipFileHandle = NULL;
		return dwResult;
	}
	while (UNZ_OK == zipinfo)
	{
		unz_file_info file_info;
		char szZipName[MAX_PATH];
		std::string strFileName;
		memset(szZipName, 0, MAX_PATH);
		unzGetCurrentFileInfo(unZipFileHandle, &file_info, szZipName, MAX_PATH, NULL, 0, NULL, 0);
		strFileName = szZipName;
		if (strFileName.empty())
			continue;
		int length = strFileName.length() - 1;

		if (strFileName[length] != DEFAULT_SLASH)
		{
			std::string Filepath = rootPath + strFileName;
#ifdef _WIN32
			replace(Filepath.begin(), Filepath.end(), '/', DEFAULT_SLASH);
#else
			replace(Filepath.begin(), Filepath.end(), '\\', DEFAULT_SLASH);
#endif
			if (UNZ_OK == unzOpenCurrentFile(unZipFileHandle))
			{
				void * FilePtr = NULL;
				FilePtr = new char[file_info.uncompressed_size];
				if (!FilePtr)
					continue;
				memset(FilePtr, 0,  file_info.uncompressed_size);
				int dwLens = unzReadCurrentFile(unZipFileHandle, FilePtr, file_info.uncompressed_size);
				if (dwLens == file_info.uncompressed_size)
				{
					dwLens = WriteFileBuffer(Filepath, FilePtr, dwLens);
				}
				delete[] FilePtr;
				FilePtr = nullptr;
				unzCloseCurrentFile(unZipFileHandle);
				printf("Zip Create File: \t %s \n", Filepath.c_str());
			}
		}
		else
		{
			std::string Filepath = rootPath + strFileName;
#ifdef _WIN32
			replace(Filepath.begin(), Filepath.end(), '/', DEFAULT_SLASH);
#else
			replace(Filepath.begin(), Filepath.end(), '\\', DEFAULT_SLASH);
#endif
			z_mkdir_recursive(Filepath.c_str());
			printf("Zip Create Folder: \t %s \n", Filepath.c_str());
		}
		zipinfo = unzGoToNextFile(unZipFileHandle);
	}
	dwResult = global_info.number_entry;
	unzClose(unZipFileHandle);
	unZipFileHandle = NULL;

	return dwResult;
}

DWORD WriteFileBuffer(std::string szFileNmae, void* pFileBuffer, DWORD dwFileSize)
{
	DWORD dwStates = -1;
	std::string szFilePath, strName;
	size_t npos = -1;
	if (szFileNmae.empty())
		return dwStates;
	npos = szFileNmae.rfind(DEFAULT_SLASH);
	if (npos != std::string::npos)
	{
		szFilePath = szFileNmae.substr(0, npos + 1);
		if (!szFilePath.empty())
		{
			if (!z_mkdir_recursive(szFilePath.c_str()))
			{
				z_file_write(szFileNmae.c_str(), (char *)pFileBuffer, dwFileSize);
			}
		}
	}

	return dwStates;
}
