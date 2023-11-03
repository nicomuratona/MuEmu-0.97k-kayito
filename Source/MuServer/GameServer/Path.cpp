#include "stdafx.h"
#include "Path.h"

CPath gPath;

CPath::CPath()
{

}

CPath::~CPath()
{

}

void CPath::SetMainPath(char* path)
{
	strcpy_s(this->m_MainPath, path);
}

char* CPath::GetFullPath(char* file)
{
	strcpy_s(this->m_FullPath, this->m_MainPath);

	strcat_s(this->m_FullPath, file);

	return this->m_FullPath;
}