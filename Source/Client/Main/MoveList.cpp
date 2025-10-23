#include "stdafx.h"
#include "MoveList.h"
#include "EventTimer.h"
#include "MiniMap.h"
#include "PrintPlayer.h"
#include "Protect.h"

CMoveList gMoveList;

CMoveList::CMoveList()
{
	this->MoveListSwitch = false;

	this->MainBaseHeight = 60.0f;

	this->MainPosX = 5.0f;

	this->MainPosY = 5.0f;

	this->SectionWidth = 40.0f;

	this->MainWidth = ((this->SectionWidth * 3.5f) + 10.0f);

	this->m_MoveList.clear();
}

CMoveList::~CMoveList()
{

}

bool CMoveList::GetMoveListState()
{
	return this->MoveListSwitch;
}

void CMoveList::Toggle()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (CheckInputInterfaces())
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->MoveListSwitch = false;

		return;
	}

	if (gEventTimer.GetEventTimerState())
	{
		gEventTimer.Toggle();
	}

	if (gMiniMap.GetMiniMapState())
	{
		gMiniMap.Toggle();
	}

	this->MoveListSwitch ^= 1;

	PlayBuffer(25, 0, 0);
}

void CMoveList::Render()
{
	if (!this->MoveListSwitch)
	{
		return;
	}

	this->RenderFrame();

	this->RenderMapsList();
}

