#include "StdAfx.h"
#include "DateTime.h"

CDateTime::CDateTime(void)
{
	CTimeStamp ts;
	m_utctime = ts.ToUTCTime();
	ComputeDayTime();
}

CDateTime::CDateTime(const CTimespan &span)
{
	CTimeStamp ts;
	m_utctime = ts.ToUTCTime();

	m_day    = span.Days();
	m_hours   = span.Hours();
	m_minutes = span.Minutes();
	m_seconds = span.Seconds();
	m_millseconds = span.Milllseconds();
	m_microseconds = span.Microseconds();
}

CDateTime::~CDateTime(void)
{
}

int CDateTime::Year()
{
	return m_year;	
}

int CDateTime::Month()
{
	return m_month;
}

int CDateTime::Day()
{
	return m_day;
}

int CDateTime::Hour()
{
	return m_hours;
}

int CDateTime::Minute()
{
	return m_minutes;
}

int CDateTime::Second()
{
	return m_seconds;
}

void CDateTime::ComputeDayTime()
{
	CTimespan span(m_utctime/10);

	m_hours   = span.Hours();
	m_minutes = span.Minutes();
	m_seconds = span.Seconds();
	m_millseconds = span.Milllseconds();
	m_microseconds = span.Microseconds();
}