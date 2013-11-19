#include "StdAfx.h"
#include "INET_Addr.h"

/*
	Addr Class
*/

void Addr::base_set(int type, int size)
{
	addr_type_ = type;
	addr_size_ = size;
}

void Addr::set_size(int size)
{
	this->addr_size_ = size;
}

void  Addr::set_type(int type)
{
	this->addr_type_ = type;
}

int Addr::get_size(void) const
{
	return this->addr_size_;
}

int Addr::get_type(void) const
{
	return this->addr_type_;
}

/*
	INET_Addr Class
*/
INET_Addr::INET_Addr()
{}

UINT32 INET_Addr::get_ip_address(void) const
{
	return NTOHL (UINT32 (this->inet_addr_.in4_.sin_addr.s_addr));
}

bool INET_Addr::is_loopback(void) const
{
	return ((this->get_ip_address () & 0XFF000000) == (INADDR_LOOPBACK & 0XFF000000));
}

const char* INET_Addr::get_host_name(void) const
{
	static char name[256+1];
	if (this->get_host_name(name, 257) == -1)
		strcpy(name, "<unknown>");
	return name;
}

int INET_Addr::string_to_addr(const char s[], int address_family)
{
	int result;
	char *ip_buf = 0;
	char *ip_addr = 0;

	if ( (ip_buf = strdup(s)) == NULL ) 
		return -1;
	
	ip_addr = ip_buf; //in the end ,wo use free('ip_buf'), attention
	char *port_p = strrchr(ip_addr, ':');
	if (port_p == 0) // Assume it's a port number.
	{
		char *endp = 0;
		long const port = strtol(ip_addr, &endp, 10);
		if (*endp == '\0') // strtol scanned the entire string - all digits
		{
			if (port < 0 || port > 65535)
				result = -1;
			else
				result = this->set((UINT16)port, (UINT32)INADDR_ANY);
		}
		else // port name
			result = this->set(ip_addr, (UINT32)INADDR_ANY);
	}
	else
	{
		*port_p = '\0'; ++port_p; // skip over ':'
		char *endp = 0;
		long port = strtol(port_p, &endp, 10);
		if (*endp == '\0') // strtol scanned the entire string - all digits
		{
			if (port < 0 || port > 65535)
				result = -1;
			else
				result = this->set((UINT16)port, ip_addr, 1, address_family);
		}
		else
			result = this->set(port_p, ip_addr);
	}

	free(ip_buf);
	return result;
}

int INET_Addr::get_host_name(char hostname[], size_t len) const
{
	int result;
	if (len > 1)
	{
		result = this->get_host_name_i(hostname, len);
		if (result < 0)
		{
			if (result == -2)
				result = -1;
			else
				hostname[0] = '\0';
		}
	}
	else
	{
		if (len == 1)
			hostname[0] = '\0';
		result = -1;
	}

	return result;
}

int INET_Addr::get_host_name_i (char hostname[], size_t len) const
{
	if (this->inet_addr_.in4_.sin_addr.s_addr == INADDR_ANY)
	{
		if (gethostname(hostname, len) == -1)
			return -1;
		else
			return 0;
	}
	else
	{
		void* addr = this->ip_addr_pointer();
		int   size = this->ip_addr_size();
		int   type = this->get_type();

		hostent* const hp = gethostbyaddr(static_cast<char*>(addr), size, type);
		if (hp == 0 || hp->h_name == 0)
			return -1;

		if (strlen(hp->h_name) >= len)
		{
			if (len > 0)
			{
				memcpy (hostname, hp->h_name, len - 1);
				hostname[len-1]= '\0';
			}
		}

		strcpy(hostname, hp->h_name);
		return 0;
	}
}

int INET_Addr::set(u_short port_number, UINT32 inet_address, int encode, int map)
{
	this->set_address(reinterpret_cast<const char*>(&inet_address), sizeof inet_address, encode, map);
	this->set_port_number(port_number, encode);

	return 0;
}

