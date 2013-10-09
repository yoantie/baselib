#pragma once

#include "..\time\TimeStamp.h"
#include <windows.h>
#include <string>
#include <cassert>

class CKFileImpl
{
public:
	CKFileImpl(const char* file);
	~CKFileImpl(){}

protected:
	typedef unsigned __int64 m_nFileSize;

	bool IsExistImpl();
	bool IsFileImpl();
	bool IsDeviceImpl();
	bool IsDirectoryImpl();
	void CopyToImpl(const std::string& path);
	void RenameToImpl(const std::string& path);
//	void RemoveImpl();
	bool CreateFileImpl();
	bool CreateDirectoryImpl();
	CTimeStamp GetLastModified();
private:
	std::string m_path;
};

class CKFile : private CKFileImpl
{
public:
	CKFile(const char* file);
	~CKFile(void);

public:
	bool IsExist();
	bool IsFile();
	bool IsDevice();
	bool IsDirectory();
	void CopyTo(const std::string& path);
	void RenameTo(const std::string& path);
//	void RemoveImpl();
	bool CreateFile();
	bool CreateDirectory();
};
