#ifndef __TC_SINGLETON_H__
#define __TC_SINGLETON_H__

#include <cassert>
#include <cstdlib>
#include "zx_thread_mutex.h"
#include "zx_thread_lock.h"

/////////////////////////////////////////////////
/**
* @file tc_singleton.h
* @brief  ������ .
*
* ����ʵ����
*
* û��ʵ�ֶԵ����������ڵĹ���,ʹ��ʾ����������:
*
* class A : public TC_Singleton<A, CreateStatic,  DefaultLifetime>
*
* {
*
*  public:
*
*    A(){cout << "A" << endl;}
*
*   ~A()
*
*   {
*
*     cout << "~A" << endl;
*
*
*    }
*
*    void test(){cout << "test A" << endl;}
*
* };
* ����Ĵ�����ʽ��CreatePolicyָ��, �����·�ʽ:
*
* CreateUsingNew: �ڶ��в���new����
*
* CreateStatic`: ��ջ�в���static����
*
* �����������ڹ�����LifetimePolicyָ��, �����·�ʽ:
*
* DefaultLifetime:ȱʡ�������ڹ���
*
*������������Ѿ�����, ���ǻ��е���, �ᴥ���쳣
*
* PhoneixLifetime:������������
*
* ������������Ѿ�����, ���ǻ��е���, ���ٴ���һ��
*
* NoDestroyLifetime:������
*
* ���󴴽��󲻻����������������, ͨ������ʵ���еķ�ʽ�Ϳ�����
*
*/
/////////////////////////////////////////////////

/**
* @brief ����CreatePolicy:������󴴽�����
*/
template<typename T>
class CreateUsingNew
{
public:
	/**
	* @brief  ����.
	*
	* @return T*
	*/
	static T* create()
	{
		return new T;
	}

	/**
	* @brief �ͷ�.
	*
	* @param t
	*/
	static void destroy(T *t)
	{
		delete t;
	}
};

template<typename T>
class CreateStatic
{
public:
	/**
	* @brief   ���Ŀռ�
	*/
	union MaxAlign
	{
		char t_[sizeof(T)];
		long double longDouble_;
	};

	/**
	* @brief   ����.
	*
	* @return T*
	*/
	static T* create()
	{
		static MaxAlign t;
		return new(&t) T;
	}

	/**
	* @brief   �ͷ�.
	*
	* @param t
	*/
	static void destroy(T *t)
	{
		t->~T();
	}
};

////////////////////////////////////////////////////////////////
/**
* @brief ����LifetimePolicy:���������������ڹ���
*/
template<typename T>
class DefaultLifetime
{
public:
	static void deadReference()
	{
		throw std::logic_error("singleton object has dead.");
	}

	static void scheduleDestruction(T*, void(*pFun)())
	{
		std::atexit(pFun);
	}
};

template<typename T>
class PhoneixLifetime
{
public:
	static void deadReference()
	{
		_bDestroyedOnce = true;
	}

	static void scheduleDestruction(T*, void(*pFun)())
	{
		if (!_bDestroyedOnce)
			std::atexit(pFun);
	}
private:
	static bool _bDestroyedOnce;
};
template <class T>
bool PhoneixLifetime<T>::_bDestroyedOnce = false;

template <typename T>
struct NoDestroyLifetime
{
	static void scheduleDestruction(T*, void(*)())
	{
	}

	static void deadReference()
	{
	}
};

//////////////////////////////////////////////////////////////////////
// Singleton
template
	<
	typename T,
	template<class> class CreatePolicy = CreateUsingNew,
	template<class> class LifetimePolicy = DefaultLifetime
	>
	class ZxSingleton
{
public:
	typedef T  instance_type;
	typedef volatile T volatile_type;

	/**
	* @brief ��ȡʵ��
	*
	* @return T*
	*/
	static T *getInstance()
	{
		//����, ˫check����, ��֤��ȷ��Ч��
		if (!_pInstance)
		{
			ZxLockT<ZxThreadMutex> lock(_tl);
			if (!_pInstance)
			{
				if (_destroyed)
				{
					LifetimePolicy<T>::deadReference();
					_destroyed = false;
				}
				_pInstance = CreatePolicy<T>::create();
				LifetimePolicy<T>::scheduleDestruction((T*)_pInstance, &destroySingleton);
			}
		}

		return (T*)_pInstance;
	}

	virtual ~ZxSingleton() {};

protected:

	static void destroySingleton()
	{
		assert(!_destroyed);
		CreatePolicy<T>::destroy((T*)_pInstance);
		_pInstance = NULL;
		_destroyed = true;
	}
protected:

	static ZxThreadMutex    _tl;
	static volatile T*      _pInstance;
	static bool             _destroyed;

protected:
	ZxSingleton() {}
	ZxSingleton(const ZxSingleton &);
	ZxSingleton &operator=(const ZxSingleton &);
};

template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
ZxThreadMutex ZxSingleton<T, CreatePolicy, LifetimePolicy>::_tl;

template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
bool ZxSingleton<T, CreatePolicy, LifetimePolicy>::_destroyed = false;

template <class T, template<class> class CreatePolicy, template<class> class LifetimePolicy>
volatile T* ZxSingleton<T, CreatePolicy, LifetimePolicy>::_pInstance = NULL;


#endif