#pragma once
#include <windows.h>
//this is the most pratical one@
#undef min
#undef max

template <typename T>
struct ACE_Numeric_Limits_t
{
  static T min (void) { return std::numeric_limits<T>::min (); }
  static T max (void) { return std::numeric_limits<T>::max (); }
};


#  define ACE_UINT64_LITERAL(n) n ## ui64
#  define ACE_INT64_LITERAL(n) n ## i64

typedef long suseconds_t;
suseconds_t const ACE_ONE_SECOND_IN_USECS_t = 1000000;


class ACE_Time_Value_t
{
	const static DWORDLONG filetime_to_timval_skew;
public:
	ACE_Time_Value_t(void);
	~ACE_Time_Value_t(void);

	ACE_Time_Value_t(const FILETIME& ft);

	void set(const FILETIME& file_time);
	operator FILETIME () const;
	void normalize (bool saturate = false);
	time_t sec (void) const;
	suseconds_t usec(void) const;
private:
	timeval tv_;
};

FILETIME GetTimeOfDatay();

class ACE_Date_Time_t
{
public:
  /// Constructor initializes current time/date info.
  ACE_Date_Time_t (void);

  /// Constructor initializes with the given ACE_Time_Value
  explicit ACE_Date_Time_t (const ACE_Time_Value_t& timevalue);

  /// Constructor with init values, no check for validy
  /// Set/get portions of ACE_Date_Time_t, no check for validity.
  ACE_Date_Time_t (long day,
                 long month = 0,
                 long year = 0,
                 long hour = 0,
                 long minute = 0,
                 long second = 0,
                 long microsec = 0,
                 long wday = 0);

  /// Update to the current time/date.
  void update (void);

  /// Update to the given ACE_Time_Value
  void update (const ACE_Time_Value_t& timevalue);

  /// Get day.
  long day (void) const;

  /// Set day.
  void day (long day);

  /// Get month.
  long month (void) const;

  /// Set month.
  void month (long month);

  /// Get year.
  long year (void) const;

  /// Set year.
  void year (long year);

  /// Get hour.
  long hour (void) const;

  /// Set hour.
  void hour (long hour);

  /// Get minute.
  long minute (void) const;

  /// Set minute.
  void minute (long minute);

  /// Get second.
  long second (void) const;

  /// Set second.
  void second (long second);

  /// Get microsec.
  long microsec (void) const;

  /// Set microsec.
  void microsec (long microsec);

  /// Get weekday.
  long weekday (void) const;

  /// Set weekday.
  void weekday (long wday);

private:
  long day_;
  long month_;
  long year_;
  long hour_;
  long minute_;
  long second_;
  long microsec_;
  long wday_;
};
