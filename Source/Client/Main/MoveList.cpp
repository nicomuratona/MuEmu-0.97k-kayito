#include "stdafx.h"
#include "MoveList.h"
#include "Protect.h"

CMoveList gMoveList;

CMoveList::CMoveList()
{
	this->m_MoveListSwitch = false;

	this->MainWidth = 180.0f;

	this->MainBaseHeight = 60.0f;

	this->MainPosX = 1.0f;

	this->MainPosY = 1.0f;

	this->SectionWidth = 50.0f;

	this->MapNamePosX = this->MainPosX + 5.0f;

	this->ReqLevelPosX = this->MapNamePosX + this->SectionWidth + 10.0f;

	this->ReqMoneyPosX = this->ReqLevelPosX + this->SectionWidth + 10.0f;

	this->m_MapList.clear();
}

CMoveList::~CMoveList()
{

}

void CMoveList::Init()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	SetCompleteHook(0xE8, 0x00525CEC, &this->MainProc);
}

void CMoveList::Toggle()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (SceneFlag == 5
	    && !*(BYTE*)0x00559C84 // Chat
	    && !*(BYTE*)0x07EAA144) // Guild Creator
	{
		this->m_MoveListSwitch ^= 1;

		PlayBuffer(25, 0, 0);
	}
}

void CMoveList::MainProc()
{
	((void(__cdecl*)()) 0x004C3530)();

	if (gMoveList.m_MoveListSwitch)
	{
		gMoveList.RenderMoveListBack();

		gMoveList.RenderMoveListMaps();
	}
}

