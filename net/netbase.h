std::string GetLocalHost()
{
	char name[32];
	gethostname(name, 32);

	CHAR hostAddress[20];
	struct hostent* pHost;
	pHost = gethostbyname(name);
	if (pHost) {
		sprintf(HostAddress, _T("%d.%d.%d.%d"),
			pHost->h_addr_list[0][0] & 0x00ff,
			pHost->h_addr_list[0][1] & 0x00ff,
			pHost->h_addr_list[0][2] & 0x00ff,
			pHost->h_addr_list[0][3] & 0x00ff );
	}

	return std::string(HostAddress);
}

unsigned long HostToInetAddr(const char* host)
{
	unsigned long ip = inet_addr(host);
	if( ip == INADDR_NONE ) {
		hostent* phost = gethostbyname(host);
		if (phost != NULL) {
			in_addr addr;
			memcpy(&(addr.S_un.S_addr), phost->h_addr_list[0], phost->h_length);
			ip = addr.S_un.S_addr;
		} else {
			return 0;
		}
	}

	return ip;
}
