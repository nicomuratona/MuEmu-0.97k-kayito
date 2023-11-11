#include "stdafx.h"
#include "FullMap.h"
#include "Protect.h"

CFullMap gFullMap;

CFullMap::CFullMap()
{
	this->FullMapSwitch = false;

	this->ZoomLevel = GetPrivateProfileInt("FullMap", "ZoomLevel", 0, ".\\Config.ini");

	this->MaxSizeX = 640.0f;
	this->MaxSizeY = 432.0f;

	this->VisualRangeX = 128 - (this->ZoomLevel * 16);
	this->VisualRangeY = 112 - (this->ZoomLevel * 16);

	this->ScaleX = this->MaxSizeX / (this->VisualRangeX * 2);
	this->ScaleY = this->MaxSizeY / (this->VisualRangeY * 2);

	this->StartPosX = ImgCenterScreenPosX(this->MaxSizeX) + (this->ScaleX / 3);
	this->StartPosY = ImgCenterScreenPosY(this->MaxSizeY) - (this->ScaleY);

	this->CharPosX = 1;
	this->CharPosY = 1;

	this->MinScaleOffsetX = 1;
	this->MaxScaleOffsetX = 1;

	this->MinScaleOffsetY = 1;
	this->MaxScaleOffsetY = 1;

	this->FullMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");
}

CFullMap::~CFullMap()
{
	char Text[33] = { 0 };

	wsprintf(Text, "%d", this->ZoomLevel);

	WritePrivateProfileString("FullMap", "ZoomLevel", Text, ".\\Config.ini");
}

void CFullMap::Init()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	SetCompleteHook(0xE8, 0x004BD300, &this->RenderFullMap);

	SetCompleteHook(0xE8, 0x004BD332, &this->RenderZoom);
}

void CFullMap::Toggle()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	if (InputEnable || TabInputEnable || GoldInputEnable || GuildInputEnable)
	{
		return;
	}

	if (!gFullMap.CheckInterfaces())
	{
		this->FullMapSwitch = 0;

		return;
	}

	this->FullMapSwitch ^= 1;

	PlayBuffer(25, 0, 0);
}

void CFullMap::CheckZoomButton()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	if (!this->FullMapSwitch)
	{
		return;
	}

	if (!gFullMap.CheckInterfaces())
	{
		this->FullMapSwitch = 0;

		return;
	}

	if (IsWorkZone(75, 411, 21, 21))
	{
		MouseOnWindow = true;

		if (MouseLButton && MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			this->ChangeZoom();

			PlayBuffer(25, 0, 0);
		}
	}
}

void CFullMap::ChangeZoom()
{
	this->ZoomLevel = (this->ZoomLevel + 1 >= 7) ? 0 : this->ZoomLevel + 1;

	this->VisualRangeX = 128 - (this->ZoomLevel * 16);
	this->VisualRangeY = 112 - (this->ZoomLevel * 16);

	this->ScaleX = this->MaxSizeX / (this->VisualRangeX * 2);
	this->ScaleY = this->MaxSizeY / (this->VisualRangeY * 2);

	this->StartPosX = ImgCenterScreenPosX(this->MaxSizeX) + (this->ScaleX / 3);
	this->StartPosY = ImgCenterScreenPosY(this->MaxSizeY) - (this->ScaleY);

	DeleteObject((HGDIOBJ)this->FullMapFont);

	this->FullMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");
}

void CFullMap::RenderFullMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	gFullMap.CharPosX = *(float*)(Hero + 0x10) / 100.0f;

	gFullMap.CharPosY = *(float*)(Hero + 0x14) / 100.0f;

	gFullMap.MinScaleOffsetX = min(max((gFullMap.CharPosX - gFullMap.VisualRangeX), 0), 256.0f - (gFullMap.VisualRangeX * 2));

	gFullMap.MaxScaleOffsetX = max(min((gFullMap.CharPosX + gFullMap.VisualRangeX), 256.0f), (gFullMap.VisualRangeX * 2));

	gFullMap.MinScaleOffsetY = min(max((gFullMap.CharPosY - gFullMap.VisualRangeY), 0), 256.0f - (gFullMap.VisualRangeY * 2));

	gFullMap.MaxScaleOffsetY = max(min((gFullMap.CharPosY + gFullMap.VisualRangeY), 256.0f), (gFullMap.VisualRangeY * 2));

	if (gFullMap.FullMapSwitch)
	{
		gFullMap.RenderMapZone();

		gFullMap.RenderCharacters();
	}
	else
	{
		RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
	}
}

