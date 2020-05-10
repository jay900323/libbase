#ifndef _ZX_LOCK_H
#define _ZX_LOCK_H

#include "zx_base.h"
#include "zx_except.h"

using namespace std;
/**
* @brief  锁异常
*/
struct ZxLockException : public Exception
{
	ZxLockException(const string &buffer) : Exception(buffer) {};
	ZxLockException(const string &buffer, int err) : Exception(buffer, err) {};
	~ZxLockException() throw() {};
};

/**
* @brief  锁模板类其他具体锁配合使用，
* 构造时候加锁，析够的时候解锁
*/
template <typename T>
class ZxLockT
{
public:

	/**
	* @brief  构造函数，构造时枷锁
	*
	* @param mutex 锁对象
	*/
	ZxLockT(const T& mutex) : _mutex(mutex)
	{
		_mutex.lock();
		_acquired = true;
	}

	/**
	* @brief  析构，析构时解锁
	*/
	virtual ~ZxLockT()
	{
		if (_acquired)
		{
			_mutex.unlock();
		}
	}

	/**
	* @brief  上锁, 如果已经上锁,则抛出异常
	*/
	void acquire() const
	{
		if (_acquired)
		{
			throw ZxLockException("thread has locked!");
		}
		_mutex.lock();
		_acquired = true;
	}

	/**
	* @brief  尝试上锁.
	*
	* @return  成功返回true，否则返回false
	*/
	bool tryAcquire() const
	{
		_acquired = _mutex.tryLock();
		return _acquired;
	}

	/**
	* @brief  释放锁, 如果没有上过锁, 则抛出异常
	*/
	void release() const
	{
		if (!_acquired)
		{
			throw ZxLockException("thread hasn't been locked!");
		}
		_mutex.unlock();
		_acquired = false;
	}

	/**
	* @brief  是否已经上锁.
	*
	* @return  返回true已经上锁，否则返回false
	*/
	bool acquired() const
	{
		return _acquired;
	}

protected:

	/**
	* @brief 构造函数
	* 用于锁尝试操作，与TC_LockT相似
	*
	*/
	ZxLockT(const T& mutex, bool) : _mutex(mutex)
	{
		_acquired = _mutex.tryLock();
	}

private:

	// Not implemented; prevents accidental use.
	ZxLockT(const ZxLockT&);
	ZxLockT& operator=(const ZxLockT&);

protected:
	/**
	* 锁对象
	*/
	const T&        _mutex;

	/**
	* 是否已经上锁
	*/
	mutable bool _acquired;
};

#endif