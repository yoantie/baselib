#include "StdAfx.h"
#include "tokenizer.h"

StringTokenizer::StringTokenizer(const std::wstring& str, std::wstring separators, int options)
{
	std::wstring::const_iterator it1 = str.begin();
	std::wstring::const_iterator it2;
	std::wstring::const_iterator it3;
	std::wstring::const_iterator end = str.end();
	
	while (it1 != end)
	{
		if (options & TOK_TRIM)
		{
			while (it1 != end && (*it1) == ' ') ++it1;
		}
		it2 = it1;
		while (it2 != end && separators.find(*it2) == std::wstring::npos) ++it2;
		it3 = it2;
		if (it3 != it1 && (options & TOK_TRIM))
		{
			--it3;
			while (it3 != it1 && (*it3) == ' ') --it3;
			if ((*it3) != ' ') ++it3;
		}
		if (options & TOK_IGNORE_EMPTY)
		{
			if (it3 != it1)
				_tokens.push_back(std::wstring(it1, it3));
		}
		else
		{
			_tokens.push_back(std::wstring(it1, it3));
		}
		it1 = it2;
		if (it1 != end) ++it1;
	}
}

StringTokenizer::~StringTokenizer()
{}