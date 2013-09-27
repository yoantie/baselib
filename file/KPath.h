#pragma once
#include <vector>
#include <string>

class CKPathImpl
{
public:
	static std::string CurrentImp();
};

class CKPath
{
	typedef std::vector<std::string> StringVec;

public:
	CKPath(void);
	CKPath(const std::string path);
	~CKPath(void);

public:
	CKPath  Parent() const;
	CKPath  Absolute();
	CKPath  Absolute(const CKPath& base);
	CKPath& MakeAbsolute();
	CKPath& MakeDirectory();
	std::string GetFileName();

	void Parse(const std::string path);

private:
	CKPath& PushDirectory(const std::string& dir);
	std::string Current();
	CKPath& MakeAbsolute(const CKPath& base);
	CKPath& MakeParent();

private:
	bool m_bAbsolute;
	std::string m_sDevice;
	std::string m_sName; //the last "\" or "\\" or "somepath"
	std::string m_sNode;
	StringVec   m_vecDirs;
};
