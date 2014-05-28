BOOL LoadPngFromResource(int nResID,CImage* pImg)
{
	HRSRC hSrc = FindResource(NULL,MAKEINTRESOURCE(nResID),_T("PNG"));
	if(hSrc == NULL) return FALSE;

	HGLOBAL hGlobal = LoadResource(NULL,hSrc);
	if(hGlobal == NULL) return FALSE;

	LPVOID lpData = LockResource(hGlobal);
	if(lpData == NULL) {
		FreeResource(hGlobal);
		return FALSE;
	}

	DWORD dwSize = SizeofResource(NULL,hSrc);

	HGLOBAL hRawData = GlobalAlloc(GMEM_FIXED,dwSize);
	if(hRawData == NULL) {
		FreeResource(hGlobal);
		return FALSE;
	}

	CopyMemory(hRawData,lpData,dwSize);
	IStream* pStream;
	HRESULT hr = CreateStreamOnHGlobal(hRawData,FALSE,&pStream);
	pImg->Load(pStream);

	GlobalFree(hRawData);
	FreeResource(hGlobal);
	return TRUE;
}