void CFullMap::RenderMapZone()
{
	int index;

	for (int sy = (int)this->MinScaleOffsetY; sy < (int)this->MaxScaleOffsetY; sy++)
	{
		for (int sx = (int)this->MinScaleOffsetX; sx < (int)this->MaxScaleOffsetX; sx++)
		{
			index = sy * 256 + sx;

			if ((TerrainWall[index] >> 2) & 0x1) // TW_NOMOVE
			{
				continue;
			}

			if ((TerrainWall[index] >> 3) & 0x1) // TW_NOGROUND
			{
				continue;
			}

			if ((TerrainWall[index] >> 4) & 0x1) // TW_WATER
			{
				continue;
			}

			if ((TerrainWall[index] >> 5) & 0x1) // TW_ACTION
			{
				continue;
			}

			if ((TerrainWall[index] >> 6) & 0x1) // TW_HEIGHT
			{
				continue;
			}

			if ((TerrainWall[index] >> 7) & 0x1) // TW_CAMERA_UP
			{
				continue;
			}

			if ((TerrainWall[index] == 0) // HUNT_ZONE
			    || ((TerrainWall[index] >> 0) & 0x1)) // TW_SAFEZONE
			{
				PaintBorder(sx, sy);

				glColor4f(0.0f, 0.0f, 0.0f, 0.8f);

				RenderColor
				(
					this->StartPosX + ((sx - this->MinScaleOffsetX) * this->ScaleX),
					(this->StartPosY + this->MaxSizeY) - ((sy - this->MinScaleOffsetY) * this->ScaleY),
					this->ScaleX,
					this->ScaleY
				);
			}
		}
	}

	glColor3f(1.0f, 1.0f, 1.0f);
}

void CFullMap::PaintBorder(int sx, int sy)
{
	float newScaleX = (this->ScaleX / 3.0f) * 2.0f;

	float newScaleY = (this->ScaleY / 3.0f) * 2.0f;

	if ((sx - 1) > 0)
	{
		if (((TerrainWall[sy * 256 + (sx - 1)] >> 0) & 0xFC) != 0)
		{
			glColor4f(0.87f, 0.98f, 0.99f, 0.5f);

			RenderColor
			(
				this->StartPosX + (((sx - 1) - this->MinScaleOffsetX) * this->ScaleX) + (newScaleX / 3),
				(this->StartPosY + this->MaxSizeY) - ((sy - this->MinScaleOffsetY) * this->ScaleY) + (newScaleY / 3),
				newScaleX,
				newScaleY
			);
		}
	}

	if ((sx + 1) < 256)
	{
		if (((TerrainWall[sy * 256 + (sx + 1)] >> 0) & 0xFC) != 0)
		{
			glColor4f(0.87f, 0.98f, 0.99f, 0.5f);

			RenderColor
			(
				this->StartPosX + (((sx + 1) - this->MinScaleOffsetX) * this->ScaleX) + (newScaleX / 3),
				(this->StartPosY + this->MaxSizeY) - ((sy - this->MinScaleOffsetY) * this->ScaleY) + (newScaleY / 3),
				newScaleX,
				newScaleY
			);
		}
	}

	if ((sy - 1) > 0)
	{
		if (((TerrainWall[(sy - 1) * 256 + sx] >> 0) & 0xFC) != 0)
		{
			glColor4f(0.87f, 0.98f, 0.99f, 0.5f);

			RenderColor
			(
				this->StartPosX + ((sx - this->MinScaleOffsetX) * this->ScaleX) + (newScaleX / 3),
				(this->StartPosY + this->MaxSizeY) - (((sy - 1) - this->MinScaleOffsetY) * this->ScaleY) + (newScaleY / 3),
				newScaleX,
				newScaleY
			);
		}
	}

	if ((sy + 1) < 256)
	{
		if (((TerrainWall[(sy + 1) * 256 + sx] >> 0) & 0xFC) != 0)
		{
			glColor4f(0.87f, 0.98f, 0.99f, 0.5f);

			RenderColor
			(
				this->StartPosX + ((sx - this->MinScaleOffsetX) * this->ScaleX) + (newScaleX / 3),
				(this->StartPosY + this->MaxSizeY) - (((sy + 1) - this->MinScaleOffsetY) * this->ScaleY) + (newScaleY / 3),
				newScaleX,
				newScaleY
			);
		}
	}
}

