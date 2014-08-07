#include "StdAfx.h"
#include "AtomicCounter.h"
#include <windows.h>

AtomicCounter::AtomicCounter(void)
:counter_(0)
{
}

AtomicCounter::AtomicCounter(ValueType initValue)
:counter_(initValue)
{
}

AtomicCounter::AtomicCounter(const AtomicCounter& counter)
:counter_(counter.value())
{
}

AtomicCounter& AtomicCounter::operator= (const AtomicCounter& counter)
{
	::InterlockedExchange(&counter_, counter.value());
	return *this;
}

AtomicCounter& AtomicCounter::operator= (ValueType counter)
{
	::InterlockedExchange(&counter_, counter);
	return *this;
}

AtomicCounter::~AtomicCounter(void)
{
}

AtomicCounter::ValueType AtomicCounter::operator++()
{
	return ::InterlockedIncrement(&counter_);
}

AtomicCounter::ValueType AtomicCounter::operator++(int)
{
	ValueType result = ::InterlockedIncrement(&counter_);
	return --result;
}

AtomicCounter::ValueType AtomicCounter::operator--()
{
	return ::InterlockedDecrement(&counter_);
}

AtomicCounter::ValueType AtomicCounter::operator--(int)
{
	ValueType result = ::InterlockedDecrement(&counter_);
	return ++result;
}

AtomicCounter::operator AtomicCounter::ValueType() const
{
	return counter_;
}

AtomicCounter::ValueType AtomicCounter::value() const
{
	return counter_;
}

bool AtomicCounter::operator !() const
{
	return counter_ == 0;
}