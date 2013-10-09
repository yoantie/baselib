#pragma once

#include <string>
#include <cassert>
class CNumberFormat
{
public:
	CNumberFormat(void);
	~CNumberFormat(void);

public:
	static void Append(std::string& str, int value);
	static void Append(std::string& str, int value, int width);
	static void Append0(std::string& str, int value, int width);

	static void Append(std::string& str, unsigned value);
	static void Append(std::string& str, unsigned value, int width);
	static void Append0(std::string& str, unsigned value, int width);

	static void Append(std::string& str, long value);
	static void Append(std::string& str, long value, int width);
	static void Append0(std::string& str, long value, int width);

	static void AppendHex(std::string& str, long value);
	static void AppendHex(std::string& str, long value, int width);

	static void Append(std::string& str, __int64 value);
	static void Append0(std::string& str, __int64 value, int width);

	static void AppendHex(std::string& str, __int64 value);
	static void AppendHex(std::string& str, __int64 value, int width);

	static void Append(std::string& str, float value);
	static void Append(std::string& str, double value);
	static void Append(std::string& str, double value, int precision);
	static void Append(std::string& str, double value, int width, int precision);
	static void Append(std::string& str, const void* ptr);

//	std::string FormatHex(int value); 
//	std::string FormatHex(int value, int width);
	
private:
	void AppendHex(std::string & str, unsigned value);
	void AppendHex(std::string & str, unsigned value, int width);
};
