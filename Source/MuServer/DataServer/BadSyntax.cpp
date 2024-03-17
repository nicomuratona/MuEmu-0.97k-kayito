#include "stdafx.h"
#include "BadSyntax.h"
#include "MemScript.h"
#include "Util.h"

CBadSyntax gBadSyntax;

CBadSyntax::CBadSyntax()
{
	this->m_BadSyntaxInfo.clear();
}

CBadSyntax::~CBadSyntax()
{

}

void CBadSyntax::Load(char* path)
{
	CMemScript* lpMemScript = new CMemScript;

	if (lpMemScript == NULL)
	{
		ErrorMessageBox(MEM_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpMemScript->SetBuffer(path))
	{
		ErrorMessageBox(lpMemScript->GetLastError());

		delete lpMemScript;

		return;
	}

	this->m_BadSyntaxInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpMemScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			BAD_SYNTAX_INFO info;

			strcpy_s(info.syntax, lpMemScript->GetString());

			this->m_BadSyntaxInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CBadSyntax::CheckSyntax(char* text)
{
	for (std::vector<BAD_SYNTAX_INFO>::iterator it = this->m_BadSyntaxInfo.begin(); it != this->m_BadSyntaxInfo.end(); it++)
	{
		if (strstr(text, it->syntax) != 0)
		{
			return false;
		}
	}

	return true;
}