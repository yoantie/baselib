/* 清除文件夹 */
void ClearAllFiles(std::wstring& path)
{
	if (path.find_last_of(L"\\") != path.length() - 1)
		path += L"\\";

	std::wstring dir = path + L"*.*";

	BOOL bFound = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFile = ::FindFirstFile(dir.c_str(), &find_data);
	if ( (bFound = (hFile != INVALID_HANDLE_VALUE)) == FALSE)
		return;

	std::wstring sub_dir;
	std::wstring find_file;
	while (bFound == TRUE) {
		find_file = find_data.cFileName;
		if (find_file != L"." && find_file != L"..") {
			find_file = path + find_data.cFileName;
			if ( (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
				ClearAllFiles(find_file);
			} else {
				if (DeleteFile(find_file.c_str()) == FALSE) {
					wchar_t buffer[128] = { 0 };
					swprintf(buffer, L"%s, DeleteFile Err:%d\r\n", find_file.c_str(), GetLastError());
					OutputDebugString(buffer);
				}
			}
		}

		bFound = FindNextFile(hFile, &find_data);
	}

	::FindClose(hFile);
	if (::RemoveDirectory(path.c_str()) == FALSE) {
		wchar_t buffer[128] = { 0 };
		swprintf(buffer, L"%s, RemoveDirectory Err:%d\r\n", path.c_str(), GetLastError());
		OutputDebugString(buffer);
	}
}

/*获取完整路径名*/
bool GetLongPathBase(LPCWSTR s, std::wstring &res)
{
  res.clear();
  int len = wcslen(s);
  wchar_t c = s[0];
  if (len < 1 || c == L'\\' || c == L'.' && (len == 1 || len == 2 && s[1] == L'.'))
    return true;
  std::wstring curDir;
  bool isAbs = false;
  if (len > 3)
    isAbs = (s[1] == L':' && s[2] == L'\\' && (c >= L'a' && c <= L'z' || c >= L'A' && c <= L'Z'));

  if (!isAbs)
    {
	wchar_t Buffer[MAX_PATH+1];
      DWORD needLength = ::GetCurrentDirectoryW(MAX_PATH + 1, Buffer);
      if (needLength == 0 || needLength > MAX_PATH)
        return false;

	  curDir = Buffer;
	  if (curDir[curDir.length() - 1] != L'\\')
        curDir += L'\\';
    }
  res = std::wstring(L"\\\\?\\") + curDir + s;
  return true;
}

bool GetLongPath(LPCWSTR path, std::wstring &longPath)
{
  if (GetLongPathBase(path, longPath))
	  return !longPath.empty();
  return false;
}

/*设备名称合法性判断*/
bool IsDeviceName(LPCWSTR n)
{
  if (n[0] != '\\' || n[1] != '\\' || n[2] != '.' ||  n[3] != '\\')
    return false;
  int len = (int)wcslen(n);
  if (len == 6 && n[5] == ':')
    return true;
  if (len < 18 || len > 22 || wcsncmp(n + 4, L"PhysicalDrive", 13) != 0)
    return false;
  for (int i = 17; i < len; i++)
    if (n[i] < '0' || n[i] > '9')
      return false;
  return true;
}

/**/
const TCHAR kDirDelimiter = '\\';
void NormalizeDirPathPrefix(std::string &dirPath)
{
	if (dirPath.empty())
		return;

	if (dirPath.find_last_of(kDirDelimiter) != dirPath.length() - 1)
		dirPath += kDirDelimiter;
}

/*解析文件名和扩展名*/
const TCHAR kExtensionDelimiter = '.';
void SplitNameToPureNameAndExtension(const std::string &fullName,
   std::string &pureName, std::string &extensionDelimiter, std::string &extension)
{
	int index = fullName.find_last_of(kExtensionDelimiter);
	if (index < 0)
	{
		pureName = fullName;
		extensionDelimiter.clear();
		extension.clear();
	}
	else
	{
		pureName = fullName.substr(0, index);
		extensionDelimiter = kExtensionDelimiter;
		extension = fullName.substr(index+1);
	}
}


/*获取设备(系统盘符)列表*/
typedef std::vector<std::wstring> StringVec;

bool GetLogicalDriveStrings(StringVec &driveStrings)
{
	driveStrings.clear();

    UINT32 size = GetLogicalDriveStringsW(0, NULL);
    if (size == 0)
      return false;
	std::vector<wchar_t> buffer(size);
    UINT32 newSize = GetLogicalDriveStringsW(size, &buffer[0]);
    if (newSize == 0)
      return false;
    if (newSize > size)
      return false;
	std::wstring string;
    for (UINT32 i = 0; i < newSize; i++)
    {
      WCHAR c = buffer[i];
      if (c == L'\0')
      {
		  driveStrings.push_back(string);
		  string.clear();
      }
      else
        string += c;
    }
	return string.empty();
}

/**/
bool SetFileAttributes(LPCTSTR fileName, DWORD fileAttributes)
{
	if (::SetFileAttributes(fileName, fileAttributes))
		return true;
	return false;
}

bool DeleteFileAlways(LPCTSTR name)
{
	if (!SetFileAttributes(name, 0))
		return false;
	if (::DeleteFile(name))
		return true;

	return false;
}
