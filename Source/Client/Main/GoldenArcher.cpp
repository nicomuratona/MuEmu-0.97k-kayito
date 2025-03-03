#include "stdafx.h"
#include "GoldenArcher.h"
#include "ItemManager.h"
#include "Protocol.h"

CGoldenArcher gGoldenArcher;

CGoldenArcher::CGoldenArcher()
{
	this->iStartPosX = 450;

	this->iStartPosY = 0;

	this->m_Rena.Type = GET_ITEM(14, 21);
	this->m_Rena.Durability = 1;
	this->m_Rena.Level = 0;
	this->m_Rena.Option1 = 0;

	this->m_Stone.Type = GET_ITEM(14, 21);
	this->m_Stone.Durability = 1;
	this->m_Stone.Level = 15;
	this->m_Stone.Option1 = 0;

	this->m_CurrentPage = 1;
	this->m_TotalPages = 1;

	// Rena exchager
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1] = 10;
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2] = 100;
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3] = 500;
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4] = 1000;

	// Stone exchanger VIP
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1] = 50;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1] = 86400;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2] = 100;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2] = 86400;

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3] = 150;
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3] = 86400;

	// Stone exchanger Lucky Number
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] = 30;

	memset(this->m_LuckyNumber, 0, sizeof(this->m_LuckyNumber));

	this->m_MyLuckyNumbers.clear();
}

CGoldenArcher::~CGoldenArcher()
{

}

void CGoldenArcher::Init()
{
	SetCompleteHook(0xE9, 0x004F46A0, &this->Render);

	SetCompleteHook(0xE9, 0x004E7AC0, &this->UpdateMouse);

	MemorySet(0x004BFEAA, 0x90, 5); // Skip Rendering items

	SetCompleteHook(0xE9, 0x0044A31A, &this->MoveCharacter_GoldenArcherMovement);

	SetCompleteHook(0xE9, 0x0044AA70, &this->MoveCharacter_GoldenArcherChatMessages);
}

