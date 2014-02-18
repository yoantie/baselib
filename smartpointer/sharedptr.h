#ifndef _shared_ptr_h__
#define _shared_ptr_h__

#include <windows.h>

namespace ULT {
	class AutoCount
	{
		volatile long m_count;
	public:
		AutoCount() : m_count(1)
		{}

		AutoCount(long count):m_count(count){
		}

		void AddReference()
		{
			::InterlockedIncrement(&m_count);
		}

		void Release()
		{
			::InterlockedDecrement(&m_count);
		}


		long GetCount()
		{
			return m_count;
		}
	};


	template<typename Object>
	class SharedPtr
	{
		AutoCount* m_rc;
		Object* m_ptr;//若用Object, std::swap(m_ptr, ptr->m_ptr); error


		void Release()
		{
			m_rc->Release();

			if ( m_ptr != NULL && m_rc->GetCount() == 0 ) {
				delete m_ptr; 
				m_ptr = NULL;

				delete m_rc;
				m_rc = NULL;
			}
		}

		void AddReference()
		{
			m_rc.AddReference();
		}

	public:
		int GetCount() 
		{
			return m_rc->GetCount();
		}

		Object* get()
		{
			return m_ptr;
		}

		const Object* get() const
		{
			return m_ptr;
		}
		
		operator Object* ()
		{
			return m_ptr;
		}
		
		Object** operator & ()
		{
			return &m_ptr;
		}

		bool operator ! () const
		{
			return m_ptr == NULL;
		}

	public:
		SharedPtr() : m_ptr(NULL), m_rc( new AutoCount )
		{
		}

		SharedPtr(Object* obj):m_ptr(obj), m_rc(new AutoCount)
		{};


		SharedPtr(const SharedPtr & ptr) : m_ptr(ptr->m_ptr), m_rc(ptr->m_rc)
		{
			m_rc->AddReference();
		}


		~SharedPtr()
		{
			Release();
		}

		void Swap( SharedPtr & ptr)
		{
			std::swap(m_ptr, ptr.m_ptr);
			std::swap(m_rc, ptr.m_rc);
		}

		Object* operator->()
		{
			return m_ptr;
		}

		const SharedPtr* operator->() const
		{
			return this;
		}

		SharedPtr& operator=(const SharedPtr & ptr) //Shared_ptr<Object>
		{
			if ( this != &ptr ) {
				SharedPtr tmp(ptr); //去掉const属性,同时AddReference
				Swap(tmp);
			}

			return *this;
		}

		SharedPtr& operator= (Object* obj)
		{
			if (m_ptr != obj) {
				Release();	

				AutoCount* pTmp = new AutoCount;
				m_rc = pTmp;
				m_ptr = obj;
			}

			return *this;
		}
	};

};

#endif