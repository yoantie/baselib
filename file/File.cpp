#include "StdAfx.h"
#include "InFile.h"

// ReadFile and WriteFile functions in Windows have BUG:
// If you Read or Write 64MB or more (probably min_failure_size = 64MB - 32KB + 1)
// from/to Network file, it returns ERROR_NO_SYSTEM_RESOURCES
// (Insufficient system resources exist to complete the requested service).

// Probably in some version of Windows there are problems with other sizes:
// for 32 MB (maybe also for 16 MB).
// And message can be "Network connection was lost"

static UInt32 kChunkSizeMax = (1 << 22);


CFile::~CFile(void)
{
}

bool CFile::Create(LPCWSTR fileName, DWORD desiredAccess, DWORD shareMode, DWORD creationDisposition, DWORD flagsAndAttributes)
{
	if (!Close())
		return false;
	_handle = ::CreateFileW(fileName, desiredAccess, shareMode,
    (LPSECURITY_ATTRIBUTES)NULL, creationDisposition,
    flagsAndAttributes, (HANDLE)NULL);

	return (_handle != INVALID_HANDLE_VALUE);
}

bool CFile::Open(LPCTSTR fileName, DWORD shareMode, DWORD createDisposition, DWORD flagsAndAttributes)
{
	return Create(fileName, GENERIC_WRITE, shareMode, createDisposition, flagsAndAttributes);
}

bool CFile::Open(LPCWSTR fileName)
{
	return Create(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL);
}

bool CFile::Open(LPCTSTR fileName, DWORD createDisposition)
{
	return Open(fileName, FILE_SHARE_READ, createDisposition, FILE_ATTRIBUTE_NORMAL);
}

bool CFile::Create(LPCTSTR fileName, bool createAlways)
{
	DWORD dwCreateDispose = createAlways ? CREATE_ALWAYS : CREATE_NEW;
	return Open(fileName, dwCreateDispose);
}

bool CFile::Open()
{
	return (_handle != INVALID_HANDLE_VALUE);
}

bool CFile::Close()
{
	if (_handle == INVALID_HANDLE_VALUE)
		return true;
	if (!::CloseHandle(_handle))
		return false;
	_handle = INVALID_HANDLE_VALUE;
	return true;
}

bool CFile::GetPosition(UInt64 &position) const
{
  return Seek(0, FILE_CURRENT, position);
}

bool CFile::GetLength(UInt64 &length) const
{
	DWORD sizeHigh;
	DWORD sizeLow = ::GetFileSize(_handle, &sizeHigh);
	if (sizeLow == 0xFFFFFFFF)
		if (::GetLastError() != NO_ERROR)
			return false;
	length = (((UInt64)sizeHigh) << 32) + sizeLow;
	return true;
}

bool CFile::Seek(Int64 distanceToMove, DWORD moveMethod, UInt64 &newPosition) const
{
	LARGE_INTEGER value;
	value.QuadPart = distanceToMove;
	value.LowPart = ::SetFilePointer(_handle, value.LowPart, &value.HighPart, moveMethod);
	if (value.LowPart == 0xFFFFFFFF)
		if (::GetLastError() != NO_ERROR)
			return false;
	newPosition = value.QuadPart;
	return true;
}

bool CFile::Seek(UInt64 position, UInt64 &newPosition)
{
  return Seek(position, FILE_BEGIN, newPosition);
}

bool CFile::SeekToBegin()
{
  UInt64 newPosition;
  return Seek(0, newPosition);
}

bool CFile::SeekToEnd(UInt64 &newPosition)
{
  return Seek(0, FILE_END, newPosition);
}

bool CFile::Read1(void *data, UInt32 size, UInt32 &processedSize)
{
	DWORD processedLoc = 0;
	bool res = ::ReadFile(_handle, data, size, &processedLoc, NULL) == TRUE;
	processedSize = processedLoc;
	return res;
}

bool CFile::ReadPart(void *data, UInt32 size, UInt32& processedSize)
{
	if (size > kChunkSizeMax)
		size = kChunkSizeMax;
	
	return Read1(data, size, processedSize);
}

bool CFile::Read(void *data, UInt32 size, UInt32 &processedSize)
{
	processedSize = 0;

	do {
		UInt32 processedLoc = 0;
		bool res = ReadPart(data, size, processedLoc);
		processedSize += processedLoc;
		if (!res)
			return false;
		if (processedLoc == 0)
			return true;
		data = (void*)((unsigned char*)data + processedLoc);
		size -= processedLoc;
	} while (size > 0);
}

bool CFile::SetTime(const FILETIME *cTime, const FILETIME *aTime, const FILETIME *mTime)
{
	return ::SetFileTime(_handle, cTime, aTime, mTime) == TRUE;
}

bool CFile::SetMTime(const FILETIME *mTime)
{
	return ::SetFileTime(_handle, NULL, NULL, mTime) == TRUE;
}

bool CFile::WritePart(const void *data, UInt32 size, UInt32 &processedSize)
{
	if (size > kChunkSizeMax)
		size = kChunkSizeMax;
	DWORD processedLoc = 0;
	bool res = ::WriteFile(_handle, data, size, &processedLoc, NULL);
	processedSize = (UInt32)processedLoc;
	return res;
}

bool CFile::Write(const void *data, UInt32 size, UInt32 &processedSize)
{
	processedSize = 0;
	do {
		UInt32 processedLoc = 0;
		bool res = WritePart(data, size, processedLoc);
		processedSize += processedLoc;
		if (!res)
			return false;
		if (processedLoc == 0)
			return true;
		data = (const void*)((const unsigned char*)data + processedLoc);
		size -= processedLoc;
	} while (size > 0);

	return true;
}

bool CFile::SetEndOfFile()
{
	return ::SetEndOfFile(_handle) == TRUE;
}

bool CFile::SetLength(UInt64 length)
{
	UInt64 newPosition;
	if (!Seek(length, newPosition))
		return false;
	if (newPosition != length)
		return false;
	return SetEndOfFile();
}