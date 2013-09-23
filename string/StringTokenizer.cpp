#include "StdAfx.h"
#include "StringTokenizer.h"

StringTokenizer::StringTokenizer(const std::string& str, const std::string& separators, int options)
{
	std::string::const_iterator it1 = str.begin();
	std::string::const_iterator it2;
	std::string::const_iterator it3;
	std::string::const_iterator end = str.end();
	
	while (it1 != end)
	{
		if (options & TOK_TRIM)
		{
			while (it1 != end && *it1 == ' ') ++it1;
		}
		it2 = it1;
		while (it2 != end && separators.find(*it2) == std::string::npos) ++it2;
		it3 = it2;
		if (it3 != it1 && (options & TOK_TRIM))
		{//处理("a  ,sdf", ",", 2);这种"a"后面还有空格的字符序列
			--it3;
			while (it3 != it1 && *it3 == ' ') --it3;
			if (*it3 != ' ' ) ++it3;
		}
		if (options & TOK_IGNORE_EMPTY)
		{
			if (it3 != it1)
				_tokens.push_back(std::string(it1, it3));
		}
		else
		{
			_tokens.push_back(std::string(it1, it3));
		}
		it1 = it2;
		if (it1 != end) ++it1;
	}
}


StringTokenizer::~StringTokenizer()
{
}


