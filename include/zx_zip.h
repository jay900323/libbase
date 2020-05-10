#ifndef _ZX_ZIP_H_
#define _ZX_ZIP_H_

#include "zx_base.h"

class BASE_CLASS ZxZip
{
public:
	ZxZip();
	~ZxZip();
	/**
	* @brief  获取当前解压文件或者压缩文件的状态信息
	*/
	std::string get_file_status() const;
	/*
	* @brief  获取压缩包内全局注释文本
	*/
	std::string get_global_comment() const;

	/**
	* @brief 压缩文件 或者文件夹 内部自动判断
	* @param strSourceFile 要被压缩的文件或文件路径全称
	* @param strSaveFile 要生成的zip压缩包名全路径
	* @return 失败返回 false 成功返回 true
	*/
	bool compress_zip_package(const std::string strSourceFile, const std::string strSaveFile);
	
	/**
	* @brief 解压zip 包文件到指定路径
	* @param strSourceZipPath 要被解压的zip文件包全路径
	* @param strDestZipPath 将要解压到的本地路径
	* @param 成功返回 解压文件的数量 失败返回 null
	*/
	DWORD uncompress_zip_package(const std::string strSourceZipPath, const std::string strDestZipPath);

	/*
	@ 压缩文件 或者文件夹 内部自动判断
	@ strSourceFile 要被压缩的文件或文件路径全称
	@ strSaveFile 要生成的zip压缩包名全路径
	@ nMode 表示压缩文件还是文件夹 0 是文件 1 是文件夹
	@ 失败返回 false 成功返回 true
	*/
	bool compress_zip_package_ex(const std::string strSourceFile, const std::string strSaveFile, int nMode);

private:
	/*
	@ 当前操作解压文件状态
	*/
	std::string file_status;
	/*
	@ 压缩包内全局注释文本
	*/
	std::string global_comment;


};


#endif