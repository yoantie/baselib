#include "StdAfx.h"
#include "TimeVal.h"
#include <time.h>
# include <limits>

const DWORDLONG ACE_Time_Value_t::filetime_to_timval_skew =
ACE_INT64_LITERAL (0x19db1ded53e8000);



ACE_Time_Value_t::ACE_Time_Value_t(void)
{
}

ACE_Time_Value_t::ACE_Time_Value_t(const FILETIME& ft)
{
	set(ft);
}

ACE_Time_Value_t::~ACE_Time_Value_t(void)
{
}

void ACE_Time_Value_t::set(const FILETIME& file_time)
{
	ULARGE_INTEGER _100ns;
	_100ns.LowPart = file_time.dwLowDateTime;
	_100ns.HighPart = file_time.dwHighDateTime;

	_100ns.QuadPart -= ACE_Time_Value_t::filetime_to_timval_skew;

	// Convert 100ns units to seconds;
	this->tv_.tv_sec = (time_t) (_100ns.QuadPart / (10000 * 1000));
	// Convert remainder to microseconds;
	this->tv_.tv_usec = (suseconds_t) ((_100ns.QuadPart % (10000 * 1000)) / 10);

	this->normalize ();
}

ACE_Time_Value_t::operator FILETIME () const
{
  FILETIME file_time;
  // ACE_OS_TRACE ("ACE_Date_Time_t::operator FILETIME");

  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (((DWORDLONG) this->tv_.tv_sec * (10000 * 1000) +
                     this->tv_.tv_usec * 10) +
                     ACE_Time_Value_t::filetime_to_timval_skew);

  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  return file_time;
}


void
ACE_Time_Value_t::normalize (bool saturate)
{
  // // ACE_OS_TRACE ("ACE_Date_Time_t::normalize");
  // From Hans Rohnert...

  if (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS_t)
    {
      /*! \todo This loop needs some optimization.
       */
      if (!saturate) // keep the conditionnal expression outside the while loop to minimize performance cost
        do
          {
            ++this->tv_.tv_sec;
            this->tv_.tv_usec -= ACE_ONE_SECOND_IN_USECS_t;
          }
        while (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS_t);
      else
        do
          if (this->tv_.tv_sec < ACE_Numeric_Limits_t<time_t>::max())
            {
              ++this->tv_.tv_sec;
              this->tv_.tv_usec -= ACE_ONE_SECOND_IN_USECS_t;
            }
          else
            this->tv_.tv_usec = ACE_ONE_SECOND_IN_USECS_t - 1;
        while (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS_t);
    }
  else if (this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS_t)
    {
      /*! \todo This loop needs some optimization.
       */
      if (!saturate)
        do
          {
            --this->tv_.tv_sec;
            this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS_t;
          }
        while (this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS_t);
      else
        do
          if (this->tv_.tv_sec > ACE_Numeric_Limits_t<time_t>::min())
            {
              --this->tv_.tv_sec;
              this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS_t;
            }
          else
            this->tv_.tv_usec = -ACE_ONE_SECOND_IN_USECS_t + 1;
        while (this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS_t);
    }

  if (this->tv_.tv_sec >= 1 && this->tv_.tv_usec < 0)
    {
      --this->tv_.tv_sec;
      this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS_t;
    }
}

 time_t
ACE_Time_Value_t::sec (void) const
{
  // ACE_OS_TRACE ("ACE_Date_Time_t::sec");
  return this->tv_.tv_sec;
}

 suseconds_t
ACE_Time_Value_t::usec (void) const
{
  // ACE_OS_TRACE ("ACE_Date_Time_t::usec");
  return this->tv_.tv_usec;
}

FILETIME GetTimeOfDay()
{
   FILETIME   tfile;
  ::GetSystemTimeAsFileTime (&tfile);
  return tfile;
}

void
ACE_Date_Time_t::update (const ACE_Time_Value_t& timevalue)
{

  time_t time = timevalue.sec ();
  struct tm tm_time;
  //ACE_OS::localtime_r (&time, &tm_time);
  _localtime64_s(&tm_time, &time);

  this->day_ = tm_time.tm_mday;
  this->month_ = tm_time.tm_mon + 1;    // localtime's months are 0-11
  this->year_ = tm_time.tm_year + 1900; // localtime reports years since 1900
  this->hour_ = tm_time.tm_hour;
  this->minute_ = tm_time.tm_min;
  this->second_ = tm_time.tm_sec;
  this->microsec_ = timevalue.usec ();
  this->wday_ = tm_time.tm_wday;
}

ACE_Date_Time_t::ACE_Date_Time_t (void)
{
  this->update ();
}

ACE_Date_Time_t::ACE_Date_Time_t (const ACE_Time_Value_t& timevalue)
{
  this->update (timevalue);
}

void
ACE_Date_Time_t::update (void)
{
  update( GetTimeOfDay() );
}

ACE_Date_Time_t::ACE_Date_Time_t (long day,
                              long month,
                              long year,
                              long hour,
                              long minute,
                              long second,
                              long microsec,
                              long wday)
  : day_ (day),
    month_ (month),
    year_ (year),
    hour_ (hour),
    minute_ (minute),
    second_ (second),
    microsec_ (microsec),
    wday_ (wday)
{
}

 long
ACE_Date_Time_t::day (void) const
{
  return day_;
}

// set day
 void
ACE_Date_Time_t::day (long day)
{
  day_ = day;
}

// get month
 long
ACE_Date_Time_t::month (void) const
{
  return month_;
}

// set month
 void
ACE_Date_Time_t::month (long month)
{
  month_ = month;
}

// get year
 long
ACE_Date_Time_t::year (void) const
{
  return year_;
}

// set year
 void
ACE_Date_Time_t::year (long year)
{
  year_ = year;
}

// get hour
 long
ACE_Date_Time_t::hour (void) const
{
  return hour_;
}

// set hour
 void
ACE_Date_Time_t::hour (long hour)
{
  hour_ = hour;
}

// get minute
 long
ACE_Date_Time_t::minute (void) const
{
  return minute_;
}

// set minute
 void
ACE_Date_Time_t::minute (long minute)
{
  minute_ = minute;
}

// get second
 long
ACE_Date_Time_t::second (void) const
{
  return second_;
}

// set second
 void
ACE_Date_Time_t::second (long second)
{
  second_ = second;
}

// get microsec
 long
ACE_Date_Time_t::microsec (void) const
{
  return microsec_;
}

// set microsec
 void
ACE_Date_Time_t::microsec (long microsec)
{
  microsec_ = microsec;
}

// get wday
 long
ACE_Date_Time_t::weekday (void) const
{
  return wday_;
}

// set wday
 void
ACE_Date_Time_t::weekday (long wday)
{
  wday_ = wday;
}
