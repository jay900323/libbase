#include "zx_thread_mutex.h"
#include <iostream>

int z_thread_mutex_create(z_thread_mutex_t *mutex, unsigned int flags)
{
	int rv = 0;

	if(mutex == NULL){
		return -1;
	}
#ifdef _WIN32
	if (flags & Z_THREAD_MUTEX_UNNESTED) {
		mutex->type = thread_mutex_unnested_event;
		mutex->handle = CreateEvent(NULL, FALSE, TRUE, NULL);
	}
	else{
#ifdef _UNICODE
			mutex->type = thread_mutex_critical_section;
			InitializeCriticalSection(&(mutex->section));
#else
			mutex->type = thread_mutex_nested_mutex;
			mutex->handle = CreateMutex(NULL, FALSE, NULL);
#endif
	}

#else

	if (flags & Z_THREAD_MUTEX_NESTED) {
		pthread_mutexattr_t mattr;

		rv = pthread_mutexattr_init(&mattr);
		if (rv) return rv;

		rv = pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
		if (rv) {
			pthread_mutexattr_destroy(&mattr);
			return rv;
		}

		rv = pthread_mutex_init(&mutex->mutex, &mattr);
		pthread_mutexattr_destroy(&mattr);
	} else
		rv = pthread_mutex_init(&mutex->mutex, NULL);

	if (rv) {
		return errno;
	}
#endif

	return 0;
}

int z_thread_mutex_lock(z_thread_mutex_t *mutex)
{
#ifdef _WIN32
	if (mutex->type == thread_mutex_critical_section) {
		EnterCriticalSection(&mutex->section);
	}
	else {
		DWORD rv = WaitForSingleObject(mutex->handle, INFINITE);
		if ((rv != WAIT_OBJECT_0) && (rv != WAIT_ABANDONED)) {
			return (rv == WAIT_TIMEOUT) ? Z_MUTEX_BUSY : Z_MUTEX_ERROR;
		}
	}

	return 0;
#else
	int rv;

	rv = pthread_mutex_lock(&mutex->mutex);
	if (rv) {
		rv = errno;
	}
	return rv;
#endif
}

int z_thread_mutex_trylock(z_thread_mutex_t *mutex)
{
#ifdef _WIN32
	if (mutex->type == thread_mutex_critical_section) {
		if (!TryEnterCriticalSection(&mutex->section)) {
			return Z_MUTEX_BUSY;
		}
		return 0;
	}
	else {
		DWORD rv = WaitForSingleObject(mutex->handle, 0);
		if ((rv != WAIT_OBJECT_0) && (rv != WAIT_ABANDONED)) {
			return (rv == WAIT_TIMEOUT) ? Z_MUTEX_BUSY : Z_MUTEX_ERROR;
		}
		return 0;
	} 

#else
	int rv;
	rv = pthread_mutex_trylock(&mutex->mutex);
	if (rv) {
		rv = errno;
		return (rv == EBUSY) ? Z_MUTEX_BUSY : rv;
	}
	return rv;
#endif
}

int z_thread_mutex_unlock(z_thread_mutex_t *mutex)
{
#ifdef _WIN32
	if (mutex->type == thread_mutex_critical_section) {
		LeaveCriticalSection(&mutex->section);
	}
	else if (mutex->type == thread_mutex_unnested_event) {
		if (!SetEvent(mutex->handle)) {
			return Z_MUTEX_ERROR;
		}
	}
	else if (mutex->type == thread_mutex_nested_mutex) {
		if (!ReleaseMutex(mutex->handle)) {
			return Z_MUTEX_ERROR;
		}
	}else{
		return Z_MUTEX_ERROR;
	}
	return 0;

#else
	int rv;

	rv = pthread_mutex_unlock(&mutex->mutex);
	if (rv) {
		rv = errno;
	}
	return rv;
#endif
}

int z_thread_mutex_destroy(z_thread_mutex_t *mutex)
{
#ifdef _WIN32
	if (mutex->type == thread_mutex_critical_section) {
		DeleteCriticalSection(&mutex->section);
	}
	else if (mutex->type == thread_mutex_unnested_event) {
		if (!CloseHandle(mutex->handle)) {
			return Z_MUTEX_ERROR;
		}
	}
	else if (mutex->type == thread_mutex_nested_mutex) {
		if (!CloseHandle(mutex->handle)) {
			return Z_MUTEX_ERROR;
		}
	}
	return 0;
#else
	int rv;
	rv = pthread_mutex_destroy(&mutex->mutex);
	if (rv) {
		rv = errno;
	}
	return rv;
#endif
}

ZxThreadMutex::ZxThreadMutex()
{
	int rc;
	rc = z_thread_mutex_create(&_mutex, Z_THREAD_MUTEX_NESTED);
	if (rc != 0)
	{
		throw ZxThreadMutexException("[ZxThreadMutex::ZxThreadMutex] z_thread_mutex_create error", rc);
	}
}

ZxThreadMutex::~ZxThreadMutex()
{
	int rc = 0;
	rc = z_thread_mutex_destroy(&_mutex);
	if (rc != 0)
	{
		cerr << "[ZxThreadMutex::~ZxThreadMutex] z_thread_mutex_destroy error:" << string(strerror(rc)) << endl;
	}
}

void ZxThreadMutex::lock() const
{
	int rc = z_thread_mutex_lock(&_mutex);
	if (rc != 0)
	{
		if (rc == EDEADLK)
		{
			throw ZxThreadMutexException("[ZxThreadMutex::lock] z_thread_mutex_lock dead lock error", rc);
		}
		else
		{
			throw ZxThreadMutexException("[ZxThreadMutex::lock] pthread_mutex_lock error", rc);
		}
	}
}

bool ZxThreadMutex::tryLock() const
{
	int rc = z_thread_mutex_trylock(&_mutex);
	if (rc != 0 && rc != Z_MUTEX_BUSY)
	{		
		throw ZxThreadMutexException("[ZxThreadMutex::tryLock] pthread_mutex_trylock error", rc);
		
	}
	return (rc == 0);
}

void ZxThreadMutex::unlock() const
{
	int rc = z_thread_mutex_unlock(&_mutex);
	if (rc != 0)
	{
		throw ZxThreadMutexException("[ZxThreadMutex::unlock] pthread_mutex_unlock error", rc);
	}
}

int ZxThreadMutex::count() const
{
	return 0;
}

void ZxThreadMutex::count(int c) const
{
}
