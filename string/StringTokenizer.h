#pragma once
#include <string>
#include <vector>

class  StringTokenizer
	/// A simple tokenizer that splits a string into
	/// tokens, which are separated by separator characters.
	/// An iterator is used to iterate over all tokens.
{
public:
	enum Options
	{
		TOK_IGNORE_EMPTY = 1, /// ignore empty tokens
		TOK_TRIM         = 2  /// remove leading and trailing whitespace from tokens
	};
	
	typedef std::vector<std::string>::const_iterator Iterator;
	
	StringTokenizer(const std::string& str, const std::string& separators, int options = 0);
	~StringTokenizer();
	
	Iterator begin() const;
	Iterator end() const;
	
	const std::string& operator [] (std::size_t index) const;
	std::size_t count() const;

private:
	StringTokenizer(const StringTokenizer&);
	StringTokenizer& operator = (const StringTokenizer&);
	
	std::vector<std::string> _tokens;
};

/*
//在调用内联函数时，要保证内联函数的定义让编译器"看"到，也就是说内联函数的定义要在头文件中，
这与通常的函数定义不一样。但如果你习惯将函数定义放在CPP文件中，或者想让头文件更简洁一点，可这样做:
//SomeInline.h中
#ifndef SOMEINLINE_H
#define SOMEINLINE_H
inline Type Example(void);
//........其他函数的声明
#include“SomeInlie.cpp” //源文件后缀名随编译器而定
#endif
//SomeInline.cpp中
#include"SomeInline.h"
Type Example(void)
{
//..........
}
//...............其他函数的定义
以上方法是通用、有效的，可放心使用，不必担心在头文件包含CPP文件会导致编译错误
*/
inline StringTokenizer::Iterator StringTokenizer::begin() const
{
	return _tokens.begin();
}


inline StringTokenizer::Iterator StringTokenizer::end() const
{
	return _tokens.end();
}


inline const std::string& StringTokenizer::operator [] (std::size_t index) const
{
	if ( index >= _tokens.size() ) throw;
	return _tokens[index];
}


inline std::size_t StringTokenizer::count() const
{
	return _tokens.size();
}
