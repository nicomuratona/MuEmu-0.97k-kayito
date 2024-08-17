#include "stdafx.h"
#include "BadSyntax.h"
#include "ReadScript.h"
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
	CReadScript* lpReadScript = new CReadScript;

	if (lpReadScript == NULL)
	{
		ErrorMessageBox(READ_SCRIPT_ALLOC_ERROR, path);

		return;
	}

	if (!lpReadScript->Load(path))
	{
		ErrorMessageBox(READ_SCRIPT_FILE_ERROR, path);

		delete lpReadScript;

		return;
	}

	this->m_BadSyntaxInfo.clear();

	try
	{
		eTokenResult token;

		while (true)
		{
			token = lpReadScript->GetToken();

			if (token == TOKEN_END || token == TOKEN_END_SECTION)
			{
				break;
			}

			BAD_SYNTAX_INFO info;

			strcpy_s(info.syntax, lpReadScript->GetString());

			this->m_BadSyntaxInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
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