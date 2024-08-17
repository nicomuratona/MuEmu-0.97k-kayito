#include "stdafx.h"
#include "Gate.h"
#include "DefaultClassInfo.h"
#include "Map.h"
#include "ReadScript.h"
#include "Util.h"

CGate gGate;

CGate::CGate()
{
	this->m_GateInfo.clear();
}

CGate::~CGate()
{

}

void CGate::Load(char* path)
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

	this->m_GateInfo.clear();

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

			GATE_INFO info;

			info.Index = lpReadScript->GetNumber();

			info.Flag = lpReadScript->GetAsNumber();

			info.Map = lpReadScript->GetAsNumber();

			info.X = lpReadScript->GetAsNumber();

			info.Y = lpReadScript->GetAsNumber();

			info.TX = lpReadScript->GetAsNumber();

			info.TY = lpReadScript->GetAsNumber();

			info.TargetGate = lpReadScript->GetAsNumber();

			info.Dir = lpReadScript->GetAsNumber();

			info.MinLevel = lpReadScript->GetAsNumber();

			info.MaxLevel = lpReadScript->GetAsNumber();

			info.MinReset = lpReadScript->GetAsNumber();

			info.MaxReset = lpReadScript->GetAsNumber();

			info.AccountLevel = lpReadScript->GetAsNumber();

			this->m_GateInfo.insert(std::pair<int, GATE_INFO>(info.Index, info));
		}
	}
	catch (...)
	{
		ErrorMessageBox(lpReadScript->GetError());
	}

	delete lpReadScript;
}

bool CGate::GetInfo(int index, GATE_INFO* lpInfo)
{
	std::map<int, GATE_INFO>::iterator it = this->m_GateInfo.find(index);

	if (it == this->m_GateInfo.end())
	{
		return 0;
	}
	else
	{
		(*lpInfo) = it->second;

		return 1;
	}
}

int CGate::GetGateMap(int index)
{
	GATE_INFO info;

	if (this->GetInfo(index, &info) == 0)
	{
		return -1;
	}

	return info.Map;
}

int CGate::GetMoveLevel(LPOBJ lpObj, int map, int level)
{
	if (lpObj->Class == CLASS_MG)
	{
		return ((level * 2) / 3);
	}

	return level;
}

bool CGate::IsGate(int index)
{
	GATE_INFO info;

	if (this->GetInfo(index, &info) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

bool CGate::IsInGate(LPOBJ lpObj, int index)
{
	GATE_INFO info;

	if (this->GetInfo(index, &info) == 0)
	{
		return 0;
	}

	if (lpObj->Map != info.Map)
	{
		return 0;
	}

	if (lpObj->X < (info.TX - 5) || lpObj->X >(info.TX + 5) || lpObj->Y < (info.TY - 5) || lpObj->Y >(info.TY + 5))
	{
		return 0;
	}

	if (info.MinLevel != -1 && lpObj->Level < this->GetMoveLevel(lpObj, info.Map, info.MinLevel))
	{
		return 0;
	}

	if (info.MaxLevel != -1 && lpObj->Level > info.MaxLevel)
	{
		return 0;
	}

	if (info.MinReset != -1 && lpObj->Reset < info.MinReset)
	{
		return 0;
	}

	if (info.MaxReset != -1 && lpObj->Reset > info.MaxReset)
	{
		return 0;
	}

	if (lpObj->AccountLevel < info.AccountLevel)
	{
		return 0;
	}

	return 1;
}

bool CGate::GetGate(int index, int* gate, int* map, int* x, int* y, int* dir, int* level)
{
	GATE_INFO info;

	if (this->GetInfo(index, &info) == 0)
	{
		return 0;
	}

	if (info.TargetGate != 0 && this->GetInfo(info.TargetGate, &info) == 0)
	{
		return 0;
	}

	int px, py;

	for (int n = 0; n < 50; n++)
	{
		if ((info.TX - info.X) > 0)
		{
			px = info.X + (GetLargeRand() % (info.TX - info.X));
		}
		else
		{
			px = info.X;
		}

		if ((info.TY - info.Y) > 0)
		{
			py = info.Y + (GetLargeRand() % (info.TY - info.Y));
		}
		else
		{
			py = info.Y;
		}

		if (gMap[info.Map].CheckAttr(px, py, 4) == 0 && gMap[info.Map].CheckAttr(px, py, 8) == 0)
		{
			(*gate) = info.Index;

			(*map) = info.Map;

			(*x) = px;

			(*y) = py;

			(*dir) = info.Dir;

			(*level) = info.MinLevel;

			return 1;
		}
	}

	return 0;
}