void CGoldenArcher::Render()
{
	if (GoldenArcherOpenType == GOLDEN_ARCHER_INTERFACE_NONE)
	{
		return;
	}

	/***********************/
	// Set Window Position
	/***********************/
	*(DWORD*)0x07EA5288 = gGoldenArcher.iStartPosX;
	*(DWORD*)0x07EA5284 = gGoldenArcher.iStartPosY;

	/***********************/
	// Render Frame
	/***********************/
	gGoldenArcher.RenderFrame();

	/***********************/
	// Render Title
	/***********************/
	gGoldenArcher.RenderTitle();

	/***********************/
	// Render Types
	/***********************/
	switch (GoldenArcherOpenType)
	{
		case GOLDEN_ARCHER_INTERFACE_RENA:
		{
			gGoldenArcher.RenderRenaCollector();

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_STONE:
		{
			gGoldenArcher.RenderStoneCollector();

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_LUCKY_NUMBER:
		{
			gGoldenArcher.RenderLuckyNumberExchanger();

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_BINGO:
		{
			gGoldenArcher.RenderLuckyNumberList();

			break;
		}
	}

	/***********************/
	// Render Close
	/***********************/
	gGoldenArcher.RenderCloseButton();
}

void CGoldenArcher::UpdateMouse()
{
	if (GoldenArcherOpenType == GOLDEN_ARCHER_INTERFACE_NONE)
	{
		return;
	}

	int iPosX = gGoldenArcher.iStartPosX;
	int iPosY = gGoldenArcher.iStartPosY;

	if (IsWorkZone(iPosX, iPosY, 190, 433))
	{
		MouseOnWindow = true;
	}

	switch (GoldenArcherOpenType)
	{
		case GOLDEN_ARCHER_INTERFACE_RENA:
		{
			if (gGoldenArcher.CheckRenaCollector())
			{
				return;
			}

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_STONE:
		{
			if (gGoldenArcher.CheckStoneCollector())
			{
				return;
			}

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_LUCKY_NUMBER:
		{
			if (gGoldenArcher.CheckLuckyNumberExchanger())
			{
				return;
			}

			break;
		}

		case GOLDEN_ARCHER_INTERFACE_BINGO:
		{
			if (gGoldenArcher.CheckLuckyNumberList())
			{
				return;
			}

			break;
		}
	}

	if (gGoldenArcher.CheckCloseButton())
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

void CGoldenArcher::RenderFrame()
{
	float fPosX = (float)gGoldenArcher.iStartPosX;
	float fPosY = (float)gGoldenArcher.iStartPosY;

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(260, fPosX, fPosY, 190.0f, 256.0f, (0.0f / 256.0f), (0.0f / 256.0f), (190.0f / 256.0f), (256.0f / 256.0f), true, true);

	fPosY += 256.0f;

	RenderBitmap(261, fPosX, fPosY, 190.0f, 177.0f, (0.0f / 256.0f), (0.0f / 256.0f), (190.0f / 256.0f), (177.0f / 256.0f), true, true);
}

void CGoldenArcher::RenderTitle()
{
	int StartPosX = gGoldenArcher.iStartPosX;
	int StartPosY = gGoldenArcher.iStartPosY;

	EnableAlphaTest(true);

	glColor3f(1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFontBold);

	SetBackgroundTextColor = Color4b(20, 20, 20, 255);

	SetTextColor = Color4b(230, 230, 230, 255);

	StartPosY += 12;

	// Render Title
	char Title[64];
	sprintf_s(Title, sizeof(Title), "%s", getMonsterName(236));

	RenderText(StartPosX + 35, CenterTextPosY(Title, StartPosY), Title, REAL_WIDTH(120), RT3_SORT_CENTER, NULL);

	StartPosY += 10;

	// Render SubTitle
	int TextNumbers[] = { 243, 244, 245, 246 };

	char SubTitle[256];
	sprintf_s(SubTitle, sizeof(SubTitle), "%s", GlobalText[TextNumbers[GoldenArcherOpenType - 1]]);
	RenderText(StartPosX + 22, CenterTextPosY(SubTitle, StartPosY), SubTitle, REAL_WIDTH(150), RT3_SORT_CENTER, NULL);
}

void CGoldenArcher::RenderRenaCollector()
{
	int iPosX = gGoldenArcher.iStartPosX;
	int iPosY = gGoldenArcher.iStartPosY + 50;

	float fBoxWidth = 120.0f;
	float fBoxHeight = 22.0f;

	// Initial Texts
	EnableAlphaTest(true);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 230, 210, 255);
	SelectObject(m_hFontDC, g_hFont);

	RenderText(iPosX, iPosY, GlobalText[882], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[883], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	// Explanation
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[218]);

	iPosY += 45;

	// Collected
	RenderBitmap(245, (float)iPosX + 13, (float)iPosY - 10.0f, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	RenderText(iPosX + 13, CenterTextPosY(GlobalText[884], iPosY), GlobalText[884], REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	iPosX += (190 - 13 - 75);

	// Registered
	RenderBitmap(245, (float)iPosX, (float)iPosY - 10.0f, 75.0f, 21.0f, (75.0f / 128.0f), (0.0f / 32.0f), (-75.0f / 128.0f), (21.0f / 32.0f), true, true);

	RenderText(iPosX, CenterTextPosY(GlobalText[885], iPosY), GlobalText[885], REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	iPosX = gGoldenArcher.iStartPosX;

	iPosY += 25;

	EnableAlphaTest(true);

	// Collected
	glColor3f(1.0f, 1.0f, 1.0f);

	RenderNumber2D((float)iPosX + 54.0f, (float)iPosY - 5.0f, gItemManager.GetInventoryItemCount(GET_ITEM(14, 21), 0), 8.0f, 8.0f);

	// Registered
	glColor3f(0.8f, 0.6f, 0.0f);

	RenderNumber2D((float)iPosX + 142.0f, (float)iPosY - 5.0f, GoldenArcherItemCount, 8.0f, 8.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	gItemManager.MyRenderItem3D((float)iPosX + 90.0f, (float)iPosY - 5.0f, 10.0f, 10.0f, &this->m_Rena, false);

	iPosY += 15;

	// Register Rena button
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[886], (gItemManager.GetInventoryItemSlot(GET_ITEM(14, 21), 0) != -1));

	iPosY += 30;

	// Divisor
	RenderBitmap(279, (float)iPosX, (float)iPosY, 190.0f, 10.0f, (0.0f / 256.0f), (0.0f / 16.0f), (190.0f / 256.0f), (10.0f / 16.0f), true, true);

	iPosY += 20;

	// Other texts
	char szSeparatedLines[7][38];

	int iLineCount = SeparateTextIntoLines(
		GlobalText[887],
		szSeparatedLines[0],
		7,
		38
	);

	for (int i = 0; i < iLineCount; i++)
	{
		RenderText(iPosX, iPosY, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

		iPosY += 15;
	}

	iPosY += 5;

	char Buffer[256];

	// Exchange renas button
	for (int i = GOLDEN_ARCHER_EXCHANGE_RENA1; i < GOLDEN_ARCHER_MAX_EXCHANGE_RENAS; i++)
	{
		if (this->m_RenaExchangeRequiredCount[i] > 0)
		{
			sprintf_s(Buffer, sizeof(Buffer), GlobalText[888], this->m_RenaExchangeRequiredCount[i]);

			this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, Buffer, (GoldenArcherItemCount >= (int)this->m_RenaExchangeRequiredCount[i]));

			iPosY += 30;
		}
	}
}

bool CGoldenArcher::CheckRenaCollector()
{
	int iPosX = gGoldenArcher.iStartPosX + 35;
	int iPosY = gGoldenArcher.iStartPosY + 80;
	int iBoxWidth = 120;
	int iBoxHeight = 22;

	// Explanation
	if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			CreateDialogInterface(713, 5); // Call explanation interface
		}

		return true;
	}

	iPosY += 85;

	// Register Rena
	if (gItemManager.GetInventoryItemSlot(GET_ITEM(14, 21), 0) != -1)
	{
		if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->CGGoldenArcherRegisterSend(GOLDEN_ARCHER_RENA_COLLECTOR);
			}

			return true;
		}
	}

	iPosY += 50;

	// Other texts
	char szSeparatedLines[7][38];

	int iLineCount = SeparateTextIntoLines(
		GlobalText[887],
		szSeparatedLines[0],
		7,
		38
	);

	iPosY += (15 * iLineCount);

	iPosY += 5;

	// Exchange renas button
	for (int i = GOLDEN_ARCHER_EXCHANGE_RENA1; i < GOLDEN_ARCHER_MAX_EXCHANGE_RENAS; i++)
	{
		if (this->m_RenaExchangeRequiredCount[i] > 0)
		{
			if (GoldenArcherItemCount >= (int)this->m_RenaExchangeRequiredCount[i])
			{
				if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
				{
					if (MouseLButton && MouseLButtonPush)
					{
						MouseLButtonPush = false;

						MouseUpdateTime = 0;

						MouseUpdateTimeMax = 6;

						PlayBuffer(25, 0, 0);

						this->CGGoldenArcherExchangeSend(GOLDEN_ARCHER_RENA_COLLECTOR, i);
					}

					return true;
				}
			}

			iPosY += 30;
		}
	}

	return false;
}

void CGoldenArcher::RenderStoneCollector()
{
	int iPosX = gGoldenArcher.iStartPosX;
	int iPosY = gGoldenArcher.iStartPosY + 50;

	float fBoxWidth = 120.0f;
	float fBoxHeight = 22.0f;

	// Initial Texts
	EnableAlphaTest(true);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 230, 210, 255);
	SelectObject(m_hFontDC, g_hFont);

	RenderText(iPosX, iPosY, GlobalText[889], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[890], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	// Explanation
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[218]);

	iPosY += 45;

	// Collected
	RenderBitmap(245, (float)iPosX + 13, (float)iPosY - 10.0f, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	RenderText(iPosX + 13, CenterTextPosY(GlobalText[891], iPosY), GlobalText[891], REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	iPosX += (190 - 13 - 75);

	// Registered
	RenderBitmap(245, (float)iPosX, (float)iPosY - 10.0f, 75.0f, 21.0f, (75.0f / 128.0f), (0.0f / 32.0f), (-75.0f / 128.0f), (21.0f / 32.0f), true, true);

	RenderText(iPosX, CenterTextPosY(GlobalText[892], iPosY), GlobalText[892], REAL_WIDTH(75), RT3_SORT_CENTER, NULL);

	iPosX = gGoldenArcher.iStartPosX;

	iPosY += 25;

	EnableAlphaTest(true);

	// Collected
	glColor3f(1.0f, 1.0f, 1.0f);

	RenderNumber2D((float)iPosX + 54.0f, (float)iPosY - 5.0f, gItemManager.GetInventoryItemCount(GET_ITEM(14, 21), 1), 8.0f, 8.0f);

	// Registered
	glColor3f(0.8f, 0.6f, 0.0f);

	RenderNumber2D((float)iPosX + 142.0f, (float)iPosY - 5.0f, GoldenArcherItemCount, 8.0f, 8.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	gItemManager.MyRenderItem3D((float)iPosX + 85.0f, (float)iPosY - 5.0f, 17.0f, 17.0f, &this->m_Stone, false);

	iPosY += 15;

	// Register Stone button
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[893], (gItemManager.GetInventoryItemSlot(GET_ITEM(14, 21), 1) != -1));

	iPosY += 30;

	// Divisor
	RenderBitmap(279, (float)iPosX, (float)iPosY, 190.0f, 10.0f, (0.0f / 256.0f), (0.0f / 16.0f), (190.0f / 256.0f), (10.0f / 16.0f), true, true);

	iPosY += 20;

	if (this->m_CurrentPage == 1)
	{
		this->RenderStoneCollector_Page1(iPosX, iPosY, fBoxWidth, fBoxHeight);
	}
	else if (this->m_CurrentPage == 2)
	{
		this->RenderStoneCollector_Page2(iPosX, iPosY, fBoxWidth, fBoxHeight);
	}

	if (this->m_TotalPages > 1)
	{
		this->RenderPagination();
	}
}

void CGoldenArcher::RenderStoneCollector_Page1(int iPosX, int iPosY, float fBoxWidth, float fBoxHeight)
{
	char Buffer[256];

	// Exchange stones vip button
	for (int i = GOLDEN_ARCHER_EXCHANGE_STONE1; i < GOLDEN_ARCHER_EXCHANGE_STONE4; i++)
	{
		if (this->m_StoneExchangeRequiredCount[i] > 0)
		{
			sprintf_s(
				Buffer,
				sizeof(Buffer),
				GlobalText[894],
				this->m_StoneExchangeRequiredCount[i],
				this->m_StoneExchangeVipDuration[i],
				(i + 1));

			RenderText(iPosX, iPosY, Buffer, REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			iPosY += 10;

			sprintf_s(Buffer, sizeof(Buffer), GlobalText[895], this->m_StoneExchangeRequiredCount[i]);

			this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, Buffer, (GoldenArcherItemCount >= (int)this->m_StoneExchangeRequiredCount[i]));

			iPosY += 35;
		}
	}
}

void CGoldenArcher::RenderStoneCollector_Page2(int iPosX, int iPosY, float fBoxWidth, float fBoxHeight)
{
	// Exchange stones lucky number button
	bool isValidLuckyNumber = (this->m_LuckyNumber[0] != '\0');

	char Buffer[256];

	if (!isValidLuckyNumber)
	{
		sprintf_s(Buffer, sizeof(Buffer), GlobalText[896], this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4]);

		RenderText(iPosX, iPosY, Buffer, REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

		iPosY += 10;

		sprintf_s(Buffer, sizeof(Buffer), GlobalText[895], this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4]);

		this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, Buffer, (GoldenArcherItemCount >= (int)this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4]));

		iPosY += 40;

		char szSeparatedLines[7][38];

		int iLineCount = SeparateTextIntoLines(
			GlobalText[897],
			szSeparatedLines[0],
			7,
			38
		);

		for (int i = 0; i < iLineCount; i++)
		{
			RenderText(iPosX, iPosY, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			iPosY += 15;
		}

		SetTextColor = Color4b(255, 255, 255, 255);

		SelectObject(m_hFontDC, g_hFont);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else
	{
		RenderText(iPosX, iPosY, GlobalText[898], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

		iPosY += 10;

		SetTextColor = Color4b(212, 150, 0, 255);
		SelectObject(m_hFontDC, g_hFontBig);

		sprintf_s(Buffer, sizeof(Buffer), "%c%c%c%c - %c%c%c%c - %c%c%c%c",
			  this->m_LuckyNumber[0], this->m_LuckyNumber[1], this->m_LuckyNumber[2], this->m_LuckyNumber[3],
			  this->m_LuckyNumber[4], this->m_LuckyNumber[5], this->m_LuckyNumber[6], this->m_LuckyNumber[7],
			  this->m_LuckyNumber[8], this->m_LuckyNumber[9], this->m_LuckyNumber[10], this->m_LuckyNumber[11]);

		RenderText(iPosX, iPosY, Buffer, REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

		SetTextColor = Color4b(255, 230, 210, 255);
		SelectObject(m_hFontDC, g_hFont);

		iPosY += 30;

		// Register Lucky Number button
		this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[899]);

		iPosY += 40;

		char szSeparatedLines[7][38];

		int iLineCount = SeparateTextIntoLines(
			GlobalText[897],
			szSeparatedLines[0],
			7,
			38
		);

		for (int i = 0; i < iLineCount; i++)
		{
			RenderText(iPosX, iPosY, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			iPosY += 15;
		}
	}
}

bool CGoldenArcher::CheckStoneCollector()
{
	int iPosX = gGoldenArcher.iStartPosX + 35;
	int iPosY = gGoldenArcher.iStartPosY + 80;
	int iBoxWidth = 120;
	int iBoxHeight = 22;

	// Explanation
	if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			CreateDialogInterface(713, 5); // Call explanation interface
		}

		return true;
	}

	iPosY += 85;

	// Register Stone
	if (gItemManager.GetInventoryItemSlot(GET_ITEM(14, 21), 1) != -1)
	{
		if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->CGGoldenArcherRegisterSend(GOLDEN_ARCHER_STONE_COLLECTOR);
			}

			return true;
		}
	}

	iPosY += 60;

	if (this->m_CurrentPage == 1) // Exchange stones vip button
	{
		if (this->CheckStoneCollector_Page1(iPosX, iPosY, iBoxWidth, iBoxHeight))
		{
			return true;
		}
	}
	else if (this->m_CurrentPage == 2) // Exchange stones lucky number button
	{
		if (this->CheckStoneCollector_Page2(iPosX, iPosY, iBoxWidth, iBoxHeight))
		{
			return true;
		}
	}

	if (this->m_TotalPages > 1 && this->CheckPagination())
	{
		return true;
	}

	return false;
}

bool CGoldenArcher::CheckStoneCollector_Page1(int iPosX, int iPosY, int iBoxWidth, int iBoxHeight)
{
	for (int i = GOLDEN_ARCHER_EXCHANGE_STONE1; i < GOLDEN_ARCHER_EXCHANGE_STONE4; i++)
	{
		if (this->m_StoneExchangeRequiredCount[i] > 0)
		{
			if (GoldenArcherItemCount >= (int)this->m_StoneExchangeRequiredCount[i])
			{
				if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
				{
					if (MouseLButton && MouseLButtonPush)
					{
						MouseLButtonPush = false;

						MouseUpdateTime = 0;

						MouseUpdateTimeMax = 6;

						PlayBuffer(25, 0, 0);

						this->CGGoldenArcherExchangeSend(GOLDEN_ARCHER_STONE_COLLECTOR, i);
					}

					return true;
				}
			}

			iPosY += 45;
		}
	}

	return false;
}

bool CGoldenArcher::CheckStoneCollector_Page2(int iPosX, int iPosY, int iBoxWidth, int iBoxHeight)
{
	// Exchange stones lucky number button
	if (this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] > 0)
	{
		if (this->m_LuckyNumber[0] == '\0')
		{
			if (GoldenArcherItemCount >= (int)this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4])
			{
				if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
				{
					if (MouseLButton && MouseLButtonPush)
					{
						MouseLButtonPush = false;

						MouseUpdateTime = 0;

						MouseUpdateTimeMax = 6;

						PlayBuffer(25, 0, 0);

						this->CGGoldenArcherExchangeSend(GOLDEN_ARCHER_STONE_COLLECTOR, GOLDEN_ARCHER_EXCHANGE_STONE4);
					}

					return true;
				}
			}
		}
		else
		{
			iPosY += 30;

			if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
			{
				if (MouseLButton && MouseLButtonPush)
				{
					MouseLButtonPush = false;

					MouseUpdateTime = 0;

					MouseUpdateTimeMax = 6;

					PlayBuffer(25, 0, 0);

					this->CGGoldenArcherRegisterLuckySend(this->m_LuckyNumber);
				}

				return true;
			}
		}
	}

	return false;
}

