#include "zx_shm.h"
#include <cassert>
#include <errno.h>


ZxShm::ZxShm(size_t iShmSize, SHM_KEY iKey, bool bOwner)
{
	init(iShmSize, iKey, bOwner);
}

ZxShm::~ZxShm()
{
	if (_bOwner)
	{
		detach();
	}
}

void ZxShm::init(size_t iShmSize, SHM_KEY iKey, bool bOwner)
{
	assert(_pshm == NULL);

	_bOwner = bOwner;

#ifdef _WIN32
	HANDLE hFile = INVALID_HANDLE_VALUE;

	//这里的读写权限是面向访问此块内存的进程
	//如果设为只读则所有进程只有只读权限
	_shemID = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, iShmSize, iKey);
	if (_shemID == NULL)
	{
		_bCreate = false;
		//共享内存已创建，试图连接
		_shemID = OpenFileMapping(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, iKey);
		if (_shemID == NULL) {
			throw Zx_Shm_Exception("[ZxShm::init()] OpenFileMapping error", GetLastError());
		}
	}
	else {
		_bCreate = true;
	}

	if((_pshm = MapViewOfFile(_shemID, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0)) == NULL)
	{
		throw Zx_Shm_Exception("[ZxShm::init()] MapViewOfFile error", errno);
	}

	_shmSize = iShmSize;
	_shmKey = iKey;
#else
	//注意_bCreate的赋值位置:保证多线程用一个对象的时候也不会有问题
	//试图创建
	if ((_shemID = shmget(iKey, iShmSize, IPC_CREAT | IPC_EXCL | 0666)) < 0)
	{
		_bCreate = false;
		//有可能是已经存在同样的key_shm,则试图连接
		if ((_shemID = shmget(iKey, iShmSize, 0666)) < 0)
		{
			throw Zx_Shm_Exception("[ZxShm::init()] shmget error", errno);
		}
	}
	else
	{
		_bCreate = true;
	}

	//try to access shm
	if ((_pshm = shmat(_shemID, NULL, 0)) == (char *)-1)
	{
		throw Zx_Shm_Exception("[ZxShm::init()] shmat error", errno);
	}
#endif
	_shmSize = iShmSize;
	_shmKey = iKey;

}

int ZxShm::detach()
{
	int iRetCode = 0;
	if (_pshm != NULL)
	{
#ifdef _WIN32
		::UnmapViewOfFile(_pshm);
		::CloseHandle(_shemID);
#else
		// 当一个进程不再需要共享内存段时，它将调用shmdt()系统调用取消这个段，
		// 但是，这并不是从内核真正地删除这个段，而是把相关shmid_ds结构的
		// shm_nattch域的值减1，当这个值为0时，内核才从物理上删除这个共享段
		iRetCode = shmdt(_pshm);
#endif
		_pshm = NULL;
	}

	return iRetCode;
}

int ZxShm::del()
{
	int iRetCode = 0;
	if (_pshm != NULL)
	{
#ifdef _WIN32
		::UnmapViewOfFile(_pshm);
		::CloseHandle(_shemID);
#else
		// IPC_RMID 命令实际上不从内核删除一个段，而是仅仅把这个段标记为删除
		//实际的删除发生在最后一个进程离开这个共享段时。
		iRetCode = shmctl(_shemID, IPC_RMID, 0);
#endif
		_pshm = NULL;
	}

	return iRetCode;
}

