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

	this->m_MoveList.clear();
}

CMoveList::~CMoveList()
{

}

void CMoveList::Toggle()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (InputEnable || TabInputEnable || GoldInputEnable || GuildInputEnable)
	{
		return;
	}

	if (!this->CheckInterfaces())
	{
		this->m_MoveListSwitch = false;

		return;
	}

	this->m_MoveListSwitch ^= 1;

	PlayBuffer(25, 0, 0);
}

void CMoveList::Render()
{
	if (this->m_MoveListSwitch)
	{
		if (!this->CheckInterfaces())
		{
			this->m_MoveListSwitch ^= 1;
		}
		else
		{
			this->RenderFrame();

			this->RenderMapsList();
		}
	}
}

void CMoveList::UpdateMouse()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (!this->m_MoveListSwitch)
	{
		return;
	}

	if (!this->CheckInterfaces())
	{
		this->m_MoveListSwitch = false;

		return;
	}

	if (IsWorkZone((int)this->MainPosX, (int)this->MainPosY, (int)this->MainWidth, (int)this->MainHeight))
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

bool CMoveList::CheckInterfaces()
{
	if (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened || GoldenArcherOpened || GuildCreatorOpened)
	{
		return false;
	}

	return true;
}

void CMoveList::RenderFrame()
{
	this->MainHeight = this->MainBaseHeight + (this->m_MoveList.size() * 12.0f);

	this->MainHeight = (this->MainHeight > 430.0f) ? 430.0f : this->MainHeight;

	EnableAlphaTest(true);

	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);

	RenderColor(this->MainPosX, this->MainPosY, this->MainWidth, this->MainHeight);

	glColor3f(1.0f, 1.0f, 1.0f);

	EnableAlphaTest(true);

	DWORD backupBgTextColor = SetBackgroundTextColor;

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

	DWORD backupTextColor = SetTextColor;

	SetTextColor = Color4b(255, 204, 26, 255);

	SelectObject(m_hFontDC, g_hFontBold);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + 5, "Teleport Window", (int)(this->MainWidth - 10) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

	SetTextColor = Color4b(127, 178, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MapNamePosX, (int)this->MainPosY + 20, "Map", (int)(this->SectionWidth) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	RenderText((int)this->ReqLevelPosX, (int)this->MainPosY + 20, "Min. Level", (int)(this->SectionWidth) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	RenderText((int)this->ReqMoneyPosX, (int)this->MainPosY + 20, "Cost", (int)(this->SectionWidth) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = Color4b(255, 0, 0, 255);

	SetTextColor = Color4b(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + (int)this->MainHeight - 15, "Close", (int)(this->MainWidth - 10) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMoveList::RenderMapsList()
{
	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	EnableAlphaTest(true);

	int PosY = (int)this->MainPosY + 35;

	if (this->m_MoveList.empty())
	{
		SetBackgroundTextColor = Color4b(255, 255, 255, 0);

		SetTextColor = Color4b(255, 255, 255, 255);

		SelectObject(m_hFontDC, g_hFontBig);

		RenderText((int)this->MainPosX + 5, PosY - 8, "NO MOVE INFO", (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_CENTER, NULL);
	}
	else
	{
		char text[32];

		int RealMinLevel = 0;

		STRUCT_DECRYPT;

		BYTE Class = *(BYTE*)(CharacterAttribute + 0x0B) & 7;

		for (std::vector<MOVE_LIST_INFO>::iterator it = this->m_MoveList.begin(); it != this->m_MoveList.end(); it++)
		{
			if (IsWorkZone((int)this->MainPosX + 5, PosY, (int)(this->MainWidth - 10), 10))
			{
				SetBackgroundTextColor = Color4b(204, 204, 25, 153);
			}
			else
			{
				SetBackgroundTextColor = Color4b(255, 255, 255, 0);
			}

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
			else if (RealMinLevel > *(WORD*)(CharacterAttribute + 0x0E))
			{
				it->CanMove = false;
			}
			else if (it->Money > *(DWORD*)(CharacterMachine + 0x548))
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
				SetTextColor = Color4b(255, 255, 255, 255);
			}
			else
			{
				SetTextColor = Color4b(164, 39, 17, 255);
			}

			if (it->CanMove && IsWorkZone((int)this->MainPosX + 5, PosY, (int)(this->MainWidth - 10.0f), 10))
			{
				SetBackgroundTextColor = Color4b(204, 204, 25, 153);
			}
			else
			{
				SetBackgroundTextColor = Color4b(255, 255, 255, 0);
			}

			wsprintf(text, "%d", RealMinLevel);
			RenderText((int)this->MainPosX + 5, PosY, text, (int)(this->MainWidth - 10.0f) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

			SetBackgroundTextColor = Color4b(255, 255, 255, 0);

			RenderText((int)this->MapNamePosX, PosY, it->MapName, (int)(this->SectionWidth) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

			ConvertGold(it->Money, text);
			RenderText((int)this->ReqMoneyPosX, PosY, text, (int)(this->SectionWidth) * WindowWidth / 640, RT3_SORT_CENTER, NULL);

			PosY += 12;
		}

		STRUCT_ENCRYPT;
	}

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

bool CMoveList::CheckClickOnMap()
{
	int PosY = (int)this->MainPosY + 35;

	for (std::vector<MOVE_LIST_INFO>::iterator it = this->m_MoveList.begin(); it != this->m_MoveList.end(); it++)
	{
		if (IsWorkZone((int)(this->MapNamePosX), PosY, (int)(this->MainWidth - 10.0f), (int)(10.0f)))
		{
			if (MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				if (!it->CanMove)
				{
					return true;
				}

				this->Toggle();

				char Text[100];

				wsprintf(Text, "/move %s", it->MapName);

				SendChat(Text);

				return true;
			}
		}

		PosY += 12;
	}

	return false;
}

bool CMoveList::CheckClickOnClose()
{
	if (IsWorkZone((int)(this->MainPosX + 5.0f), (int)(this->MainPosY + this->MainHeight - 15.0f), (int)(this->MainWidth - 10.0f), (int)(10.0f)))
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

	this->m_MoveList.clear();

	for (int i = 0; i < lpMsg->count; i++)
	{
		MOVE_LIST_INFO* lpInfo = (MOVE_LIST_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_MOVE_LIST_RECV) + (sizeof(MOVE_LIST_INFO) * i));

		MOVE_LIST_INFO info;

		info.MapNumber = lpInfo->MapNumber;

		memcpy(info.MapName, lpInfo->MapName, sizeof(info.MapName));

		info.LevelMin = lpInfo->LevelMin;

		info.Money = lpInfo->Money;

		info.CanMove = lpInfo->CanMove;

		this->m_MoveList.push_back(info);
	}
}