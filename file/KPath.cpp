#include "StdAfx.h"
#include "KPath.h"
#include <windows.h>

std::string CKPathImpl::CurrentImp()
{
	std::string result;
	DWORD len = ::GetCurrentDirectory(0, NULL);
	if (len > 0)
	{
		std::vector<char> buffer(len);
		DWORD n = ::GetCurrentDirectory(len, &buffer[0]);
		if (n > 0 && n <= len)
		{
			if (result[result.size() - 1] != '\\')
				result.append("\\");
			return result;
		}
	}

	return "";
}

CKPath::CKPath(void)
{
}

CKPath::CKPath(const std::string path)
{
	Parse(path);
}

CKPath::~CKPath(void)
{
}

CKPath CKPath::Parent() const
{
	CKPath p(*this);
	return p.MakeParent();
}

CKPath CKPath::Absolute()
{
	CKPath result(*this);
	if (!result.m_bAbsolute) {
		result.MakeAbsolute();
	}
	return result;
}

CKPath CKPath::Absolute(const CKPath& base)
{
	CKPath result(*this);
	if (!result.m_bAbsolute) {
		result.MakeAbsolute(base);
	}
	return result;
}

CKPath& CKPath::MakeAbsolute()
{
	return MakeAbsolute(Current());
}

CKPath& CKPath::MakeDirectory()
{
	PushDirectory( m_sName);
	m_sName.clear();
	return *this;
}

std::string CKPath::GetFileName()
{
	return m_sName;
}

void CKPath::Parse(const std::string path)
{
	std::string::const_iterator it  = path.begin();
	std::string::const_iterator end = path.end();

	if (it != end) {
		if (*it == '\\' || *it == '/') { m_bAbsolute = true; ++it; }
		if (m_bAbsolute && it != end && (*it == '\\' || *it == '/')) 
		{
			++it;
			while (it != end && (*it != '\\' && *it != '/')) m_sNode = *it++;
		} 
		else if (it != end) 
		{//check "c:\"
			char d = *it++;
			if (it != end && *it == ':' ) 
			{
				if (m_bAbsolute && (( d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z'))) throw; 
				m_bAbsolute = true;
				m_sDevice += d;
				++it;
				if (it == end && *it != '\\' && *it != '/') throw;
				it++;
			}
			else 
			{
				--it;
			}
		}

		while (it != end)
		{
			std::string name;
			while (it != end && *it != '\\' && *it != '/') name += *it++;
			if (it != end)
				PushDirectory(name);
			else
				m_sName = name;
			if (it != end) ++it;
		}
	}

	if (!m_sNode.empty() && m_vecDirs.empty() && !m_sName.empty())
		MakeDirectory();
}

CKPath& CKPath::PushDirectory(const std::string& dir)
{
	if (!dir.empty() && dir != ".")
	{
		if (dir == "..")
		{
			if (!m_vecDirs.empty() && m_vecDirs.back() != "..")
				m_vecDirs.pop_back();
			else if (!m_bAbsolute)
				m_vecDirs.push_back(dir);
		}
		else
			m_vecDirs.push_back(dir);
	}

	return *this;
}

std::string CKPath::Current()
{
	return CKPathImpl::CurrentImp();
}

CKPath& CKPath::MakeAbsolute(const CKPath& base)
{
	if (!m_bAbsolute) {
		CKPath tmp = base;
		tmp.MakeDirectory();
		for (StringVec::const_iterator it = m_vecDirs.begin(); it != m_vecDirs.end(); it++) {
			tmp.PushDirectory(*it);
		}

		m_sNode     = tmp.m_sNode;
		m_sDevice   = tmp.m_sDevice;
		m_vecDirs   = tmp.m_vecDirs;
		m_bAbsolute = tmp.m_bAbsolute;
	}

	return *this;
}

CKPath& CKPath::MakeParent()
{
	if (m_sName.empty())
	{
		if (m_vecDirs.empty())
		{
			if (!m_bAbsolute)
			{
				m_vecDirs.push_back("..");
			}
		}
		else
		{
			if (m_vecDirs.back() == "..")
				m_vecDirs.push_back("..");
			else
				m_vecDirs.pop_back();
		}
	}
	else
	{
		m_sName.clear();
	}

	return *this;
}