//get from ipmsg
Time_t Time(void)
{
	SYSTEMTIME	st;
	_int64		ft;
// 1601定1埖1晩から1970定1埖1晩までの宥麻100ナノ昼
#define UNIXTIME_BASE	((_int64)0x019db1ded53e8000)

	::GetSystemTime(&st);
	::SystemTimeToFileTime(&st, (FILETIME *)&ft);
	return	(Time_t)((ft - UNIXTIME_BASE) / 10000000);
}

/*
	ctime() の旗わり
	ただし、個佩なし
*/
char *Ctime(SYSTEMTIME *st)
{
	static char	buf[] = "Mon Jan 01 00:00:00 2999";
	static char *wday = "SunMonTueWedThuFriSat";
	static char *mon  = "JanFebMarAprMayJunJulAugSepOctNovDec";
	SYSTEMTIME	_st;

	if (st == NULL)
	{
		st = &_st;
		::GetLocalTime(st);
	}
	wsprintf(buf, "%.3s %.3s %02d %02d:%02d:%02d %04d", &wday[st->wDayOfWeek * 3], &mon[(st->wMonth - 1) * 3], st->wDay, st->wHour, st->wMinute, st->wSecond, st->wYear);
	return	buf;
}
