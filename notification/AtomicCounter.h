#pragma once

class AtomicCounter
{
//	long ImplType;
	
public:
	typedef int ValueType;

	AtomicCounter(void);
	explicit AtomicCounter(ValueType initValue);
	AtomicCounter(const AtomicCounter& counter);
	
	~AtomicCounter(void);
	
	AtomicCounter& operator= (const AtomicCounter& counter);
	AtomicCounter& operator= (ValueType counter);

	ValueType operator++();
	ValueType operator++(int);
	ValueType operator--();
	ValueType operator--(int);

	operator ValueType() const;
	ValueType value() const;

	bool operator !() const;

private:
	long counter_;
};
