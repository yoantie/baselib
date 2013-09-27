#pragma once

#include <string>
#include <cassert>
class CNumberFormat
{
public:
	CNumberFormat(void);
	~CNumberFormat(void);

public:
	void Append(std::string& str, int value);
	void Append(std::string& str, int value, int width);
	void Append0(std::string& str, int value, int width);

	void Append(std::string& str, unsigned value);
	void Append(std::string& str, unsigned value, int width);
	void Append0(std::string& str, unsigned value, int width);

	void Append(std::string& str, long value);
	void Append(std::string& str, long value, int width);
	void Append0(std::string& str, long value, int width);

	void AppendHex(std::string& str, long value);
	void AppendHex(std::string& str, long value, int width);

	void Append(std::string& str, __int64 value);
	void Append0(std::string& str, __int64 value, int width);

	void AppendHex(std::string& str, __int64 value);
	void AppendHex(std::string& str, __int64 value, int width);

	void Append(std::string& str, float value);
	void Append(std::string& str, double value);
	void Append(std::string& str, double value, int precision);
	void Append(std::string& str, double value, int width, int precision);
	void Append(std::string& str, const void* ptr);

//	std::string FormatHex(int value); 
//	std::string FormatHex(int value, int width);
	
private:
	void AppendHex(std::string & str, unsigned value);
	void AppendHex(std::string & str, unsigned value, int width);
};
