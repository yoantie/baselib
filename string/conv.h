#pragma once

#include "stdafx.h"
#include <string>

namespace ULT {
	namespace CONV {

		inline std::wstring UTF8ToUnicode(const char* szSrc)
		{
			wchar_t szBuffer[2048] = { 0 };
			int nlength = ::MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, NULL, 0);
			if (nlength > 0) {
				::MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, szBuffer, nlength);
			}

			return std::wstring(szBuffer, nlength+1);
		}


		inline std::string UnicodeToUTF8(const wchar_t *szSrc)
		{
			char szBuffer[2048] = { 0 };
			int nlength = ::WideCharToMultiByte(CP_UTF8, 0, szSrc, -1, NULL, 0, 0, 0);
			if (nlength > 0) {
				::WideCharToMultiByte(CP_UTF8, 0, szSrc, -1, szBuffer, nlength, 0, 0);
			}

			return std::string(szBuffer, nlength+1);
		}

		/* 目前其实转换的都是std::string/std::wstring, 没有null结束符，
		   按此种方式转换后都会添加一个null,所以最后要去掉，以免比对字符时出错
		*/
		inline std::wstring MBSToUnicode(const char* szSrc)
		{
			wchar_t szBuffer[2048] = { 0 };
			int nlength = ::MultiByteToWideChar(CP_ACP, 0, szSrc, -1, NULL, 0);
			if (nlength > 0) {
				::MultiByteToWideChar(CP_ACP, 0, szSrc, -1, szBuffer, nlength);
			}

			return std::wstring(szBuffer, nlength-1);
		}

		inline std::string UnicodeToMBS(const wchar_t *szSrc)
		{
			char szBuffer[2048] = { 0 };
			int nlength = ::WideCharToMultiByte(CP_ACP, 0, szSrc, -1, NULL, 0, 0, 0);
			if (nlength > 0) {
				::WideCharToMultiByte(CP_ACP, 0, szSrc, -1, szBuffer, nlength, 0, 0);
			}

			return std::string(szBuffer, nlength+1);
		}

	};

};
