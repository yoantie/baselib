{//C++ ����
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
GetSystemTime��õ�ʱ���ǻ���ͨ��Э��ʱ(UTC, Universal Time Coordinated)��
GetLocalTime��õ�ʱ���ǻ��ڱ���ʱ��ġ�
��GetLocalTime��õı���������ʱ��������8ʱ������GetSystemTime��ȡʱ����ʱ����0ʱ��ע������������������
*/
//Windows API
SYSTEMTIME st;
GetSystemTime(&st);

SYSTEMTIME stLocalTime;
SystemTimeToTzSpecificLocalTime(NULL, &st, &stLocalTime);