void CGoldenArcher::RenderLuckyNumberExchanger()
{
	int iPosX = gGoldenArcher.iStartPosX;
	int iPosY = gGoldenArcher.iStartPosY + 50;

	float fBoxWidth = 120.0f;
	float fBoxHeight = 22.0f;

	// Initial Texts
	EnableAlphaTest(true);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 230, 210, 255);
	SelectObject(m_hFontDC, g_hFont);

	RenderText(iPosX, iPosY, GlobalText[904], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[905], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	// Explanation
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[218]);

	iPosY += 30;

	// Divisor
	RenderBitmap(279, (float)iPosX, (float)iPosY, 190.0f, 10.0f, (0.0f / 256.0f), (0.0f / 16.0f), (190.0f / 256.0f), (10.0f / 16.0f), true, true);

	iPosY += 20;

	// Other texts
	RenderText(iPosX, iPosY, GlobalText[906], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	SetTextColor = Color4b(223, 191, 103, 255);
	SelectObject(m_hFontDC, g_hFontBold);

	RenderText(iPosX, iPosY, GlobalText[907], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	SelectObject(m_hFontDC, g_hFont);

	RenderText(iPosX, iPosY, GlobalText[908], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[909], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[910], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 20;

	// Lucky Number Input
	RenderBitmap(271, (float)iPosX + 35.0f, (float)iPosY, fBoxWidth, 18.0f, (0.0f / 128.0f), (0.0f / 32.0f), (113.0f / 128.0f), (18.0f / 32.0f), true, true);

	InputTextWidth = REAL_WIDTH(110);

	RenderInputText(iPosX + 40, iPosY + 9, 0);

	InputTextWidth = 256;

	iPosY += 30;

	// Register Lucky Number Button
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[906], (strlen(InputText[InputIndex]) > 0));

	if (strlen(GoldenArcherLuckyNumberText) > 0)
	{
		iPosY += 30;

		char szSeparatedLines[7][38];

		int iLineCount = SeparateTextIntoLines(GoldenArcherLuckyNumberText, szSeparatedLines[0], 7, 38);

		for (int i = 0; i < iLineCount; i++)
		{
			glColor3f(0.0f, 0.0f, 0.0f);

			SelectObject(m_hFontDC, g_hFontBold);

			RenderText(iPosX + 1, iPosY + 1, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			if (sin(WorldTime * 0.01f) < 0)
			{
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else
			{
				glColor3f(0.6f, 0.6f, 0.6f);
			}

			SetTextColor = Color4b(255, 210, 103, 255);

			RenderText(iPosX, iPosY, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			iPosY += 15;
		}

		SetTextColor = Color4b(255, 255, 255, 255);

		SelectObject(m_hFontDC, g_hFont);

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

bool CGoldenArcher::CheckLuckyNumberExchanger()
{
	int iPosX = gGoldenArcher.iStartPosX + 35;
	int iPosY = gGoldenArcher.iStartPosY + 80;
	int iBoxWidth = 120;
	int iBoxHeight = 22;

	// Explanation
	if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			CreateDialogInterface(713, 5); // Call explanation interface
		}

		return true;
	}

	iPosY += 160;

	// Register Lucky Number
	if (strlen(InputText[InputIndex]) > 0)
	{
		if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->CGGoldenArcherExchangeLuckySend(InputText[InputIndex]);
			}

			return true;
		}
	}

	return false;
}

std::vector<std::string> getPage(const std::vector<std::string>& data, int pageNumber)
{
	std::vector<std::string> page;

	int startIndex = (pageNumber - 1) * 10;

	int endIndex = startIndex + 10;

	if (startIndex >= static_cast<int>(data.size()))
	{
		return page;
	}

	endIndex = min(endIndex, static_cast<int>(data.size()));

	for (int i = startIndex; i < endIndex; i++)
	{
		page.push_back(data[i]);
	}

	return page;
}

void CGoldenArcher::RenderLuckyNumberList()
{
	int iPosX = gGoldenArcher.iStartPosX;
	int iPosY = gGoldenArcher.iStartPosY + 50;

	float fBoxWidth = 120.0f;
	float fBoxHeight = 22.0f;

	// Initial Texts
	EnableAlphaTest(true);
	SetBackgroundTextColor = Color4b(255, 255, 255, 0);
	SetTextColor = Color4b(255, 230, 210, 255);
	SelectObject(m_hFontDC, g_hFont);

	RenderText(iPosX, iPosY, GlobalText[915], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	RenderText(iPosX, iPosY, GlobalText[916], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 15;

	// Explanation
	this->RenderBox((float)iPosX + 35, (float)iPosY, fBoxWidth, fBoxHeight, GlobalText[218]);

	iPosY += 30;

	// Divisor
	RenderBitmap(279, (float)iPosX, (float)iPosY, 190.0f, 10.0f, (0.0f / 256.0f), (0.0f / 16.0f), (190.0f / 256.0f), (10.0f / 16.0f), true, true);

	if (this->m_MyLuckyNumbers.empty())
	{
		iPosY = gGoldenArcher.iStartPosY + 216;

		SelectObject(m_hFontDC, g_hFontBold);

		SetBackgroundTextColor = Color4b(255, 255, 255, 0);

		char szSeparatedLines[7][38];

		int iLineCount = SeparateTextIntoLines(
			GlobalText[897],
			szSeparatedLines[0],
			7,
			38
		);

		for (int i = 0; i < iLineCount; i++)
		{
			RenderText(iPosX, iPosY, szSeparatedLines[i], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

			iPosY += 15;
		}

		SetTextColor = Color4b(255, 230, 210, 255);

		SelectObject(m_hFontDC, g_hFont);

		return;
	}

	iPosY += 20;

	RenderText(iPosX, iPosY, GlobalText[917], REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosY += 20;

	SetTextColor = Color4b(212, 150, 0, 255);
	SelectObject(m_hFontDC, g_hFontBig);

	bool invert = true;

	char Text[64] = { '\0' };

	std::vector<std::string> numbers = getPage(this->m_MyLuckyNumbers, this->m_CurrentPage);

	for (std::string Number : numbers)
	{
		if (invert)
		{
			SetBackgroundTextColor = Color4b(0, 0, 0, 128);
		}
		else
		{
			SetBackgroundTextColor = Color4b(255, 255, 255, 0);
		}

		sprintf_s(Text, sizeof(Text), "%c%c%c%c - %c%c%c%c - %c%c%c%c",
			  Number[0], Number[1], Number[2], Number[3],
			  Number[4], Number[5], Number[6], Number[7],
			  Number[8], Number[9], Number[10], Number[11]);

		RenderText(iPosX + 13, iPosY, Text, REAL_WIDTH(190 - 26), RT3_SORT_CENTER, NULL);

		iPosY += 20;

		invert ^= 1;
	}

	if (this->m_TotalPages > 1)
	{
		this->RenderPagination();
	}
}

bool CGoldenArcher::CheckLuckyNumberList()
{
	int iPosX = gGoldenArcher.iStartPosX + 35;
	int iPosY = gGoldenArcher.iStartPosY + 80;
	int iBoxWidth = 120;
	int iBoxHeight = 22;

	// Explanation
	if (IsWorkZone(iPosX, iPosY, iBoxWidth, iBoxHeight))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			PlayBuffer(25, 0, 0);

			CreateDialogInterface(713, 5); // Call explanation interface
		}

		return true;
	}

	if (this->m_TotalPages > 1 && this->CheckPagination())
	{
		return true;
	}

	return false;
}

void CGoldenArcher::RenderCloseButton()
{
	int iPosX = gGoldenArcher.iStartPosX + 25;
	int iPosY = gGoldenArcher.iStartPosY + 395;

	glColor3f(1.0f, 1.0f, 1.0f);

	RenderBitmap(280, (float)iPosX, (float)iPosY, 24.0f, 24.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);

	if (IsWorkZone(iPosX, iPosY, 24, 24))
	{
		SelectObject(m_hFontDC, g_hFont);

		SetBackgroundTextColor = Color4b(255, 255, 255, 0);

		SetTextColor = Color4b(255, 255, 255, 255);

		RenderTipText(iPosX + 2, iPosY - 15, GlobalText[247]);
	}
}

bool CGoldenArcher::CheckCloseButton()
{
	int iPosX = gGoldenArcher.iStartPosX + 25;
	int iPosY = gGoldenArcher.iStartPosY + 395;

	if (IsWorkZone(iPosX, iPosY, 24, 24))
	{
		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			this->GoldenArcherCloseProc();

			gGoldenArcher.CGGoldenArcherCloseSend();
		}

		return true;
	}

	return false;
}

void CGoldenArcher::RenderBox(float PosX, float PosY, float Width, float Height, const char* Text, bool Hoverable)
{
	if (!Hoverable)
	{
		DisableAlphaBlend();

		glColor3f(0.4f, 0.4f, 0.4f);

		RenderBitmap(240, PosX, PosY, Width, Height, (0.0f / 256.0f), (0.0f / 64.0f), (213.0f / 256.0f), (64.0f / 64.0f), true, true);
	}
	else
	{
		DisableAlphaBlend();

		glColor3f(1.0f, 1.0f, 1.0f);

		RenderBitmap(240, PosX, PosY, Width, Height, (0.0f / 256.0f), (0.0f / 64.0f), (213.0f / 256.0f), (64.0f / 64.0f), true, true);

		if (IsWorkZone((int)PosX, (int)PosY, (int)Width, (int)Height))
		{
			glColor3f(0.8f, 0.6f, 0.4f);

			EnableAlphaBlend();

			RenderBitmap(240, PosX, PosY, Width, Height, (0.0f / 256.0f), (0.0f / 64.0f), (213.0f / 256.0f), (64.0f / 64.0f), true, true);

			glColor3f(1.0f, 1.0f, 1.0f);

			DisableAlphaBlend();
		}
	}

	EnableAlphaTest(true);

	RenderText((int)PosX, CenterTextPosY(Text, (int)(PosY + (Height / 2))), Text, REAL_WIDTH((int)Width), RT3_SORT_CENTER, NULL);

	glColor3f(1.0f, 1.0f, 1.0f);
}

void CGoldenArcher::RenderPagination()
{
	int iPosX = gGoldenArcher.iStartPosX;

	int iPosY = gGoldenArcher.iStartPosY + 375;

	float fButtonSize = 19.0f;

	EnableAlphaTest(true);

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);

	SetTextColor = Color4b(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFontBold);

	char Text[32] = { '\0' };
	sprintf_s(Text, sizeof(Text), "%d/%d", this->m_CurrentPage, this->m_TotalPages);

	RenderText(gGoldenArcher.iStartPosX, CenterTextPosY(Text, (int)(iPosY + (fButtonSize / 2))), Text, REAL_WIDTH(190), RT3_SORT_CENTER, NULL);

	iPosX += 95;

	if (this->m_CurrentPage > 1)
	{
		if (IsWorkZone(iPosX - 40, iPosY, (int)fButtonSize, (int)fButtonSize))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, (float)iPosX - 40.0f, (float)iPosY, fButtonSize, fButtonSize, (0.0f / 32.0f), (0.0f / 32.0f), (32.0f / 32.0f), (32.0f / 32.0f), true, true);
			}
			else
			{
				RenderBitmap(0xFF, (float)iPosX - 40.0f, (float)iPosY, fButtonSize, fButtonSize, (0.0f / 32.0f), (0.0f / 32.0f), (32.0f / 32.0f), (32.0f / 32.0f), true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, (float)iPosX - 40.0f, (float)iPosY, fButtonSize, fButtonSize, (0.0f / 32.0f), (0.0f / 32.0f), (32.0f / 32.0f), (32.0f / 32.0f), true, true);
		}
	}

	if (this->m_CurrentPage < this->m_TotalPages)
	{
		if (IsWorkZone(iPosX + 21, iPosY, (int)fButtonSize, (int)fButtonSize))
		{
			if (MouseLButtonPush)
			{
				RenderBitmap(0x100, (float)iPosX + 21.0f, (float)iPosY, fButtonSize, fButtonSize, (32.0f / 32.0f), (0.0f / 32.0f), (-32.0f / 32.0f), (32.0f / 32.0f), true, true);
			}
			else
			{
				RenderBitmap(0xFF, (float)iPosX + 21.0f, (float)iPosY, fButtonSize, fButtonSize, (32.0f / 32.0f), (0.0f / 32.0f), (-32.0f / 32.0f), (32.0f / 32.0f), true, true);
			}
		}
		else
		{
			RenderBitmap(0xFE, (float)iPosX + 21.0f, (float)iPosY, fButtonSize, fButtonSize, (32.0f / 32.0f), (0.0f / 32.0f), (-32.0f / 32.0f), (32.0f / 32.0f), true, true);
		}
	}

	SetBackgroundTextColor = Color4b(255, 255, 255, 0);

	SetTextColor = Color4b(255, 255, 255, 255);

	SelectObject(m_hFontDC, g_hFont);
}

