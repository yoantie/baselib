{//C++ 函数
	//[ wcsftime for unicode]
	time_t t = std::time(0);

	tm* timeinfo = std::localtime(&t); //to local time
	char str[200];
	std::strftime(str, 200, " %b %d %X ", timeinfo);

	timeinfo = std::gmtime(&t); // UTC

	std::cout << std::ctime(&t) << std::endl;
	//Converts given time since epoch to a calendar local time and then to a textual representation,
	//as if by calling std::asctime(std::localtime(time))
	//output:
	//Tue Dec 27 17:21:29 2011
}

/*
GetSystemTime获得的时间是基于通用协调时(UTC, Universal Time Coordinated)。
GetLocalTime获得的时间是基于本地时间的。
如GetLocalTime获得的本地主机的时间是上午8时，当用GetSystemTime获取时，该时间是0时，注意这两个函数的区别
*/
//Windows API
SYSTEMTIME st;
GetSystemTime(&st);

SYSTEMTIME stLocalTime;
SystemTimeToTzSpecificLocalTime(NULL, &st, &stLocalTime);
