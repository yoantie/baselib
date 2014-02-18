
// copied from atl
// helper for multi-byte character sets
inline bool _IsDBCSTrailByte(LPCTSTR lpstr, int nChar)
{
#ifndef _UNICODE
	int i = nChar;
	for( ; i > 0; i--)
	{
		if(!::IsDBCSLeadByte(lpstr[i - 1]))
			break;
	}
	return ((nChar > 0) && (((nChar - i) & 1) != 0));
#else // _UNICODE
	lpstr; nChar;
	return false;
#endif // _UNICODE
}

//safe critical section -- also copied from atl
class CComSafeDeleteCriticalSection : public CComCriticalSection
{
public:
	CComSafeDeleteCriticalSection(): m_bInitialized(false)
	{
	}

	~CComSafeDeleteCriticalSection() throw()
	{
		if (!m_bInitialized)
		{
			return;
		}
		m_bInitialized = false;
		CComCriticalSection::Term();
	}

	HRESULT Init() throw()
	{
		ATLASSERT( !m_bInitialized );
		HRESULT hr = CComCriticalSection::Init();
		if (SUCCEEDED(hr))
		{
			m_bInitialized = true;
		}
		return hr;
	}

	HRESULT Term() throw()
	{
		if (!m_bInitialized)
		{
			return S_OK;
		}
		m_bInitialized = false;
		return CComCriticalSection::Term();
	}

	HRESULT Lock()
	{
		// CComSafeDeleteCriticalSection::Init or CComAutoDeleteCriticalSection::Init
		// not called or failed.
		// m_critsec member of CComObjectRootEx is now of type
		// CComAutoDeleteCriticalSection. It has to be initialized
		// by calling CComObjectRootEx::_AtlInitialConstruct
		ATLASSUME(m_bInitialized);
		return CComCriticalSection::Lock();
	}

private:
	bool m_bInitialized;
};

//use template for the self-adaption of functionA/W

template <typename CharType>
HRESULT InternalStringCchVPrintf(CharType* dest_buffer,
                                 size_t dest_size,
                                 const CharType* format_str,
                                 va_list arg_list);

template <>
HRESULT InternalStringCchVPrintf<char>(char* dest_buffer,
                                       size_t dest_size,
                                       const char* format_str,
                                       va_list arg_list) {
  return ::StringCchVPrintfA(dest_buffer, dest_size, format_str, arg_list);
}

template <>
HRESULT InternalStringCchVPrintf<wchar_t>(wchar_t* dest_buffer,
                                          size_t dest_size,
                                          const wchar_t* format_str,
                                          va_list arg_list) {
  return ::StringCchVPrintfW(dest_buffer, dest_size, format_str, arg_list);
}

// Define a templated wrapper for strlen() that will call the appropriate
// W/A version based on template parameter.
template <typename CharType>
size_t InternalStrlen(const CharType* str);

template <>
size_t InternalStrlen<char>(const char* str) {
  return ::strlen(str);
}

template <>
size_t InternalStrlen<wchar_t>(const wchar_t* str) {
  return ::wcslen(str);
}