bool CGoldenArcher::CheckPagination()
{
	int iPosX = gGoldenArcher.iStartPosX + 95;

	int iPosY = gGoldenArcher.iStartPosY + 375;

	int iButtonSize = 19;

	if (this->m_CurrentPage > 1)
	{
		if (IsWorkZone(iPosX - 40, iPosY, iButtonSize, iButtonSize))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->m_CurrentPage--;
			}

			return true;
		}
	}

	if (this->m_CurrentPage < this->m_TotalPages)
	{
		if (IsWorkZone(iPosX + 21, iPosY, iButtonSize, iButtonSize))
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->m_CurrentPage++;
			}

			return true;
		}
	}

	return false;
}

_declspec(naked) void CGoldenArcher::MoveCharacter_GoldenArcherMovement()
{
	static DWORD jmpOn1 = 0x0044A370;
	static DWORD jmpOn2 = 0x0044A390;
	static DWORD jmpOnNot = 0x0044A3B2;
	static DWORD c;

	_asm
	{
		Mov Ecx, Dword Ptr Ds : [Ebp + 0xC] ;
		Mov c, Ecx;
		Pushad;
	}

	if (*(BYTE*)(c + 0x84) != 4 || *(WORD*)(c + 2) != 390)
	{
		goto EXIT;
	}

	if (*(int*)(c + 4) < 206 || *(int*)(c + 4) > 208)
	{
		goto EXIT;
	}

	if (World == MAP_LORENCIA || World == MAP_NORIA)
	{
		_asm
		{
			Popad;
			Jmp[jmpOn1];
		}
	}
	else if (World == MAP_DEVIAS)
	{
		_asm
		{
			Popad;
			Jmp[jmpOn2];
		}
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CGoldenArcher::MoveCharacter_GoldenArcherChatMessages()
{
	static DWORD jmpBack = 0x0044AA6B;
	static int TextIndex;
	static int action;
	static DWORD c;

	_asm
	{
		Pushad;
		Lea Eax, Dword Ptr Ds : [Ebp - 0xBC] ;
		Mov TextIndex, Eax;
		Mov Eax, Dword Ptr Ds : [Ebp - 0xB8] ;
		Mov action, Eax;
		Mov Eax, Dword Ptr Ds : [Ebp + 0x8] ;
		Mov c, Eax;
	}

	if (action < 80)
	{
		SetAction(c, 1);
	}
	else if (action < 85)
	{
		SetAction(c, 97); // clap
	}
	else if (action < 90)
	{
		SetAction(c, 99); // cheer
	}
	else if (action < 95)
	{
		SetAction(c, 111); // See
	}
	else if (action < 100)
	{
		SetAction(c, 105);
	}

	// if (o->CurrentAction != o->PriorAction)
	if (*(BYTE*)(c + 261) != *(BYTE*)(c + 262))
	{
		if (World == MAP_LORENCIA || World == MAP_NORIA)
		{
			static int TextIndexes[] = { 700, 701, 702, 703, 704 };

			TextIndex = TextIndexes[(rand() % (sizeof(TextIndexes) / sizeof(TextIndexes[0])))];

			CreateChat((char*)(c + 0x1C1), GlobalText[TextIndex], c, 0, -1);
		}
		else if (World == MAP_DEVIAS)
		{
			static int TextIndexes[] = { 818, 819, 820, 821, 822, 823 };

			TextIndex = TextIndexes[(rand() % (sizeof(TextIndexes) / sizeof(TextIndexes[0])))];

			CreateChat((char*)(c + 0x1C1), GlobalText[TextIndex], c, 0, -1);
		}
	}

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

/**********************************************/
//***************** Protocols *****************/
/**********************************************/

void CGoldenArcher::GCGoldenArcherInfoRecv(PMSG_GOLDEN_ARCHER_INFO_RECV* lpMsg)
{
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1] = lpMsg->RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA1];
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2] = lpMsg->RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA2];
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3] = lpMsg->RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA3];
	this->m_RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4] = lpMsg->RenaExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_RENA4];

	// Stone exchanger VIP
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1] = lpMsg->StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE1];
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1] = lpMsg->StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE1];

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2] = lpMsg->StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE2];
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2] = lpMsg->StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE2];

	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3] = lpMsg->StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE3];
	this->m_StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3] = lpMsg->StoneExchangeVipDuration[GOLDEN_ARCHER_EXCHANGE_STONE3];

	// Stone exchanger Lucky Number
	this->m_StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4] = lpMsg->StoneExchangeRequiredCount[GOLDEN_ARCHER_EXCHANGE_STONE4];
}

