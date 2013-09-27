#pragma once

#include "Timestamp.h"
#include "Timespan.h"

//�����գ����������й���ë�ã�
class CDateTime
{
public:
	CDateTime(void);
	CDateTime(const CTimespan& span);
	~CDateTime(void);

	int Year();
	int Month();
	int Day();
	int Hour();
	int Minute();
	int Second();

private:
	bool IsLeapYear();
	void ComputeDayTime();

private:
	UTCTimeVal m_utctime;

	int m_year;
	int m_month;
	int m_day;
	int m_hours;
	int m_minutes;
	int m_seconds;
	int m_millseconds;
	int m_microseconds;
};
