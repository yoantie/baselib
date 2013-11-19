#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef unsigned short        UINT16;
typedef unsigned int          UINT32;

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

#define ACE_MAX_DEFAULT_PORT 65535

#define SWAP_LONG(L) ((SWAP_WORD ((L) & 0xFFFF) << 16) \
            | SWAP_WORD(((L) >> 16) & 0xFFFF))
#define SWAP_WORD(L) ((((L) & 0x00FF) << 8) | (((L) & 0xFF00) >> 8))

//if define little end
#define HTONL(X) SWAP_LONG (X)
#define NTOHL(X) SWAP_LONG (X)

#define HTONS(x) SWAP_WORD(x)
#define NTOHS(x) SWAP_WORD(x)

static int get_port_number_from_name(const char port_name[], const char protocol[]);
static int inet_aton(const char *host_name, struct in_addr *addr);

class Addr
{
public:
	void base_set(int type, int size);
	void set_size(int size);
	void set_type(int type);

	int get_size(void) const;
	int get_type(void) const;

protected:
	int addr_type_;
	int addr_size_;
};

class INET_Addr : public Addr
{
public:
	INET_Addr();


public:
	UINT32 get_ip_address (void) const;
	bool is_loopback (void) const;

	const char* get_host_name(void) const;
	int string_to_addr(const char s[], int address_familiy);

	int set(u_short port_number, UINT32 inet_address = INADDR_ANY, int encode = 1, int map = 0);
	int set(const char port_name[], UINT32 inet_address, const char protocol[] = "tcp");
	int set(u_short port_number, const char host_name[], int encode = 1, int address_family = AF_UNSPEC);
	int set(const char port_name[], const char host_name[], const char protocol[] = "tcp");

private:
	int  get_host_name(char hostname[], size_t len) const;
	int  get_host_name_i (char hostname[], size_t len) const;

	void set_port_number(u_short port_number, int encode);
	int  set_address(const char *ip_addr, int len, int encode = 1, int map = 0);
	
	u_short get_port_number(void) const;
	
	void* ip_addr_pointer(void) const;
	int ip_addr_size(void) const;

private:
	union
	{
		sockaddr_in in4_;
	 //for the compatibiliy of ip6   
//#if defined(HAS_IPV6)    
//	sockaddr_in6 in6_;
//#endif 
	}inet_addr_;
};
