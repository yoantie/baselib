//for this convertion, boost::lexcical_cast may not handle well

std::string dec2hex(int i, int width)
{
	std::stringstream ioss;
	std::string s_temp;
	ioss << std::hex << i;
	ioss >> s_temp;
	std::string s(width - s_temp.size(), '0');
	std::string result;
	result = "0x" + s + s_temp;
	return result;
}

int hex2dec(const std::string& value)
{
	int result;
	std::stringstream ioss(value);
	ioss >> std::hex >> result;
	return result;
}
