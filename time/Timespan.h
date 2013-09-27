#pragma once

#include "timestamp.h"

class CTimespan
{ 
public:
	CTimespan(void);
	CTimespan(TimeDiff microseconds);
	CTimespan(long seconds, long microseconds);
	CTimespan(int days, int hours, int minutes, int seconds, int microseconds);
	CTimespan(const CTimespan& timespan);

	~CTimespan(void);

	int Days() const;
	int Hours() const;
	int TotalHours() const;
	int Minutes() const;
	int TotalMinutes() const;
	int Seconds() const;
	int TotalSeconds() const;
	int Milllseconds() const;
	int TotalMillseconds() const;
	int Microseconds() const;
	TimeDiff TotalMicroseconds() const;
	int USeconds() const;

	static const TimeDiff MILLISECONDS; /// The number of microseconds in a millisecond.
	static const TimeDiff SECONDS;      /// The number of microseconds in a second.
	static const TimeDiff MINUTES;      /// The number of microseconds in a minute.
	static const TimeDiff HOURS;        /// The number of microseconds in a hour.
	static const TimeDiff DAYS;         /// The number of microseconds in a day.

private:
	TimeDiff m_span;
};
