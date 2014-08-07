#include "stdafx.h"
#include "MutexImpl.h"

MutexImpl::MutexImpl()
{
	// the fct has a boolean return value under WInnNt/2000/XP but not on Win98
	// the return only checks if the input address of &_cs was valid, so it is safe to omit it
	InitializeCriticalSectionAndSpinCount(&_cs, 4000);
}


MutexImpl::~MutexImpl()
{
	DeleteCriticalSection(&_cs);
}


bool MutexImpl::tryLockImpl(long milliseconds)
{
	if (TryEnterCriticalSection(&_cs) == TRUE)
		return true;

	return false;
}
