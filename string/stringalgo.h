template <typename Char>
inline bool IsAsciiWhitespace(Char c) {
  return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}
template <typename Char>
inline bool IsAsciiAlpha(Char c) {
  return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z'));
}
template <typename Char>
inline bool IsAsciiDigit(Char c) {
  return c >= '0' && c <= '9';
}

template <typename Char>
inline bool IsHexDigit(Char c) {
  return (c >= '0' && c <= '9') ||
         (c >= 'A' && c <= 'F') ||
         (c >= 'a' && c <= 'f');
}

template <typename Char>
inline Char HexDigitToInt(Char c) {
  DCHECK(IsHexDigit(c));
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return 0;
}


template<typename T>
struct ToUnsigned {
  typedef T Unsigned;
};

template<>
struct ToUnsigned<char> {
  typedef unsigned char Unsigned;
};
template<>
struct ToUnsigned<signed char> {
  typedef unsigned char Unsigned;
};
template<>
struct ToUnsigned<wchar_t> {
#if defined(WCHAR_T_IS_UTF16)
  typedef unsigned short Unsigned;
#elif defined(WCHAR_T_IS_UTF32)
  typedef uint32 Unsigned;
#endif
};
template<>
struct ToUnsigned<short> {
  typedef unsigned short Unsigned;
};


template<class STR>
static bool DoIsStringASCII(const STR& str) {
  for (size_t i = 0; i < str.length(); i++) {
    typename ToUnsigned<typename STR::value_type>::Unsigned c = str[i];
    if (c > 0x7F)
      return false;
  }
  return true;
}

bool IsStringASCII(const StringPiece& str) {
  return DoIsStringASCII(str);
}

bool IsStringASCII(const string16& str) {
  return DoIsStringASCII(str);
}


template<typename STR>
static size_t TokenizeT(const STR& str,
                        const STR& delimiters,
                        std::vector<STR>* tokens) {
  tokens->clear();

  size_t start = str.find_first_not_of(delimiters);
  while (start != STR::npos) {
    size_t end = str.find_first_of(delimiters, start + 1);
    if (end == STR::npos) {
      tokens->push_back(str.substr(start));
      break;
    } else {
      tokens->push_back(str.substr(start, end - start));
      start = str.find_first_not_of(delimiters, end + 1);
    }
  }

  return tokens->size();
}

size_t Tokenize(const string16& str,
                const string16& delimiters,
                std::vector<string16>* tokens) {
  return TokenizeT(str, delimiters, tokens);
}

/*
 strncpy, wcsncpy's problem:
1.if the dest buffer is not big enough, null will not added to the end, overflow will ocurr
2.if the dest buffer is too big, the left space will be filled with null, which may cause performence problem

strncpy's problem:
1.if the dest buffer is not big engouh, null will not added to the end, overflow will ocurr

http://www.gratisoft.us/todd/papers/strlcpy.html
*/
namespace {

template <typename CHAR>
size_t lcpyT(CHAR* dst, const CHAR* src, size_t dst_size) {
  for (size_t i = 0; i < dst_size; ++i) {
    if ((dst[i] = src[i]) == 0)  // We hit and copied the terminating NULL.
      return i;
  }

  // We were left off at dst_size.  We over copied 1 byte.  Null terminate.
  if (dst_size != 0)
    dst[dst_size - 1] = 0;

  // Count the rest of the |src|, and return it's length in characters.
  while (src[dst_size]) ++dst_size;
  return dst_size;
}

}  // namespace

size_t base::strlcpy(char* dst, const char* src, size_t dst_size) {
  return lcpyT<char>(dst, src, dst_size);
}
size_t base::wcslcpy(wchar_t* dst, const wchar_t* src, size_t dst_size) {
  return lcpyT<wchar_t>(dst, src, dst_size);
}

enum TrimPositions {
  TRIM_NONE     = 0,
  TRIM_LEADING  = 1 << 0,
  TRIM_TRAILING = 1 << 1,
  TRIM_ALL      = TRIM_LEADING | TRIM_TRAILING,
};

