#pragma once

#include "AtomicCounter.h"

class RefCountedObject
{
public:
	RefCountedObject(void);
	void duplicate() const;
	void release() const;
	int referenceCount() const;

protected:
	virtual ~RefCountedObject(void);

private:
	RefCountedObject(const RefCountedObject&);
	RefCountedObject& operator= (const RefCountedObject&);

	mutable AtomicCounter _counter;
};

inline int RefCountedObject::referenceCount() const
{
	return _counter.value();
}

inline void RefCountedObject::duplicate() const
{
	++_counter;
}

inline void RefCountedObject::release() const
{
	if (--_counter == 0)
		delete this;
}