void CMoveList::RenderMoveListBack()
{
	this->MainHeight = this->MainBaseHeight + (this->m_MapList.size() * 12.0f);

	this->MainHeight = (this->MainHeight > 430.0f) ? 430.0f : this->MainHeight;

	EnableAlphaTest(true);

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);

	RenderColor(this->MainPosX, this->MainPosY, this->MainWidth, this->MainHeight);

	glColor3f(1.0f, 1.0f, 1.0f);

	EnableAlphaTest(true);

	DWORD backupBgTextColor = SetBackgroundTextColor;

	SetBackgroundTextColor = Color4f(0, 0, 0, 0);

	DWORD backupTextColor = SetTextColor;

	SetTextColor = Color4f(255, 204, 26, 255);

	SelectObject(m_hFontDC, g_hFontBold);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + 5, "Teleport Window", (int)(this->MainWidth - 10) * WindowWidth / 640, 1, 0);

	SetBackgroundTextColor = Color4f(0, 0, 0, 0);

	SetTextColor = Color4f(127, 178, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MapNamePosX, (int)this->MainPosY + 20, "Map", (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

	RenderText((int)this->ReqLevelPosX, (int)this->MainPosY + 20, "Min. Level", (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

	RenderText((int)this->ReqMoneyPosX, (int)this->MainPosY + 20, "Cost", (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

	SetBackgroundTextColor = Color4f(255, 0, 0, 255);

	SetTextColor = Color4f(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + (int)this->MainHeight - 15, "Close", (int)(this->MainWidth - 10) * WindowWidth / 640, 1, 0);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMoveList::RenderMoveListMaps()
{
	EnableAlphaTest(true);

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	SetBackgroundTextColor = Color4f(0, 0, 0, 0);

	SelectObject(m_hFontDC, g_hFont);

	char text[32];

	int PosY = 0;

	STRUCT_DECRYPT;

	BYTE Class = *(BYTE*)(*(DWORD*)(CharacterAttribute)+0x0B) & 7;

	int RealMinLevel = 0;

	for (std::vector<MAP_INFO>::iterator it = this->m_MapList.begin(); it != this->m_MapList.end(); it++)
	{
		it->CanMove = true;

		if (Class == 3)
		{
			RealMinLevel = ((it->LevelMin * 2) / 3);
		}
		else
		{
			RealMinLevel = it->LevelMin;
		}

		if (*(BYTE*)(Hero + 0x2EA) >= 5) // PK
		{
			it->CanMove = false;
		}
		else if (RealMinLevel > *(WORD*)(*(DWORD*)(CharacterAttribute)+0x0E))
		{
			it->CanMove = false;
		}
		else if (it->Money > *(DWORD*)(*(DWORD*)(CharacterMachine)+0x548))
		{
			it->CanMove = false;
		}
		else if (it->MapNumber == MAP_ATLANS && (*(short*)(Hero + 0x2B8) == 0x332 || *(short*)(Hero + 0x2B8) == 0x333))
		{
			it->CanMove = false;
		}
		else if (it->MapNumber == MAP_ICARUS && ((*(short*)(Hero + 0x2A0) == -1 && *(short*)(Hero + 0x2B8) != 0x333) || *(short*)(Hero + 0x2B8) == 0x332))
		{
			it->CanMove = false;
		}

		if (it->CanMove)
		{
			SetTextColor = Color4f(255, 255, 255, 255);
		}
		else
		{
			SetTextColor = Color4f(164, 39, 17, 255);
		}

		if (it->Selected && it->CanMove)
		{
			glColor4f(0.8f, 0.8f, 0.1f, 0.6f);

			RenderColor(this->MapNamePosX, this->MainPosY + 34.0f + (PosY * 12.0f), this->MainWidth - 10.0f, 10.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			EnableAlphaTest(true);
		}

		RenderText((int)this->MapNamePosX, (int)this->MainPosY + 35 + (PosY * 12), it->MapName, (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

		wsprintf(text, "%d", RealMinLevel);
		RenderText((int)this->ReqLevelPosX, (int)this->MainPosY + 35 + (PosY * 12), text, (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

		wsprintf(text, "%d", it->Money);
		RenderText((int)this->ReqMoneyPosX, (int)this->MainPosY + 35 + (PosY * 12), text, (int)(this->SectionWidth) * WindowWidth / 640, 1, 0);

		PosY++;
	}

	STRUCT_ENCRYPT;

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMoveList::CheckMoveListMouse()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (!this->m_MoveListSwitch)
	{
		return;
	}

	if (MouseX >= this->MainPosX
	    && MouseX <= (this->MainPosX + this->MainWidth)
	    && MouseY >= this->MainPosY
	    && MouseY <= (this->MainPosY + this->MainHeight))
	{
		MouseOnWindow = true;

		if (this->CheckClickOnMap())
		{
			return;
		}

		if (this->CheckClickOnClose())
		{
			return;
		}

		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;
		}
	}
}

bool CMoveList::CheckClickOnMap()
{
	int PosY = 0;

	for (std::vector<MAP_INFO>::iterator it = this->m_MapList.begin(); it != this->m_MapList.end(); it++)
	{
		if (MouseX >= this->MapNamePosX
		    && MouseX <= (this->MapNamePosX + this->MainWidth - 10.0f)
		    && MouseY >= this->MainPosY + 35 + (PosY * 12)
		    && MouseY <= this->MainPosY + 45 + (PosY * 12))
		{
			it->Selected = true;

			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				if (!it->CanMove)
				{
					return true;
				}

				it->Selected = false;

				this->Toggle();

				char cCommand[100];

				wsprintf(cCommand, "/move %s", it->MapName);

				SendChat(cCommand);

				return true;
			}
		}
		else
		{
			it->Selected = false;
		}

		PosY++;
	}

	return false;
}

bool CMoveList::CheckClickOnClose()
{
	if (MouseX >= (this->MainPosX + 5)
	    && MouseX <= (this->MainPosX + this->MainWidth - 10.0f)
	    && MouseY >= ((int)this->MainPosY + (int)this->MainHeight - 15)
	    && MouseY <= ((int)this->MainPosY + (int)this->MainHeight - 5))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			this->Toggle();
		}

		return true;
	}

	return false;
}

void CMoveList::GCMoveListRecv(PMSG_MOVE_LIST_RECV* lpMsg)
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	this->m_MapList.clear();

	for (int n = 0; n < lpMsg->count; n++)
	{
		MOVE_LIST_INFO* lpInfo = (MOVE_LIST_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_MOVE_LIST_RECV) + (sizeof(MOVE_LIST_INFO) * n));

		MAP_INFO info;

		info.MapNumber = lpInfo->MapNumber;

		memcpy(info.MapName, lpInfo->MapName, sizeof(info.MapName));

		info.LevelMin = lpInfo->LevelMin;

		info.Money = lpInfo->Money;

		info.CanMove = true;

		info.Selected = false;

		this->m_MapList.push_back(info);
	}
}