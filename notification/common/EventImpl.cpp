#include "stdafx.h"
#include "EventImpl.h"


EventImpl::EventImpl(bool autoReset)
{
	_event = CreateEventW(NULL, autoReset ? FALSE : TRUE, FALSE, NULL);
	if (!_event)
		return;
}


EventImpl::~EventImpl()
{
	CloseHandle(_event);
}


void EventImpl::waitImpl()
{
	switch (WaitForSingleObject(_event, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		return;
	}
}


bool EventImpl::waitImpl(long milliseconds)
{
	switch (WaitForSingleObject(_event, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		return false;		
	}
}
