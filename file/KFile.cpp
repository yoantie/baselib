#include "StdAfx.h"
#include "KFile.h"

CKFileImpl::CKFileImpl(const char* file):m_path(file)
{}

bool CKFileImpl::IsExistImpl()
{
	assert(!m_path.empty());
	
	DWORD attr = GetFileAttributes(m_path.c_str());
	if (attr == 0xFFFFFFFF)
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
		case ERROR_NOT_READY:
		case ERROR_INVALID_DRIVE:
			return false;
		default:
			return false;//throw;
		}
	}
	return true;
}

bool CKFileImpl::IsFileImpl()
{
	return !IsDirectoryImpl() && !IsDeviceImpl();
}

bool CKFileImpl::IsDirectoryImpl()
{
	assert(!m_path.empty());

	DWORD attr = GetFileAttributes(m_path.c_str());
	if (attr == 0xFFFFFFFF)
		return false;

	return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
}


bool CKFileImpl::IsDeviceImpl()
{
	//return
	//	m_path.compare(0, 4, "\\\\.\\") == 0 ||
	//	icompare(m_path, "CON") == 0 ||
	//	icompare(m_path, "PRN") == 0 ||
	//	icompare(m_path, "AUX") == 0 ||
	//	icompare(m_path, "NUL") == 0 ||
	//	icompare(m_path, "LPT1") == 0 ||
	//	icompare(m_path, "LPT2") == 0 ||
	//	icompare(m_path, "LPT3") == 0 ||
	//	icompare(m_path, "LPT4") == 0 ||
	//	icompare(m_path, "LPT5") == 0 ||
	//	icompare(m_path, "LPT6") == 0 ||
	//	icompare(m_path, "LPT7") == 0 ||
	//	icompare(m_path, "LPT8") == 0 ||
	//	icompare(m_path, "LPT9") == 0 ||
	//	icompare(m_path, "COM1") == 0 ||
	//	icompare(m_path, "COM2") == 0 ||
	//	icompare(m_path, "COM3") == 0 ||
	//	icompare(m_path, "COM4") == 0 ||
	//	icompare(m_path, "COM5") == 0 ||
	//	icompare(m_path, "COM6") == 0 ||
	//	icompare(m_path, "COM7") == 0 ||
	//	icompare(m_path, "COM8") == 0 ||
	//	icompare(m_path, "COM9") == 0;

	static std::string device[] = {
		"\\\\.\\", "CON", "PRN", "AUX", "NUL",
		"LPT1","LPT2","LPT3","LPT4","LPT5","LPT6","LPT7","LPT8","LPT9",
		"COM","COM2","COM3","COM4","COM5","COM6","COM7","COM8","COM9"
	};

	for ( int i = 0; i < sizeof(device)/sizeof(device[0]); i++ ) {
		if ( device[i] == m_path )
			return true;
	}

	return false;
}

void CKFileImpl::CopyToImpl(const std::string& path)
{
	assert(!m_path.empty());
	CopyFile(m_path.c_str(), path.c_str(), FALSE);
}

void CKFileImpl::RenameToImpl(const std::string& path)
{
	assert(!m_path.empty());
	MoveFile(m_path.c_str(), path.c_str());
}

bool CKFileImpl::CreateFileImpl()
{
	assert(!m_path.empty());

	HANDLE hFile = CreateFile(m_path.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return true;
	}
	
	return false;
}

bool CKFileImpl::CreateDirectoryImpl()
{
	assert(!m_path.empty());
	
	if (IsExistImpl() && IsDirectoryImpl())
		return false;
	if (CreateDirectory(m_path.c_str(), 0) == 0)
		return false;

	return true;
}

CTimeStamp CKFileImpl::GetLastModified()
{
	assert(!m_path.empty());
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (::GetFileAttributes(m_path.c_str())) {
		return CTimeStamp::FromFileTimeNP(fad.ftLastWriteTime.dwLowDateTime, fad.ftLastWriteTime.dwHighDateTime);
	}

	throw;//throw an exception temporarily
}

CKFile::CKFile(const char* file)
:CKFileImpl(file)
{
}

CKFile::~CKFile(void)
{
}

bool CKFile::IsExist()
{
	return IsExistImpl();
}

bool CKFile::IsFile()
{
	return IsFileImpl();
}

bool CKFile::IsDevice()
{
	return IsDeviceImpl();
}

bool CKFile::IsDirectory()
{
	return IsDirectoryImpl();
}

void CKFile::CopyTo(const std::string& path)
{
	return CopyToImpl(path);
}

void CKFile::RenameTo(const std::string& path)
{
	return RenameToImpl(path);
}

bool CKFile::CreateFile()
{
	return CreateFileImpl();
}

bool CKFile::CreateDirectory()
{
	return CreateDirectoryImpl();
}