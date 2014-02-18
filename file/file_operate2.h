#pragma once

#include "stdafx.h"

namespace ULT {
	namespace FILE {

		/* file */
		inline bool IsFileExist(const std::wstring& strFile) 
		{
			if (strFile.empty()) return false;

			DWORD dwAttr = GetFileAttributesW(strFile.c_str());
			return (dwAttr != 0xffffffff);
		}
		/* path */
		inline std::wstring GetModulePath()
		{
			wchar_t szPath[MAX_PATH] = { 0 };
			::GetModuleFileName(NULL, szPath, MAX_PATH);
				
			*wcsrchr(szPath, '\\') = 0;
			wcscat(szPath, L"\\");
			return std::wstring(szPath);
		}

		inline bool GetFileInfoExt(const std::wstring& strFile, std::wstring& strPath, std::wstring& strName)
		{
			if (strFile.empty()) 
				return false;

			std::wstring::size_type nsep = strFile.rfind(L"\\");
			if (nsep == std::wstring::npos) {
				strPath = L"";
				strName = strFile;
			} else {
				strPath = strFile.substr(0, nsep);
				strName = strFile.substr(nsep+1, strFile.length()-nsep-1);
			}

			return true;
		}

		inline void MakeDirectory(std::wstring& strPath)
		{
			if (strPath.empty()) return;

			if (strPath[strPath.length()] != '\\') {
				strPath.append(L"\\");
			}

			std::wstring strDir;
			std::wstring::size_type nPos = 0;
			
			while ((nPos = strPath.find(L"\\", nPos)) != std::wstring::npos)
			{
				strDir = strPath.substr(0, nPos++);
				::CreateDirectory(strDir.c_str(), NULL);
			}
		}

		inline void DeleteDirectory(std::wstring& strPath)
		{
			if (strPath.empty()) return;

			if (strPath[strPath.length()-1] != '\\') {
				strPath.append(L"\\");
			}

			std::wstring strSearchPath = strPath;
			strSearchPath.append(L"*.*");

			WIN32_FIND_DATA wfd;
			HANDLE hFind = FindFirstFile(strSearchPath.c_str(), &wfd);
			if (hFind == INVALID_HANDLE_VALUE)
			{	
				::FindClose(hFind);
				return;
			}

			BOOL bFound = TRUE;
			while (bFound == TRUE) {
				if ( wcscmp(wfd.cFileName, L".") == 0 || wcscmp(wfd.cFileName, L"..") == 0 )
				{
					bFound = ::FindNextFileW(hFind, &wfd);
					continue;
				}

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					DeleteDirectory(std::wstring(strPath).append(wfd.cFileName));
				}

				::DeleteFile( std::wstring(strPath + wfd.cFileName).c_str());
				bFound = ::FindNextFile(hFind, &wfd);
			}

			::FindClose(hFind);
			::RemoveDirectory(strPath.c_str());
		}

		/* disk */
		inline __int64 GetFreeDiskSpace() 
		{
			ULARGE_INTEGER freeBytes, totalBytes, userBytes;
			freeBytes.QuadPart  = 0;
			totalBytes.QuadPart = 0;
			userBytes.QuadPart  = 0;
		
			::GetDiskFreeSpaceEx(NULL, &freeBytes, &totalBytes, &userBytes);

			return userBytes.QuadPart;
		}
	};
};