void CGoldenArcher::GCGoldenArcherRecv(PMSG_NPC_GOLDEN_ARCHER_RECV* lpMsg)
{
	((void(_cdecl*)()) 0x004CBA60)(); // Close inventory related windows

	InventoryOpened = false;

	CharacterOpened = false;

	GoldenArcherOpenType = lpMsg->Type + 1;

	GoldenArcherItemCount = lpMsg->Count;

	memcpy(this->m_LuckyNumber, lpMsg->LuckyNumber, sizeof(this->m_LuckyNumber));

	this->m_CurrentPage = 1;

	this->m_TotalPages = 2;

	if (lpMsg->Type == GOLDEN_ARCHER_LUCKY_NUMBER_EXCHANGE)
	{
		memset(GoldenArcherLuckyNumberText, 0, 64);

		ClearInput(FALSE);

		InputTextMax[0] = 12;

		InputNumber = 1;

		InputEnable = false;

		GoldInputEnable = false;

		InputGold = 0;

		StorageGoldFlag = 0;

		GoldenArcherLuckyNumberTicket = true;
	}
}

void CGoldenArcher::GCGoldenArcherListRecv(PMSG_GOLDEN_ARCHER_LIST_RECV* lpMsg)
{
	((void(_cdecl*)()) 0x004CBA60)(); // Close inventory related windows

	InventoryOpened = false;

	CharacterOpened = false;

	GoldenArcherOpenType = GOLDEN_ARCHER_INTERFACE_BINGO;

	this->m_CurrentPage = 1;

	this->m_TotalPages = (lpMsg->count / 10) + 1;

	this->m_MyLuckyNumbers.clear();

	for (int i = 0; i < lpMsg->count; i++)
	{
		LUCKY_NUMBER_INFO* lpInfo = (LUCKY_NUMBER_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_GOLDEN_ARCHER_LIST_RECV) + (sizeof(LUCKY_NUMBER_INFO) * i));

		this->m_MyLuckyNumbers.push_back(lpInfo->LuckyNumber);
	}
}

