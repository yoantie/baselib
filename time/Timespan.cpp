#include "StdAfx.h"
#include "Timespan.h"

const TimeDiff CTimespan::MILLISECONDS = 1000;
const TimeDiff CTimespan::SECONDS      = 1000*CTimespan::MILLISECONDS;
const TimeDiff CTimespan::MINUTES      =   60*CTimespan::SECONDS;
const TimeDiff CTimespan::HOURS        =   60*CTimespan::MINUTES;
const TimeDiff CTimespan::DAYS         =   24*CTimespan::HOURS;

CTimespan::CTimespan(void)
:m_span(0)
{
}

CTimespan::CTimespan(TimeDiff microseconds)
:m_span(microseconds)
{

}

CTimespan::CTimespan(long seconds, long microseconds)
:m_span(seconds * SECONDS + microseconds)
{}

CTimespan::CTimespan(int days, int hours, int minutes, int seconds, int microseconds)
:m_span(days * DAYS + hours * HOURS + minutes * MINUTES + seconds * SECONDS + microseconds)
{}

CTimespan::CTimespan(const CTimespan& timespan)
{
	m_span = timespan.TotalMicroseconds();
}

CTimespan::~CTimespan(void)
{
}

int CTimespan::Days() const
{
	return m_span / DAYS;
}

int CTimespan::Hours() const
{
	return ( m_span / HOURS ) % 24;
}

int CTimespan::TotalHours() const
{
	return m_span / HOURS;
}

int CTimespan::Minutes() const
{
	return ( m_span / MINUTES ) % 60;
}

int CTimespan::TotalMinutes() const
{
	return m_span / MINUTES;
}

int CTimespan::Seconds() const
{
	return ( m_span / SECONDS ) % 60;
}

int CTimespan::TotalSeconds() const
{
	return m_span / SECONDS;
}

int CTimespan::Milllseconds() const
{
	//return ( m_span - TotalSeconds() * SECONDS ) / MILLISECONDS;
	return ( m_span / MILLISECONDS ) % 1000;
}

int CTimespan::TotalMillseconds() const
{
	return m_span / MILLISECONDS;
}

int CTimespan::Microseconds() const
{
	return  ( m_span % 1000 );
}

TimeDiff CTimespan::TotalMicroseconds() const
{
	return m_span;
}

int CTimespan::USeconds() const
{
	return m_span % 1000000;
}