int INET_Addr::set(const char port_name[], UINT32 inet_address, const char protocol[])
{
	int const port_number = get_port_number_from_name(port_name, protocol);
	if (port_number == -1)
		return -1;

	return this->set(static_cast<u_short>(port_number), inet_address, 0);
}

int INET_Addr::set(u_short port_number, const char host_name[], int encode, int address_family)
{
	if (host_name == 0)
		return -1;

	memset((void*)&this->inet_addr_, 0, sizeof this->inet_addr_);
	
	address_family = AF_INET;
	this->set_type(address_family);
	this->inet_addr_.in4_.sin_family = static_cast<short>(address_family);
	
	struct in_addr addrv4;
	if (inet_aton(host_name, &addrv4) == 1)
		return this->set(port_number, encode ? NTOHL(addrv4.s_addr) : addrv4.s_addr, encode);
	else
	{
		hostent *hp = gethostbyname(host_name);
		if (hp == 0)
			return -1;
		else {
			memcpy(&addrv4.s_addr, hp->h_addr, hp->h_length);
            return this->set( port_number, 
				encode ? NTOHL(addrv4.s_addr) : addrv4.s_addr, encode);
		}
	}
	
}

int INET_Addr::set(const char port_name[], const char host_name[], const char protocol[])
{
	int const port_number = get_port_number_from_name(port_name, protocol);
	if (port_number == -1)
		return -1;

	int address_family = PF_UNSPEC;
	return this->set(static_cast<u_short>(port_number), host_name, 0, address_family);
}

void INET_Addr::set_port_number(u_short port_number, int encode)
{
	if (encode)
		port_number = HTONS(port_number);
	this->inet_addr_.in4_.sin_port = port_number;
}

int INET_Addr::set_address(const char *ip_addr, int len, int encode, int map)
{
  // This is really intended for IPv4. If the object is IPv4, or the type
  // hasn't been set but it's a 4-byte address, go ahead. If this is an
  // IPv6 object and <encode> is requested, refuse.
	if (encode && len != 4)
	{
		//not supported
		return -1;
	}

	if (len == 4)
	{
		UINT32 ip4 = *reinterpret_cast<const UINT32*>(ip_addr);
		if (encode)
			ip4 = HTONL(ip4);
		if (this->get_type() == AF_INET && map == 0){
			this->inet_addr_.in4_.sin_family = AF_INET;
			this->set_size(sizeof (this->inet_addr_.in4_));
			memcpy(&this->inet_addr_.in4_.sin_addr, &ip4, len);
		}

		return 0;
	}

	//not supported
	return -1;
}

static int get_port_number_from_name(const char port_name[], const char protocol[])
{
	char *endp = 0;
	long port_number = strtol(port_name, &endp, 10);
	if (*endp == '\0')
	{
		if (port_number < 0 || port_number > ACE_MAX_DEFAULT_PORT)
			return -1;

		 u_short n = static_cast<u_short> (port_number);
		 n = HTONS (n);
		 return n;
	} //get server by name ...
	else
	{
		port_number = -1;
		servent *sp = getservbyname(port_name, protocol);
		if (sp != 0)
			port_number = sp->s_port;
	}
	
	return port_number;
}

static int inet_aton(const char *host_name, struct in_addr *addr)
{
	if (host_name == 0 || host_name[0] == '\0')
		host_name = " ";
	unsigned long ip_addr = inet_addr(host_name);
	if (ip_addr == INADDR_NONE && strcmp(host_name, "255.255.255.255") != 0)
		return 0;
	else if (addr == NULL)
		return 0;
	else{
		addr->S_un.S_addr = ip_addr;
		return 1;
	}
}

void* INET_Addr::ip_addr_pointer(void) const
{
	return (void*)&this->inet_addr_.in4_.sin_addr;
}

int INET_Addr::ip_addr_size(void) const
{
	return static_cast<int>(sizeof this->inet_addr_.in4_.sin_addr.s_addr);
}