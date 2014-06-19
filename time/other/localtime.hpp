{
	//[ wcsftime for unicode]
	time_t t = std::time(0);
	tm* timeinfo = std::localtime(&t);
	char str[200];
	std::strftime(str, 200, " %b %d %X ", timeinfo);
}


SYSTEMTIME st;
GetSystemTime(&st);

SYSTEMTIME stLocalTime;
SystemTimeToTzSpecificLocalTime(NULL, &st, &stLocalTime);

#include <ctime>
#include <iostream>

int main()
{
    std::time_t result = std::time(NULL);
    std::cout << std::asctime(std::localtime(&result))
              << result << " seconds since the Epoch\n";
}

