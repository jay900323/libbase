#ifndef _THREAD_LOCK_H
#define _THREAD_LOCK_H
	
#include "zx_base.h"
#include "zx_except.h"
#include <string>
using namespace std;

#define Z_MUTEX_BUSY  1
#define Z_MUTEX_ERROR  -1

#define Z_THREAD_MUTEX_NESTED  1  /*The same thread allows nested use*/
#define Z_THREAD_MUTEX_UNNESTED  2

#ifdef _WIN32
typedef enum thread_mutex_type {
	thread_mutex_critical_section,
	thread_mutex_unnested_event,
	thread_mutex_nested_mutex
} thread_mutex_type;

typedef struct  
{
	thread_mutex_type type;
	HANDLE handle;
	CRITICAL_SECTION  section;
}z_thread_mutex_t;

#else

typedef struct  
{
	pthread_mutex_t mutex;
}z_thread_mutex_t;
#endif

BASE_API  int z_thread_mutex_create(z_thread_mutex_t *mutex, unsigned int flags);
BASE_API  int z_thread_mutex_lock(z_thread_mutex_t *mutex);
BASE_API  int z_thread_mutex_trylock(z_thread_mutex_t *mutex);
BASE_API  int z_thread_mutex_unlock(z_thread_mutex_t *mutex);
BASE_API  int z_thread_mutex_destroy(z_thread_mutex_t *mutex);

	
/**
* @brief 线程互斥对象
*/
struct BASE_CLASS ZxThreadMutexException : public Exception
{
	ZxThreadMutexException(const string &buffer) : Exception(buffer) {};
	ZxThreadMutexException(const string &buffer, int err) : Exception(buffer, err) {};
	~ZxThreadMutexException() throw() {};
};

/* 互斥对象，可重入 */
class BASE_CLASS ZxThreadMutex
{
public:

	ZxThreadMutex();
	virtual ~ZxThreadMutex();

	/**
	* @brief 加锁
	*/
	void lock() const;

	/**
	* @brief 尝试锁
	*
	* @return bool
	*/
	bool tryLock() const;

	/**
	* @brief 解锁
	*/
	void unlock() const;

	/**
	* @brief 加锁后调用unlock是否会解锁，
	*        给TC_Monitor使用的 永远返回true
	* @return bool
	*/
	bool willUnlock() const { return true; }

protected:

	// noncopyable
	ZxThreadMutex(const ZxThreadMutex&);
	void operator=(const ZxThreadMutex&);

	/**
	* @brief 计数
	*/
	int count() const;

	/**
	* @brief 计数
	*/
	void count(int c) const;

protected:
	mutable z_thread_mutex_t _mutex;

};


#endif