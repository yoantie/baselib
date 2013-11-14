static char  *hexstr   =  "0123456789abcdef";
static WCHAR *hexstr_w = L"0123456789abcdef";

inline u_char hexchar2char(u_char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	if (ch >= 'a' && ch <= 'z')
		return ch - 'a' + 10;
	if (ch >= 'A' && ch <= 'Z')
		return ch - 'A' + 10;
	return 0xff;
}

BOOL hexstr2bin(const char *buf, BYTE *bindata, int maxlen, int *len)
{
	for (*len=0; buf[0] && buf[1] && *len < maxlen; buf+=2, (*len)++)
	{
		u_char c1 = hexchar2char(buf[0]);
		u_char c2 = hexchar2char(buf[1]);
		if (c1 == 0xff || c2 == 0xff) break;
		bindata[*len] = (c1 << 4) | c2;
	}
	return	TRUE;
}

int bin2hexstr(const BYTE *bindata, int len, char *buf)
{
	for (const BYTE *end=bindata+len; bindata < end; bindata++)
	{
		*buf++ = hexstr[*bindata >> 4];
		*buf++ = hexstr[*bindata & 0x0f];
	}
	*buf = 0;
	return	len * 2;
}

_int64 hex2ll(char *buf)
{
	_int64	ret = 0;

	for ( ; *buf; buf++)
	{
		if (*buf >= '0' && *buf <= '9')
			ret = (ret << 4) | (*buf - '0');
		else if (toupper(*buf) >= 'A' && toupper(*buf) <= 'F')
			ret = (ret << 4) | (toupper(*buf) - 'A' + 10);
		else continue;
	}
	return	ret;
}

void rev_order(BYTE *data, int size)
{
	BYTE	*d1 = data;
	BYTE	*d2 = data + size - 1;

	for (BYTE *end = d1 + (size/2); d1 < end; ) {
		BYTE	sv = *d1;
		*d1++ = *d2;
		*d2-- = sv;
	}
}

void rev_order(const BYTE *src, BYTE *dst, int size)
{
	dst = dst + size - 1;

	for (const BYTE *end = src + size; src < end; ) {
		*dst-- = *src++;
	}
}
