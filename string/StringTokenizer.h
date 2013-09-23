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
//�ڵ�����������ʱ��Ҫ��֤���������Ķ����ñ�����"��"����Ҳ����˵���������Ķ���Ҫ��ͷ�ļ��У�
����ͨ���ĺ������岻һ�����������ϰ�߽������������CPP�ļ��У���������ͷ�ļ������һ�㣬��������:
//SomeInline.h��
#ifndef SOMEINLINE_H
#define SOMEINLINE_H
inline Type Example(void);
//........��������������
#include��SomeInlie.cpp�� //Դ�ļ���׺�������������
#endif
//SomeInline.cpp��
#include"SomeInline.h"
Type Example(void)
{
//..........
}
//...............���������Ķ���
���Ϸ�����ͨ�á���Ч�ģ��ɷ���ʹ�ã����ص�����ͷ�ļ�����CPP�ļ��ᵼ�±������
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
