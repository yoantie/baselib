int strncmpi(const char *str1, const char *str2, size_t num)
{
	for (size_t cnt=0; cnt < num; cnt++)
	{
		char	c1 = toupper(str1[cnt]), c2 = toupper(str2[cnt]);

		if (c1 == c2)
		{
			if (c1)
				continue;
			else
				return	0;
		}
		if (c1 > c2)
			return	1;
		else
			return	-1;
	}
	return	0;
}
