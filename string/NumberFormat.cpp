#include "StdAfx.h"
#include "NumberFormat.h"
#include <sstream>
#include <iomanip>
#include <iosfwd>

CNumberFormat::CNumberFormat(void)
{
}

CNumberFormat::~CNumberFormat(void)
{
}

void CNumberFormat::Append(std::string &str, int value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%d", value);
	str.append(buffer);
}

void CNumberFormat::Append(std::string& str, int value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%*d", width, value);
	str.append(buffer);
}

void CNumberFormat::Append0(std::string& str, int value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*d", width, value);
}

void CNumberFormat::Append(std::string& str, unsigned value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%u", value);
	str.append(buffer);
}

void CNumberFormat::Append(std::string& str, unsigned value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%*u", width, value);
	str.append(buffer);
}

void CNumberFormat::Append0(std::string& str, unsigned value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*u", width, value);
	str.append(buffer);
}

void CNumberFormat::Append(std::string& str, long value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%ld", value);
	str.append(buffer);
}

void CNumberFormat::Append(std::string& str, long value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%*ld", width, value);
	str.append(buffer);
}

void CNumberFormat::Append0(std::string& str, long value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*ld", width, value);
	str.append(buffer);
}

void CNumberFormat::AppendHex(std::string& str, long value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%lX", value);
	str.append(buffer);
}

void CNumberFormat::AppendHex(std::string& str, long value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*lX", width, value); 
}

void CNumberFormat::Append(std::string& str, __int64 value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%I64d", value);
	str.append(buffer);
}

void CNumberFormat::Append0(std::string& str, __int64 value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*I64d", width, value);
	str.append(buffer);
}

void CNumberFormat::AppendHex(std::string& str, __int64 value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%I64X", value);
	str.append(buffer);
}

void CNumberFormat::AppendHex(std::string &str, __int64 value, int width)
{
	assert(width > 0 && width < 64);
	char buffer[64] = { 0 };
	sprintf(buffer, "%0*I64X", width, value);
	str.append(buffer);
}

void CNumberFormat::Append(std::string &str, float value)
{
	//精度不确定
	char buffer[64] = { 0 };
	std::stringstream stream;
	stream << std::setprecision(8) << value;
	stream >> buffer;
	str.append(buffer);
}

void CNumberFormat::Append(std::string &str, double value)
{
	//精度不确定
	char buffer[64] = { 0 };
	std::stringstream stream;
	stream << std::setprecision(16) << value;
	stream >> buffer;
	str.append(buffer);
}
void CNumberFormat::Append(std::string &str, double value, int precision)
{
	char buffer[64] = { 0 };
	std::stringstream stream;
	stream << std::setprecision(precision) << value;
	stream >> buffer;
	str.append(buffer);
}

void CNumberFormat::Append(std::string &str, double value, int width, int precision)
{
	//std::fixed --> 将小数点固定为6位，若设置了std::setprecision,则以该函数为准
	char buffer[64] = { 0 };
	std::stringstream stream;
	stream << std::fixed << std::showpoint << std::setw(width) << std::setprecision(precision) << value;
	stream >> buffer;
	str.append(buffer);
}

void CNumberFormat::Append(std::string& str, const void* ptr)
{
	char buffer[24] = { 0 };
	sprintf(buffer, "%08lX", (unsigned long)ptr);
	str.append(buffer);
}

//std::string CNumberFormat::FormatHex(int value)
//{
//	std::string result;
//	AppendHex(result, value);
//	return result;
//}
//
//std::string CNumberFormat::FormatHex(int value, int width)
//{
//	std::string result;
//	AppendHex(result, value, width);
//	return result;
//}

void CNumberFormat::AppendHex(std::string & str, unsigned value)
{
	char buffer[64] = { 0 };
	sprintf(buffer, "%X",  value);
	str.append(buffer);
}

void CNumberFormat::AppendHex(std::string & str, unsigned value, int width)
{
	assert(width > 0 && width < 64);

	char buffer[64] = { 0 };
	sprintf(buffer, "%0*X", width, value);
	str.append(buffer);
}