void CFullMap::RenderCharacters()
{
	float PlayerSizeX = (this->ScaleX * 3 / 2);

	float PlayerSizeY = (this->ScaleY * 3 / 2);

	DWORD ViewportAddress;

	float PlayerPosX;

	float PlayerPosY;

	for (int i = 0; i < MAX_MAIN_VIEWPORT; i++)
	{
		if ((ViewportAddress = (*(DWORD*)(0x07ABF5D0) + (i * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		PlayerPosX = this->StartPosX + (((*(float*)(ViewportAddress + 0x10) / 100.0f) - this->MinScaleOffsetX) * this->ScaleX);

		PlayerPosY = (this->StartPosY + this->MaxSizeY) - (((*(float*)(ViewportAddress + 0x14) / 100.0f) - this->MinScaleOffsetY) * this->ScaleY);

		if (*(BYTE*)(ViewportAddress + 0x84) == 1) // Player
		{
			if (ViewportAddress == Hero)
			{
				EnableAlphaTest(true);

				glColor3f(0.0f, 1.0f, 0.0f);

				MyRenderBitmapRotate
				(
					0x9,
					PlayerPosX,
					PlayerPosY + this->ScaleY,
					this->ScaleX * 2,
					this->ScaleY * 3,
					*(float*)(ViewportAddress + 0x24),
					(2.0f / 32.0f),
					(14.0f / 32.0f),
					((32.0f - 2.0f) / 32.0f),
					((32.0f - 14.0f) / 32.0f)
				);
			}
			else
			{
				EnableAlphaTest(true);

				glColor3f(1.0f, 1.0f, 0.0f);

				MyRenderBitmapRotate
				(
					0x9,
					PlayerPosX,
					PlayerPosY + this->ScaleY,
					PlayerSizeX,
					PlayerSizeY,
					*(float*)(ViewportAddress + 0x24),
					(2.0f / 32.0f),
					(14.0f / 32.0f),
					((32.0f - 2.0f) / 32.0f),
					((32.0f - 14.0f) / 32.0f)
				);
			}
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 2) // Monster
		{
			EnableAlphaTest(true);

			glColor3f(0.5f, 0.0f, 0.0f);

			RenderColor(PlayerPosX, PlayerPosY, this->ScaleX, this->ScaleY);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 4) // NPC
		{
			EnableAlphaTest(true);

			glColor3f(1.0f, 0.0f, 1.0f);

			MyRenderBitmapRotate
			(
				0x9,
				PlayerPosX,
				PlayerPosY + this->ScaleY,
				PlayerSizeX,
				PlayerSizeY,
				*(float*)(ViewportAddress + 0x24),
				(2.0f / 32.0f),
				(14.0f / 32.0f),
				((32.0f - 2.0f) / 32.0f),
				((32.0f - 14.0f) / 32.0f)
			);

			DisableAlphaBlend();

			glColor3f(1.0f, 1.0f, 1.0f);

			SelectObject(m_hFontDC, this->FullMapFont);

			SetBackgroundTextColor = Color4f(0, 0, 0, 255);

			SetTextColor = Color4f(255, 255, 255, 255);

			RenderText
			(
				CenterTextPosX((char*)(ViewportAddress + 0x1C1), (int)PlayerPosX),
				CenterTextPosY((char*)(ViewportAddress + 0x1C1), (int)(PlayerPosY - this->ScaleY)),
				(char*)(ViewportAddress + 0x1C1),
				0,
				0,
				NULL
			);

			SelectObject(m_hFontDC, g_hFont);
		}
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void CFullMap::RenderZoom(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	if (gFullMap.FullMapSwitch)
	{
		EnableAlphaTest(true);

		glColor3f(1.0f, 1.0f, 1.0f);

		RenderBitmap(0xF5, 0.0f, 411.0f, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

		if (IsWorkZone(75, 411, 21, 21) && MouseLButton)
		{
			RenderBitmap(0x121, 75.0f, 411.0f, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}
		else
		{
			RenderBitmap(0x120, 75.0f, 411.0f, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
		}

		DisableAlphaBlend();

		glColor3f(1.0f, 1.0f, 1.0f);

		SelectObject(m_hFontDC, g_hFontBold);

		char Text[128] = { 0 };

		wsprintf(Text, "Zoom Level: %d", gFullMap.ZoomLevel);

		DWORD bkBgColor = SetBackgroundTextColor;

		SetBackgroundTextColor = Color4f(0, 0, 0, 255);

		DWORD bkTextColor = SetTextColor;

		SetTextColor = Color4f(255, 0, 0, 255);

		RenderText(5, 417, Text, 0, 0, NULL);

		SetBackgroundTextColor = bkBgColor;

		SetTextColor = bkTextColor;

		SelectObject(m_hFontDC, g_hFont);
	}
	else
	{
		RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
	}
}

bool CFullMap::CheckInterfaces()
{
	if (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened || GoldenArcherOpened)
	{
		return false;
	}

	return true;
}