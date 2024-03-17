#include "stdafx.h"
#include "CustomPKFree.h"
#include "MemScript.h"
#include "Util.h"

CPKFree gPKFree;

CPKFree::CPKFree()
{
	this->m_PKFreeInfo.clear();
}

CPKFree::~CPKFree()
{

}

void CPKFree::Load(char* path)
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

	this->m_PKFreeInfo.clear();

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

			MOVE_PKFREE_INFO info;

			info.Map = lpMemScript->GetNumber();

			info.X = lpMemScript->GetAsNumber();

			info.Y = lpMemScript->GetAsNumber();

			info.TX = lpMemScript->GetAsNumber();

			info.TY = lpMemScript->GetAsNumber();

			this->m_PKFreeInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpMemScript->GetLastError());
	}

	delete lpMemScript;
}

bool CPKFree::CheckPKFree(int map, int x, int y)
{
	for (std::vector<MOVE_PKFREE_INFO>::iterator it = this->m_PKFreeInfo.begin(); it != this->m_PKFreeInfo.end(); it++)
	{
		if (it->Map != map)
		{
			continue;
		}

		if ((it->X > x || it->TX < x) || (it->Y > y || it->TY < y))
		{
			continue;
		}
		else
		{
			return 1;
		}
	}

	return 0;
}