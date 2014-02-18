#ifndef _COM_PTR_H__
#define _COM_PTR_H__


#include <cassert>

namespace ULT {
template<class T>
class CComPtr
{
	T* p;
public:
	CComPtr() throw()
	{
		p = NULL;
	}

	CComPtr(T* lp) throw()
	{
		p = lp;
		if (p != NULL)
			p->AddRef();
	}

public:
	~CComPtr() throw()
	{
		if (p)
			p->Release();
	}

	operator T*() const throw()
	{
		return p;
	}

	T* operator->() const throw()
	{
		return p;
	}

	T& operator *() const throw()
	{
		assert(p);
		return *p;
	}

	T** operator&() throw()
	{
		return &p;
	}

	void Release() throw()
	{
		T* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
	}

	void Attach(T* p2) throw()
	{
		if(p)
			p->Release();
		p = p2;
	}

	T* Detach() throw()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
};

};
#endif