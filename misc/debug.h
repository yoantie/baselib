void Debug(char *fmt,...)
{
	char buf[8192];

	va_list	ap;
	va_start(ap, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	::OutputDebugString(buf);
}

void DebugW(WCHAR *fmt,...)
{
	WCHAR buf[8192];

	va_list	ap;
	va_start(ap, fmt);
	_vsnwprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);
	::OutputDebugStringW(buf);
}

const char *Fmt(char *fmt,...)
{
	static char buf[8192];

	va_list	ap;
	va_start(ap, fmt);
	_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	return	buf;
}

const WCHAR *FmtW(WCHAR *fmt,...)
{
	static WCHAR buf[8192];

	va_list	ap;
	va_start(ap, fmt);
	_vsnwprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	return	buf;
}
