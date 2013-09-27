#include "StdAfx.h"
#include "TimeStamp.h"

CTimeStamp::CTimeStamp(void)
{
	Update();
}

CTimeStamp::CTimeStamp(TimeVal tv)
{
	m_ts = tv;
}

CTimeStamp::~CTimeStamp(void)
{
}

CTimeStamp CTimeStamp::FromUTCTime(UTCTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return CTimeStamp(val);
}

CTimeStamp CTimeStamp::FromEpochTime(std::time_t val)
{
	return CTimeStamp( val * 1000000 );
}

std::time_t CTimeStamp::ToEpochTime() const
{
	return  std::time_t(m_ts / 1000000);
}

TimeVal CTimeStamp::ToEpochMicros()
{
	return m_ts;
}

UTCTimeVal CTimeStamp::ToUTCTime()
{
	return m_ts * 10 +  (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

void CTimeStamp::Update()
{
	/*��1601��1��1�տ�ʼ��100����Ϊ��λ��ʱ�䡣*/
	FILETIME ft;

	//�Ȼ�ȡϵͳʱ�䣬Ȼ��תΪFILETIME��ʽ��ϵͳʱ�伴��UTC
	::GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch;			//��1970.1.1����
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE; 

	ULARGE_INTEGER ts;
	ts.LowPart = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;

	m_ts = ts.QuadPart / 10;
}

CTimeStamp CTimeStamp::FromFileTimeNP(unsigned int ftlow, unsigned int fthigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;
	
	ULARGE_INTEGER ts;
	ts.LowPart  = ftlow;
	ts.HighPart = fthigh;
	ts.QuadPart -= epoch.QuadPart;

	return CTimeStamp(ts.QuadPart/10);
}

void CTimeStamp::ToFileTimeNP(unsigned int& ftlow, unsigned int& fthigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.QuadPart  = m_ts*10;
	ts.QuadPart += epoch.QuadPart;
	ftlow  = ts.LowPart;
	fthigh = ts.HighPart;
}