void CGoldenArcher::CGGoldenArcherRegisterSend(int Type)
{
	PMSG_GOLDEN_ARCHER_REGISTER_SEND pMsg;

	pMsg.header.set(0x95, sizeof(pMsg));

	pMsg.Type = Type;

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::CGGoldenArcherExchangeSend(int Type, int Count)
{
	PMSG_GOLDEN_ARCHER_EXCHANGE_SEND pMsg;

	pMsg.header.set(0x97, 0x02, sizeof(pMsg));

	pMsg.Type = Type;

	pMsg.Count = Count;

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::GCGoldenArcherRegisterRecv(PMSG_GOLDEN_ARCHER_REGISTER_RECV* lpMsg)
{
	GoldenArcherItemCount = lpMsg->Count;
}

void CGoldenArcher::GCGoldenArcherLuckyNumberRecv(PMSG_GOLDEN_ARCHER_LUCKY_NUMBER_RECV* lpMsg)
{
	memcpy(this->m_LuckyNumber, lpMsg->LuckyNumber, sizeof(this->m_LuckyNumber));
}

void CGoldenArcher::CGGoldenArcherRegisterLuckySend(const char* LuckyNumber)
{
	PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_SEND pMsg;

	pMsg.header.set(0x97, 0x03, sizeof(pMsg));

	memcpy(pMsg.LuckyNumber, LuckyNumber, sizeof(pMsg.LuckyNumber));

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::GCGoldenArcherRegisterLuckyRecv(PMSG_GOLDEN_ARCHER_REGISTER_LUCKY_RECV* lpMsg)
{
	if (lpMsg->Result >= 0 && lpMsg->Result < 4)
	{
		CreateOkMessageBox(GlobalText[900 + lpMsg->Result]);
	}
	else
	{
		CreateOkMessageBox(GlobalText[901]);
	}
}

void CGoldenArcher::CGGoldenArcherExchangeLuckySend(const char* LuckyNumber)
{
	PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_SEND pMsg;

	pMsg.header.set(0x9D, sizeof(pMsg));

	memcpy(pMsg.LuckyNumber, LuckyNumber, sizeof(pMsg.LuckyNumber));

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CGoldenArcher::GCGoldenArcherExchangeLuckyRecv(PMSG_GOLDEN_ARCHER_EXCHANGE_LUCKY_RECV* lpMsg)
{
	char Text[300] = { '\0' };

	if (lpMsg->Result >= 0 && lpMsg->Result < 4)
	{
		sprintf_s(Text, sizeof(Text), GlobalText[911 + lpMsg->Result]);
	}
	else
	{
		sprintf_s(Text, sizeof(Text), GlobalText[901]);
	}

	CreateOkMessageBox(Text);

	memcpy(GoldenArcherLuckyNumberText, Text, 64);
}

void CGoldenArcher::GoldenArcherCloseProc()
{
	((void(_cdecl*)()) 0x004CBA60)(); // Close inventory related windows

	InventoryOpened = false;

	CharacterOpened = false;

	ClearInput(FALSE);

	InputEnable = false;

	GoldInputEnable = false;

	InputGold = 0;

	StorageGoldFlag = 0;

	GoldenArcherLuckyNumberTicket = false;
}

void CGoldenArcher::CGGoldenArcherCloseSend()
{
	PBMSG_HEAD pMsg;

	pMsg.set(0x31, sizeof(pMsg));

	gProtocol.DataSend((BYTE*)&pMsg, pMsg.size);
}