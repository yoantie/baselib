
#include <shlobj.h>
void OpenDirectory()
{
	BROWSEINFO bi;
	wchar_t Buffer[MAX_PATH];
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = L"选择目标文件路径";
//	bi.ulFlags = BIF_EDITBOX;
//	bi.ulFlags = BIF_RETURNFSANCESTORS;
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	bi.lpfn = NULL;
	bi.lParam = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		std::wcout << Buffer << std::endl;
		CoTaskMemFree(pIDList);
	}
}

void OpenFile()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = _T("Exe文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0");
	ofn.lpstrInitialDir = _T("D:\\Program Files");
	ofn.lpstrFile = szBuffer;
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);

	std::wcout << szBuffer << std::endl;
}


#define BIF_NEWDIALOGSTYLE   0x40
#define BIF_USENEWUI （BIF_NEWDIALOGSTYLE|BIF_EDITBOX）

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}


void OpenDirWithNewFolderAbility()
{
	BROWSEINFO bi;
	wchar_t Buffer[MAX_PATH];
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = L"选择目标文件路劲";
	bi.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE;
	bi.lParam = 0;
	bi.lpfn = BrowseCallbackProc;

	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if (pIDList) {
		SHGetPathFromIDList(pIDList, Buffer);

		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pIDList);
			imalloc->Release();
		}
	}
}