void CMoveList::UpdateMouse()
{
	if (gProtect.m_MainInfo.EnableMoveList == 0)
	{
		return;
	}

	if (!this->MoveListSwitch)
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->MoveListSwitch = false;

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

void CMoveList::RenderFrame()
{
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

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + 5, "Teleport Window", REAL_WIDTH((int)(this->MainWidth - 10.0f)), RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

	SetTextColor = Color4b(127, 178, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	int PosX = (int)this->MainPosX + 5;

	RenderText(PosX, (int)this->MainPosY + 20, "Map", REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

	PosX += ((int)this->SectionWidth);

	RenderText(PosX, (int)this->MainPosY + 20, "Level", REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

	PosX += ((int)this->SectionWidth);

	RenderText(PosX, (int)this->MainPosY + 20, "Cost", REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

	PosX += ((int)this->SectionWidth);

	RenderText(PosX, (int)this->MainPosY + 20, "VIP", REAL_WIDTH((int)(this->SectionWidth * 0.5f)), RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = Color4b(255, 0, 0, 255);

	SetTextColor = Color4b(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFont);

	RenderText((int)this->MainPosX + 5, (int)this->MainPosY + (int)this->MainHeight - 15, "Close", REAL_WIDTH((int)(this->MainWidth - 10.0f)), RT3_SORT_CENTER, NULL);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMoveList::RenderMapsList()
{
	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	EnableAlphaTest(true);

	int PosX = (int)this->MainPosX + 5;

	int PosY = (int)this->MainPosY + 35;

	if (this->m_MoveList.empty())
	{
		SetBackgroundTextColor = Color4b(255, 255, 255, 0);

		SetTextColor = Color4b(255, 255, 255, 255);

		SelectObject(m_hFontDC, g_hFontBig);

		RenderText(PosX, PosY - 8, "NO MOVE INFO", REAL_WIDTH((int)(this->MainWidth - 10.0f)), RT3_SORT_CENTER, NULL);
	}
	else
	{
		char text[32];

		STRUCT_DECRYPT;

		for (std::vector<MOVE_LIST_INFO>::iterator it = this->m_MoveList.begin(); it != this->m_MoveList.end(); it++)
		{
			if (it->MinLevel != -1 && *(WORD*)(CharacterAttribute + 0x0E) < it->MinLevel)
			{
				it->CanMove = false;
			}
			else if (it->MaxLevel != -1 && *(WORD*)(CharacterAttribute + 0x0E) > it->MaxLevel)
			{
				it->CanMove = false;
			}
			else if (it->MinReset != -1 && gPrintPlayer.ViewReset < (DWORD)it->MinReset)
			{
				it->CanMove = false;
			}
			else if (it->MaxReset != -1 && gPrintPlayer.ViewReset > (DWORD)it->MaxReset)
			{
				it->CanMove = false;
			}
			else if (*(DWORD*)(CharacterMachine + 0x548) < it->Money)
			{
				it->CanMove = false;
			}
			else if (this->PKLimitFree == 0 && *(BYTE*)(Hero + 0x2EA) >= PKLVL_OUTLAW)
			{
				it->CanMove = false;
			}
			else if (it->MapNumber == MAP_ATLANS && (*(short*)(Hero + 0x2B8) == GET_ITEM_MODEL(13, 2) || *(short*)(Hero + 0x2B8) == GET_ITEM_MODEL(13, 3))) // Uniria,Dinorant
			{
				it->CanMove = false;
			}
			else if (it->MapNumber == MAP_ICARUS && (*(short*)(Hero + 0x2A0) == -1 && *(short*)(Hero + 0x2B8) != GET_ITEM_MODEL(13, 3))) // Wings
			{
				it->CanMove = false;
			}
			else if (it->MapNumber == MAP_ICARUS && *(short*)(Hero + 0x2B8) == GET_ITEM_MODEL(13, 2)) // Uniria
			{
				it->CanMove = false;
			}
			else
			{
				it->CanMove = true;
			}

			if (it->CanMove)
			{
				if (IsWorkZone(PosX, PosY, (int)(this->MainWidth - 10), 10))
				{
					EnableAlphaTest(true);

					glColor4f(0.8f, 0.8f, 0.1f, 0.6f);

					RenderColor((float)PosX, (float)PosY, this->MainWidth - 10.0f, 10.0f);

					glColor3f(1.0f, 1.0f, 1.0f);

					EnableAlphaTest(true);
				}

				SetTextColor = Color4b(255, 255, 255, 255);
			}
			else
			{
				SetTextColor = Color4b(164, 39, 17, 255);
			}

			SetBackgroundTextColor = Color4b(255, 255, 255, 0);

			RenderText(PosX, PosY, it->MapName, REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

			PosX += ((int)this->SectionWidth);

			/*
			if (it->MinLevel == -1) // MinLevel -1
			{
				if (it->MaxLevel == -1) // MinLevel -1, MaxLevel -1
				{
					wsprintf(text, "~ / ~");
				}
				else // MinLevel -1, MaxLevel valid
				{
					wsprintf(text, "~ / %d", it->MaxLevel);
				}
			}
			else if (it->MaxLevel == -1) // MinLevel valid, MaxLevel -1
			{
				wsprintf(text, "%d / ~", it->MinLevel);
			}
			else // Both Valid
			{
				wsprintf(text, "%d / %d", it->MinLevel, it->MaxLevel);
			}
			*/

			if (it->MinLevel == -1) // MinLevel -1
			{
				wsprintf(text, "~");
			}
			else // Valid
			{
				wsprintf(text, "%d", it->MinLevel);
			}

			RenderText(PosX, PosY, text, REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

			PosX += ((int)this->SectionWidth);

			ConvertGold(it->Money, text);
			RenderText(PosX, PosY, text, REAL_WIDTH((int)this->SectionWidth), RT3_SORT_CENTER, NULL);

			PosX += ((int)this->SectionWidth);

			if (it->AccountLevel != -1 && it->AccountLevel > 0)
			{
				SetTextColor = Color4b(255, 0, 0, 255);
				RenderText(PosX, PosY, "[VIP]", REAL_WIDTH((int)(this->SectionWidth * 0.5f)), RT3_SORT_CENTER, NULL);
			}

			PosY += 12;

			PosX = (int)this->MainPosX + 5;
		}

		STRUCT_ENCRYPT;
	}

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

bool CMoveList::CheckClickOnMap()
{
	int PosX = (int)this->MainPosX + 5;

	int PosY = (int)this->MainPosY + 35;

	for (std::vector<MOVE_LIST_INFO>::iterator it = this->m_MoveList.begin(); it != this->m_MoveList.end(); it++)
	{
		if (IsWorkZone(PosX, PosY, (int)(this->MainWidth - 10.0f), (int)(10.0f)))
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

	this->PKLimitFree = lpMsg->PKLimitFree;

	this->m_MoveList.clear();

	for (int i = 0; i < lpMsg->count; i++)
	{
		MOVE_LIST_INFO* lpInfo = (MOVE_LIST_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_MOVE_LIST_RECV) + (sizeof(MOVE_LIST_INFO) * i));

		MOVE_LIST_INFO info;

		info.MapNumber = lpInfo->MapNumber;

		memcpy(info.MapName, lpInfo->MapName, sizeof(info.MapName));

		info.MinLevel = lpInfo->MinLevel;

		info.MaxLevel = lpInfo->MaxLevel;

		info.MinReset = lpInfo->MinReset;

		info.MaxReset = lpInfo->MaxReset;

		info.AccountLevel = lpInfo->AccountLevel;

		info.Money = lpInfo->Money;

		info.CanMove = lpInfo->CanMove;

		this->m_MoveList.push_back(info);
	}

	this->MainHeight = this->MainBaseHeight + (lpMsg->count * 12.0f);

	this->MainHeight = (this->MainHeight > 430.0f) ? 430.0f : this->MainHeight;
}