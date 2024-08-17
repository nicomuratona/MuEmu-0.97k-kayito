#include "stdafx.h"
#include "Filter.h"
#include "ReadScript.h"
#include "Util.h"

CFilter gFilter;

CFilter::CFilter()
{
	this->m_count = 0;
}

CFilter::~CFilter()
{

}

void CFilter::Load(char* path)
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

	this->m_count = 0;

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

			FILTER_INFO info;

			memset(&info, 0, sizeof(info));

			strcpy_s(info.label, lpReadScript->GetString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

void CFilter::SetInfo(FILTER_INFO info)
{
	if (this->m_count < 0 || this->m_count >= MAX_FILTER_SYNTAX)
	{
		return;
	}

	this->m_FilterInfo[this->m_count++] = info;
}

void CFilter::CheckSyntax(char* text)
{
	for (int n = 0; n < this->m_count; n++)
	{
		char* temp = text;

		while (true)
		{
			temp = strstr(temp, this->m_FilterInfo[n].label);

			if (temp == 0)
			{
				break;
			}

			int len = strlen(this->m_FilterInfo[n].label);

			memset(temp, 0x2A, len);

			temp += len;
		}
	}
}