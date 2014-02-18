
#include <shlobj.h>
void OpenDirectory()
{
	BROWSEINFO bi;
	wchar_t Buffer[MAX_PATH];
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;
	bi.lpszTitle = L"ѡ��Ŀ���ļ�·��";
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
		COTaskMemFree(pIDList);
	}
}

void OpenFile()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = _T("Exe�ļ�(*.exe)\0*.exe\0�����ļ�(*.*)\0*.*\0");
	ofn.lpstrInitialDir = _T("D:\\Program Files");
	ofn.lpstrFile = szBuffer;
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
	//OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);

	std::wcout << szBuffer << std::endl;
}
