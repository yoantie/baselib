#ifndef _TOKENIZER_H__
#define _TOKENIZER_H__ 

#include <string>
#include <vector>

class StringTokenizer
{
public:
	enum Options
	{
		TOK_IGNORE_EMPTY = 1, /// ignore empty tokens
		TOK_TRIM         = 2  /// remove leading and trailing whitespace from tokens
	};
	
	typedef std::vector<std::wstring>::const_iterator Iterator;
	
	StringTokenizer(const std::wstring& str, std::wstring separators, int options = TOK_IGNORE_EMPTY);
	~StringTokenizer();
	
	Iterator begin() const;
	Iterator end() const;
	
	const std::wstring operator [] (std::size_t index) const;
	std::size_t count() const;
private:
	StringTokenizer(const StringTokenizer&);
	StringTokenizer& operator = (const StringTokenizer&);
	
	std::vector<std::wstring> _tokens;
};

inline StringTokenizer::Iterator StringTokenizer::begin() const
{
	return _tokens.begin();
}

inline StringTokenizer::Iterator StringTokenizer::end() const
{
	return _tokens.end();
}

inline const std::wstring StringTokenizer::operator [] (std::size_t index) const
{
	if (index < _tokens.size()) {
		return _tokens[index];
	}

	return L""; 
}

inline std::size_t StringTokenizer::count() const
{
	return _tokens.size();
}

#endif