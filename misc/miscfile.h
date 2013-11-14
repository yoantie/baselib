BOOL IsValidFileName(char *fname)
{
	return !strpbrk(fname, "\\/<>|:?*");
}

BOOL IsValidFileName(char *fname)
{
	return !strpbrk(fname, "\\/<>|:?*");
}

//get path and file name by org_path
BOOL PathToDir(const char *org_path, char *target_dir)
{
	char	path[256], *fname=NULL;

	if (GetFullPathName(org_path, sizeof(path), path, &fname) == 0 || fname == NULL)
		return	strncpy(target_dir, org_path, 256), FALSE;

	//set the null characterc
	if (fname - path > 3 || path[1] != ':')
		*(fname - 1) = 0;
	else
		*fname = 0;		// C:\ の場合

	strncpy(target_dir, path, 256);
	return	TRUE;
}

#define MAX_BUF 1024
BOOL PathToFname(const char *org_path, char *target_fname)
{
	char	path[MAX_BUF], *fname=NULL;

	if (GetFullPathName(org_path, sizeof(path), path, &fname) == 0 || fname == NULL)
		return	FALSE;

	strncpy(target_fname, fname, MAX_BUF);
	return	TRUE;
}

BOOL IsSafePath(const char *fullpath, const char *fname)
{
	char	fname2[MAX_BUF];

	if (!PathToFname(fullpath, fname2))
		return	FALSE;

	return	strcmp(fname, fname2) == 0 ? TRUE : FALSE;
}

//get file info
HANDLE FindFirstFileEx(const char *path, WIN32_FIND_DATA *fdat)
{
	return ::FindFirstFile(path, fdat);
}


BOOL GetFileInfomation(const char *path, WIN32_FIND_DATA *fdata)
{
	HANDLE	fh;

	if ((fh = FindFirstFile(path, fdata)) != INVALID_HANDLE_VALUE)
	{
		::FindClose(fh);
		return	TRUE;
	}

	if ((fh = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0)) != INVALID_HANDLE_VALUE)
	{
		BY_HANDLE_FILE_INFORMATION	info;
		BOOL	info_ret = ::GetFileInformationByHandle(fh, &info);
		::CloseHandle(fh);
		if (info_ret) {
			memcpy(fdata, &info, (char *)&info.dwVolumeSerialNumber - (char *)&info);
			return	TRUE;
		}
	}

	return	(fdata->dwFileAttributes = GetFileAttributes(path)) == 0xffffffff ? FALSE : TRUE;
}

//使用的时候要做适当修改
void ForcePathToFname(const char *org_path, char *target_fname)
{
	if (PathToFname(org_path, target_fname))
		return;

	if (org_path[1] == ':')
		wsprintf(target_fname, "(%c-drive)", *org_path);
	else if (org_path[0] == '\\' && org_path[1] == '\\') { //这种情况可能只出现在Unicode字符中
		if (!PathToFname(org_path + 2, target_fname))
			wsprintf(target_fname, "(root)");
	}
	else wsprintf(target_fname, "(unknown)");
}
