	time_t t = std::time(0);
	tm* timeinfo = std::localtime(&t);
	char str[200];
	std::strftime(str, 200, " %b %d %X ", timeinfo);
	