template<typename STR>
TrimPositions TrimStringT(const STR& input,
                          const STR& trim_chars,
                          TrimPositions positions,
                          STR* output) {
  // Find the edges of leading/trailing whitespace as desired.
  const size_t last_char = input.length() - 1;
  const size_t first_good_char = (positions & TRIM_LEADING) ?
      input.find_first_not_of(trim_chars) : 0;
  const size_t last_good_char = (positions & TRIM_TRAILING) ?
      input.find_last_not_of(trim_chars) : last_char;

  // When the string was all whitespace, report that we stripped off whitespace
  // from whichever position the caller was interested in.  For empty input, we
  // stripped no whitespace, but we still need to clear |output|.
  if (input.empty() ||
      (first_good_char == STR::npos) || (last_good_char == STR::npos)) {
    bool input_was_empty = input.empty();  // in case output == &input
    output->clear();
    return input_was_empty ? TRIM_NONE : positions;
  }

  // Trim the whitespace.
  *output =
      input.substr(first_good_char, last_good_char - first_good_char + 1);

  // Return where we trimmed from.
  return static_cast<TrimPositions>(
      ((first_good_char == 0) ? TRIM_NONE : TRIM_LEADING) |
      ((last_good_char == last_char) ? TRIM_NONE : TRIM_TRAILING));
}


bool TrimString(const std::string& input,
                const std::string& trim_chars,
                std::string* output) {
  return TrimStringT(input, trim_chars, TRIM_ALL, output) !=
      TRIM_NONE;
}


/*

*/

template <typename STR>
bool StartsWithT(const STR& str, const STR& search, bool case_sensitive) {
  if (case_sensitive) {
    return str.compare(0, search.length(), search) == 0;
  } else {
    if (search.size() > str.size())
      return false;
    return std::equal(search.begin(), search.end(), str.begin(),
                      CaseInsensitiveCompare<typename STR::value_type>());
  }
}

bool StartsWith(const std::string& str, const std::string& search,
                bool case_sensitive) {
  return StartsWithT(str, search, case_sensitive);
}

bool StartsWith(const std::wstring& str, const std::wstring& search,
                bool case_sensitive) {
  return StartsWithT(str, search, case_sensitive);
}

template <typename STR>
bool EndsWithT(const STR& str, const STR& search, bool case_sensitive) {
  size_t str_length = str.length();
  size_t search_length = search.length();
  if (search_length > str_length)
    return false;
  if (case_sensitive)
    return str.compare(str_length - search_length, search_length, search) == 0;
  return std::equal(search.begin(), search.end(),
                    str.begin() + (str_length - search_length),
                    CaseInsensitiveCompare<typename STR::value_type>());
}

bool EndsWith(const std::string& str, const std::string& search,
              bool case_sensitive) {
  return EndsWithT(str, search, case_sensitive);
}

bool EndsWith(const std::wstring& str, const std::wstring& search,
              bool case_sensitive) {
  return EndsWithT(str, search, case_sensitive);
}



template<typename STR>
bool ReplaceCharsT(const STR& input,
                   const STR& replace_chars,
                   const STR& replace_with,
                   STR* output) {
  bool removed = false;
  size_t replace_length = replace_with.length();

  *output = input;

  size_t found = output->find_first_of(replace_chars);
  while (found != STR::npos) {
    removed = true;
    output->replace(found, 1, replace_with);
    found = output->find_first_of(replace_chars, found + replace_length);
  }

  return removed;
}

bool ReplaceChars(const std::wstring& input,
                  const std::wstring& replace_chars,
                  const std::wstring& replace_with,
                  std::wstring* output) {
  return ReplaceCharsT(input, replace_chars, replace_with, output);
}

bool ReplaceChars(const std::string& input,
                  const std::string& replace_chars,
                  const std::string& replace_with,
                  std::string* output) {
  return ReplaceCharsT(input, replace_chars, replace_with, output);
}

bool RemoveChars(const std::wstring& input,
                 const std::wstring& remove_chars,
                 std::wstring* output) {
  return ReplaceChars(input, remove_chars, std::wstring(), output);
}

bool RemoveChars(const std::string& input,
                 const std::string& remove_chars,
                 std::string* output) {
  return ReplaceChars(input, remove_chars, std::string(), output);
}
