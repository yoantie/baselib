#include "StdAfx.h"
#include "KTime.h"

CKTime::CKTime(void):
 m_year(0),
 m_month(0),
 m_day(0),
 m_wday(0),
 m_hour(0),
 m_min(0),
 m_sec(0)
{
	::time(&this->m_time);
}

 CKTime::CKTime(time_t time):
  m_year(0),
  m_month(0),
  m_day(0),
  m_wday(0),
  m_hour(0),
  m_min(0),
  m_sec(0)
{
	this->m_time = time;
}

CKTime::CKTime(struct tm* timeptr)
{
	SetTime( timeptr );
}

CKTime::~CKTime(void)
{
	m_time = 0;
}

void CKTime::SetTime(time_t time)
{
	this->m_time = time;
}

void CKTime::SetTime(struct tm *timeptr)
{
	if ( timeptr )
		this->m_time = ::mktime(timeptr);
}

std::string CKTime::Date()
{
	return ::ctime(&m_time);
}

//std::string CKTime::GMDate()
//{
//	tm* tmptr = ::gmtime(&m_time);
//	if ( tmptr ) {
//		//从1900年开始算起的
//		char buffer[128] = { 0 };
//		sprintf(buffer, "%02d:%02d:%02d--%02d:%02d:%02d",
//			tmptr->tm_year - 100, tmptr->tm_mon + 1, tmptr->tm_mday,
//			tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
//		return buffer;
//	}
//
//	return "";
//}
//
//std::string CKTime::LocalDate()
//{
//	tm* tmptr = ::localtime(&m_time);
//	if ( tmptr ) {
//		char buffer[128] = { 0 };
//		sprintf(buffer, "%02d:%02d:%02d--%02d:%02d:%02d",
//			tmptr->tm_year - 100, tmptr->tm_mon + 1, tmptr->tm_mday,
//			tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
//		return buffer;
//	}
//
//	return "";
//}

void CKTime::GMDate()
{
	tm* tmptr = ::gmtime(&m_time);
	if ( tmptr ) {
		m_year  = tmptr->tm_yday - 100;
		m_month = tmptr->tm_mon + 1;
		m_day   = tmptr->tm_yday;
		m_wday  = tmptr->tm_mday;
		m_hour  = tmptr->tm_hour;
		m_min   = tmptr->tm_min;
		m_sec   = tmptr->tm_sec;
	}
}

void CKTime::LocalDate()
{
	tm* tmptr = ::localtime(&m_time);
	if ( tmptr ) {
		m_year  = tmptr->tm_yday - 100;
		m_month = tmptr->tm_mon + 1;
		m_day   = tmptr->tm_yday;
		m_wday  = tmptr->tm_mday;
		m_hour  = tmptr->tm_hour;
		m_min   = tmptr->tm_min;
		m_sec   = tmptr->tm_sec;
	}
}

int CKTime::GetYear()
{
	return m_year;
}

int CKTime::GetMonth()
{
	return m_month;
}

int CKTime::GetDay()
{
	return m_day;
}

int CKTime::GetWeekDay()
{
	return m_wday;
}

int CKTime::GetHour()
{
	return m_hour;
}

int CKTime::GetMinute()
{
	return m_min;
}

int CKTime::GetSecond()
{
	return m_sec;
}

double CKTime::operator-(const CKTime& time)
{
	return ::difftime(m_time, time.m_time);
}

CKTime& CKTime::operator=(const CKTime& time)
{
	m_time = time.m_time;

	return *this;
}