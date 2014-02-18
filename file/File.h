#pragma once
#include "basetype.h"
#include "typedef.h"

class CFile
{
	HANDLE _handle;

	bool Create(LPCWSTR fileName, DWORD desiredAccess,
		DWORD shareMode, DWORD creationDisposition, DWORD flagsAndAttributes);
	/*for write*/
	bool Open(LPCTSTR fileName, DWORD shareMode, DWORD createDisposition, DWORD flagsAndAttributes);
public:
	CFile(void):_handle(INVALID_HANDLE_VALUE){};
	~CFile(void);
	
	bool Open(LPCTSTR fileName);/*for read*/
	bool Open(LPCTSTR fileName, DWORD createDisposition);/*for read*/
	bool Create(LPCTSTR fileName, bool createAlways);	/*for write*/

	bool Open();
	bool Close();
	
	bool GetPosition(UInt64& opsition) const;
	bool GetLength(UInt64& length) const;
	
	bool Read1(void *data, UInt32 size, UInt32 &processedSize);
	bool ReadPart(void *data, UInt32 size, UInt32& processedSize);
	bool Read(void *data, UInt32 size, UInt32 &processedSize);

	bool Seek(Int64 distanceToMove, DWORD moveMethod, UInt64 &newPosition) const;
	bool Seek(UInt64 position, UInt64 &newPosition);
	bool SeekToBegin();
	bool SeekToEnd(UInt64 &newPosition);

	bool SetTime(const FILETIME *cTime, const FILETIME *aTime, const FILETIME *mTime);
	bool SetMTime(const FILETIME *mTime);
	bool WritePart(const void* data, UInt32 size, UInt32 &processedSize);
	bool Write(const void* data, UInt32 size, UInt32 &processedSize);
	bool SetEndOfFile();
	bool SetLength(UInt64 length);
};
