#include "stdafx.h"
#include "Filter.h"
#include "MemScript.h"
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

	this->m_count = 0;

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

			FILTER_INFO info;

			memset(&info, 0, sizeof(info));

			strcpy_s(info.label, lpMemScript->GetString());

			this->SetInfo(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
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