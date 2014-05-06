#pragma once


#include "stdafx.h"
#include <string>
#include <vector>


namespace ULT {
	namespace TEXT {
		template<class S, class C>
		inline void TrimLeft(S& Str, C Ch)
		{
			typename S::const_iterator it  = Str.begin();
			typename S::const_iterator end = Str.end();

			while (it != end && *it == Ch) ++it;
			Str.erase(Str.begin(), it);
		}

		template<class S, class C>
		inline void TrimRight(S &Str, C Ch)
		{
			int pos = int(Str.size()) - 1;

			while (pos >= 0 && Str[pos] == Ch) --pos;
			Str.resize(pos+1);
		}

		template<class S, class C>
		inline void Trim(S& Str, C Ch)
		{
			int first = 0;
			int last  = int(Str.size()) - 1;

			while (first <= last && Str[first] == Ch) first++;
			while (last >= first && Str[last] == Ch) last--;

			Str.resize(last+1);
			Str.erase(0, first);
		}

		template<class S>
		void Replace(S& Str, S& From, S& To)
		{
			S::size_type index;
			while( ( index = Str.find( From ) ) != S::npos )
			{
				Str.replace(index, From.size(), To);
			}
		}

		template<class S>
		int icompare(const S& str1, const S& str2)
		{
			typename S::const_iterator it1(str1.begin());
			typename S::const_iterator end1(str1.end());
			typename S::const_iterator it2(str2.begin());
			typename S::const_iterator end2(str2.end());

			while (it1 != end1 && it2 != end2)
			{
				typename S::value_type c1(::tolower(*it1));
				typename S::value_type c2(::tolower(*it2));
				if (c1 < c2)
					return -1;
				else if(c1 > c2)
					return 1;
				++it1; 
				++it2;
			}

			if (it1 == end1)
				return it2 == end2 ? 0 : -1;
			else
				return 1;
		}
	};

	inline void TrimLeft(std::wstring& str, wchar_t ch = ' ')
	{
		TEXT::TrimLeft(str, ch);
	}

	inline void TrimRight(std::wstring& str, wchar_t ch  = ' ')
	{
		TEXT::TrimRight(str, ch);
	}

	inline void Trim(std::wstring& str, wchar_t ch  = ' ')
	{
		TEXT::Trim(str, ch);
	}

	inline int Stricmp(const std::wstring& src, const std::wstring& dest)
	{
		return TEXT::icompare(src, dest);
	}
};
