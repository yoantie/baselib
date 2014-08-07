
#ifndef _MUTEX_IMPL_H__
#define _MUTEX_IMPL_H__

#include <windows.h>

class MutexImpl
{
protected:
	MutexImpl();
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	CRITICAL_SECTION _cs;
};


typedef MutexImpl FastMutexImpl;


//
// inlines
//
inline void MutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
	}
	catch (...)
	{
	}
}


inline bool MutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}
	catch (...)
	{
	}
}


inline void MutexImpl::unlockImpl()
{
	LeaveCriticalSection(&_cs);
}

#endif