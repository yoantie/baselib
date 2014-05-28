SYSTEMTIME st;
GetSystemTime(&st);

SYSTEMTIME stLocalTime;
SystemTimeToTzSpecificLocalTime(NULL, &st, &stLocalTime);
