#ifndef _file_check_h__
#define _file_check_h__

#include "MD5.h"
#include "conv.h"

class CFileMD5
{
public:
	static std::wstring GetMD5(const std::wstring& file);
};

#endif

inline std::wstring CFileMD5::GetMD5(const std::wstring& file)
{
	std::wstring tMd5;
	
	HANDLE hFile = ::CreateFile(file.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return tMd5;

	char buffer[512] = { 0 };
	MD5 md5;
	md5.reset();

	DWORD dwBytes;
	while (true) {
		ReadFile(hFile, buffer, sizeof(buffer), &dwBytes, NULL);
		if (dwBytes > 0) {
			md5.update(buffer, dwBytes);
		} else {
			break;
		}
	}

	CloseHandle(hFile);

	tMd5 = ULT::CONV::MBSToUTF16(md5.toString().c_str());
	return tMd5;
}