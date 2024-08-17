#include "stdafx.h"
#include "CustomPKFree.h"
#include "ReadScript.h"
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

	this->m_PKFreeInfo.clear();

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

			MOVE_PKFREE_INFO info;

			info.Map = lpReadScript->GetNumber();

			info.X = lpReadScript->GetAsNumber();

			info.Y = lpReadScript->GetAsNumber();

			info.TX = lpReadScript->GetAsNumber();

			info.TY = lpReadScript->GetAsNumber();

			this->m_PKFreeInfo.push_back(info);
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
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