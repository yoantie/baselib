#pragma once

#include "typedef.h"
#include <ctime>
#include <windows.h>


/*
Timestamps are UTC (Coordinated Universal Time)
based and thus independent of the timezone
in effect on the system.
*/
class CTimeStamp
{
public:
	CTimeStamp(void);
	CTimeStamp(TimeVal tv);
	~CTimeStamp(void);
	
	static CTimeStamp FromUTCTime(UTCTimeVal val);
	static CTimeStamp FromEpochTime(std::time_t val);

	//Epoch指的是一个特定的时间：1970-01-01 00:00:00 UTC
	std::time_t ToEpochTime();

	/*
	Returns the timestamp expressed in time_t.
	time_t base time is midnight, January 1, 1970.
	Resolution is one second.
	*/
	TimeVal ToEpochMicros();

	/*
	Returns the timestamp expressed in UTC-based
	time. UTC base time is midnight, October 15, 1582.
	Resolution is 100 nanoseconds.
	*/
	UTCTimeVal ToUTCTime();

private:
	void Update();

private:
	TimeVal m_ts;//in microsecond;
};
