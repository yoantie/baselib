
#include <windows.h>
class EventImpl
{
protected:
	EventImpl(bool autoReset);		
	~EventImpl();
	void setImpl();
	void waitImpl();
	bool waitImpl(long milliseconds);
	void resetImpl();
	
private:
	HANDLE _event;
};


//
// inlines
//
inline void EventImpl::setImpl()
{
	if (!SetEvent(_event))
	{
		return;
	}
}


inline void EventImpl::resetImpl()
{
	if (!ResetEvent(_event))
	{
		return;
	}
}
