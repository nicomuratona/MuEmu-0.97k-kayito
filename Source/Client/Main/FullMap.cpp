#include "stdafx.h"
#include "FullMap.h"
#include "Protect.h"
#include "LoadModels.h"

CFullMap gFullMap;

CFullMap::CFullMap()
{
	this->FullMapSwitch = false;

	this->ZoomLevel = GetPrivateProfileInt("FullMap", "ZoomLevel", 0, ".\\Config.ini");

	this->MaxSize = 640.0f;

	this->VisualRange = 128 - (this->ZoomLevel * 16);

	this->Scale = this->MaxSize / (this->VisualRange * 2);

	this->Size = 256.0f * this->Scale;

	this->CharPosX = 1;
	this->CharPosY = 1;
	
	this->MapOffsetX = 1;
	this->MapOffsetY = 1;

	this->FullMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");

	this->LastMap = -1;

	this->m_GatesIndex.clear();
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

void CFullMap::LoadImages()
{
	glGenTextures(1, &this->MapTextureID); // Reserve the texture ID

	glBindTexture(GL_TEXTURE_2D, this->MapTextureID); // Bind the texture

	glTexImage2D(GL_TEXTURE_2D, 0, 4, MAP_SIZE, MAP_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)NULL); // Reserve memory space

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Set the texture filter

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Set the texture filter

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // Set the texture wrap

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // Set the texture wrap

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
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

	this->VisualRange = 128 - (this->ZoomLevel * 16);

	this->Scale = this->MaxSize / (this->VisualRange * 2);

	this->Size = 256.0f * this->Scale;

	DeleteObject((HGDIOBJ)this->FullMapFont);

	this->FullMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");
}

bool CFullMap::CheckInterfaces()
{
	if (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened || GoldenArcherOpened || GuildCreatorOpened)
	{
		return false;
	}

	return true;
}

void CFullMap::LoadMap()
{
	BITMAP_t* pNewBitmap = gLoadModels.GetBitmap(MAP_TEXTURE_NUMBER);

	int MapWidth = MAP_SIZE;

	int MapHeight = MAP_SIZE;

	pNewBitmap->Width = (float)MapWidth;

	pNewBitmap->Height = (float)MapHeight;

	pNewBitmap->Components = 4;

	pNewBitmap->TextureNumber = this->MapTextureID;

	size_t BufferSize = MapWidth * MapHeight * 4;

	if (!pNewBitmap->Buffer)
	{
		pNewBitmap->Buffer = (BYTE*)operator_new(BufferSize);

		m_dwUsedTextureMemory += BufferSize;

		pNewBitmap->Ref++;
	}

	memset(pNewBitmap->Buffer, 0, BufferSize);

	int index;

	unsigned char* dst = NULL;

	int dstIndex;

	for (int sy = 0; sy < TERRAIN_SIZE; sy++)
	{
		for (int sx = 0; sx < TERRAIN_SIZE; sx++)
		{
			index = (TERRAIN_SIZE - 1 - sy) * TERRAIN_SIZE + sx;

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
			    || ((TerrainWall[index] >> 0) & 0x1) // TW_SAFEZONE
			    || ((TerrainWall[index] >> 1) & 0x1)) // TW_CHARACTER
			{
				this->PaintBorder(sx, sy, pNewBitmap->Buffer);

				dstIndex = (((sy * 2) * MapWidth) + (sx * 2)) * 4;
				dst = &pNewBitmap->Buffer[dstIndex];

				dst[0] = 0;
				dst[1] = 0;
				dst[2] = 0;
				dst[3] = 204;

				dst[4] = 0;
				dst[5] = 0;
				dst[6] = 0;
				dst[7] = 189;

				dstIndex = ((((sy * 2) + 1) * MapWidth) + (sx * 2)) * 4;
				dst = &pNewBitmap->Buffer[dstIndex];

				dst[0] = 0;
				dst[1] = 0;
				dst[2] = 0;
				dst[3] = 204;

				dst[4] = 0;
				dst[5] = 0;
				dst[6] = 0;
				dst[7] = 189;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, pNewBitmap->TextureNumber); // Bind the texture

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, MapWidth, MapHeight, GL_RGBA, GL_UNSIGNED_BYTE, pNewBitmap->Buffer); // Update the texture

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

void CFullMap::LoadGates()
{
	this->m_GatesIndex.clear();

	for (BYTE i = 0; i < MAX_GATES; i++)
	{
		if (GateAttribute[i].Flag == 1 && GateAttribute[i].Map == World)
		{
			this->m_GatesIndex.push_back(i);
		}
	}
}

void CFullMap::PaintBorder(int sx, int sy, BYTE* bitmap)
{
	unsigned char* dst = NULL;

	if ((sx - 1) > 0)
	{
		if (((TerrainWall[(TERRAIN_SIZE - 1 - sy) * TERRAIN_SIZE + (sx - 1)] >> 0) & 0xFC) != 0)
		{
			dst = &bitmap[((sy * 2) * MAP_SIZE + ((sx - 1) * 2)) * 4];

			dst[4] = 192;
			dst[5] = 254;
			dst[6] = 243;
			dst[7] = 128;

			dst = &bitmap[(((sy * 2) + 1) * MAP_SIZE + ((sx - 1) * 2)) * 4];

			dst[4] = 192;
			dst[5] = 254;
			dst[6] = 243;
			dst[7] = 128;
		}
	}

	if ((sx + 1) < TERRAIN_SIZE)
	{
		if (((TerrainWall[(TERRAIN_SIZE - 1 - sy) * TERRAIN_SIZE + (sx + 1)] >> 0) & 0xFC) != 0)
		{
			dst = &bitmap[((sy * 2) * MAP_SIZE + ((sx + 1) * 2)) * 4];

			dst[0] = 192;
			dst[1] = 254;
			dst[2] = 243;
			dst[3] = 128;

			dst = &bitmap[(((sy * 2) + 1) * MAP_SIZE + ((sx + 1) * 2)) * 4];

			dst[0] = 192;
			dst[1] = 254;
			dst[2] = 243;
			dst[3] = 128;
		}
	}

	if ((sy - 1) > 0)
	{
		if (((TerrainWall[(TERRAIN_SIZE - 1 - (sy - 1)) * TERRAIN_SIZE + sx] >> 0) & 0xFC) != 0)
		{
			dst = &bitmap[((((sy - 1) * 2) + 1) * MAP_SIZE + (sx * 2)) * 4];

			dst[0] = 192;
			dst[1] = 254;
			dst[2] = 243;
			dst[3] = 128;

			dst[4] = 192;
			dst[5] = 254;
			dst[6] = 243;
			dst[7] = 128;
		}
	}

	if ((sy + 1) < TERRAIN_SIZE)
	{
		if (((TerrainWall[(TERRAIN_SIZE - 1 - (sy + 1)) * TERRAIN_SIZE + sx] >> 0) & 0xFC) != 0)
		{
			dst = &bitmap[((((sy + 1) * 2)) * MAP_SIZE + (sx * 2)) * 4];

			dst[0] = 192;
			dst[1] = 254;
			dst[2] = 243;
			dst[3] = 128;

			dst[4] = 192;
			dst[5] = 254;
			dst[6] = 243;
			dst[7] = 128;
		}
	}
}

void CFullMap::RenderFullMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	gFullMap.CharPosX = *(float*)(Hero + 0x10) / 100.0f;

	gFullMap.CharPosY = *(float*)(Hero + 0x14) / 100.0f;

	gFullMap.MapOffsetX = (gFullMap.CharPosX / 256.0f) * gFullMap.Size;

	gFullMap.MapOffsetY = (gFullMap.CharPosY / 256.0f) * gFullMap.Size;

	if (gFullMap.LastMap != World)
	{
		gFullMap.LoadMap();

		gFullMap.LoadGates();

		gFullMap.LastMap = World;
	}

	if (gFullMap.FullMapSwitch)
	{
		MyRenderBitRotate
		(
			MAP_TEXTURE_NUMBER,
			gFullMap.Size - gFullMap.MapOffsetX,
			gFullMap.Size - (gFullMap.Size - gFullMap.MapOffsetY),
			gFullMap.Size,
			gFullMap.Size,
			-45.0f,
			(0.0f / 512.0f),
			(0.0f / 512.0f),
			((512.0f - 0.0f) / 512.0f),
			((512.0f - 0.0f) / 512.0f)
		);

		gFullMap.RenderGates();

		gFullMap.RenderCharacters();
	}
	else
	{
		RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
	}
}

void CFullMap::RenderGates()
{
	float TargetPosX;
	float TargetPosY;

	float DifX;
	float DifY;

	GATE_ATTRIBUTE* gate;
	GATE_ATTRIBUTE* targetGate;

	char text[64];

	for each (BYTE index in this->m_GatesIndex)
	{
		gate = &GateAttribute[index];

		DifX = ceil((gate->EndX - gate->StartX) / 2.0f);
		DifY = ceil((gate->EndY - gate->StartY) / 2.0f);

		TargetPosX = (((gate->StartX + DifX) / 256.0f) * this->Size);
		TargetPosY = (((gate->StartY + DifY) / 256.0f) * this->Size);

		targetGate = &GateAttribute[gate->Target];

		sprintf_s(text, "Gate to %s", gMapManager.GetMapName(targetGate->Map));

		EnableAlphaTest(true);

		glColor3f(0.0f, 1.0f, 1.0f);

		SelectObject(m_hFontDC, this->FullMapFont);

		MyRenderPointRotate
		(
			0x9,
			TargetPosX,
			(this->Size - TargetPosY),
			this->Scale * 5,
			this->Scale * 5,
			this->Size - this->MapOffsetX,
			this->Size - (this->Size - this->MapOffsetY),
			this->Size,
			this->Size,
			-45.0f,
			0.0f,
			(0.0f / 32.0f),
			(0.0f / 32.0f),
			(32.0f / 32.0f),
			(32.0f / 32.0f),
			text
		);
	}
}

void CFullMap::RenderCharacters()
{
	DWORD ViewportAddress;

	float TargetPosX;

	float TargetPosY;

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

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

		TargetPosX = (((*(float*)(ViewportAddress + 0x10) / 100.0f) / 256.0f) * this->Size);

		TargetPosY = (((*(float*)(ViewportAddress + 0x14) / 100.0f) / 256.0f) * this->Size);

		if (*(BYTE*)(ViewportAddress + 0x84) == 1) // Player
		{
			if (ViewportAddress == Hero)
			{
				EnableAlphaTest(true);

				glColor3f(0.0f, 1.0f, 0.0f);

				MyRenderPointRotate
				(
					0x9,
					this->MapOffsetX,
					(this->Size - this->MapOffsetY),
					this->Scale * 3,
					this->Scale * 3,
					this->Size - this->MapOffsetX,
					this->Size - (this->Size - this->MapOffsetY),
					this->Size,
					this->Size,
					-45.0f,
					*(float*)(ViewportAddress + 0x24) - 45.0f,
					(2.0f / 32.0f),
					(14.0f / 32.0f),
					((32.0f - 2.0f) / 32.0f),
					((32.0f - 14.0f) / 32.0f),
					NULL
				);
			}
			else
			{
				EnableAlphaTest(true);

				glColor3f(1.0f, 1.0f, 0.0f);

				MyRenderPointRotate
				(
					0x9,
					TargetPosX,
					(this->Size - TargetPosY),
					this->Scale * 3,
					this->Scale * 3,
					this->Size - this->MapOffsetX,
					this->Size - (this->Size - this->MapOffsetY),
					this->Size,
					this->Size,
					-45.0f,
					*(float*)(ViewportAddress + 0x24) - 45.0f,
					(2.0f / 32.0f),
					(14.0f / 32.0f),
					((32.0f - 2.0f) / 32.0f),
					((32.0f - 14.0f) / 32.0f),
					NULL
				);
			}
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 2) // Monster
		{
			EnableAlphaTest(true);

			glColor3f(0.5f, 0.0f, 0.0f);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				(this->Size - TargetPosY),
				this->Scale * 2,
				this->Scale * 2,
				this->Size - this->MapOffsetX,
				this->Size - (this->Size - this->MapOffsetY),
				this->Size,
				this->Size,
				-45.0f,
				*(float*)(ViewportAddress + 0x24) - 45.0f,
				(2.0f / 32.0f),
				(14.0f / 32.0f),
				((32.0f - 2.0f) / 32.0f),
				((32.0f - 14.0f) / 32.0f),
				NULL
			);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 4) // NPC
		{
			EnableAlphaTest(true);

			glColor3f(1.0f, 0.0f, 1.0f);

			SelectObject(m_hFontDC, this->FullMapFont);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				(this->Size - TargetPosY),
				this->Scale * 3,
				this->Scale * 3,
				this->Size - this->MapOffsetX,
				this->Size - (this->Size - this->MapOffsetY),
				this->Size,
				this->Size,
				-45.0f,
				*(float*)(ViewportAddress + 0x24) - 45.0f,
				(2.0f / 32.0f),
				(14.0f / 32.0f),
				((32.0f - 2.0f) / 32.0f),
				((32.0f - 14.0f) / 32.0f),
				(char*)(ViewportAddress + 0x1C1)
			);
		}
	}

	DisableAlphaBlend();

	SelectObject(m_hFontDC, g_hFont);

	glColor3f(1.0f, 1.0f, 1.0f);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CFullMap::RenderZoom(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	if (gFullMap.FullMapSwitch)
	{
		float alpha = 0.3f;

		if (IsWorkZone(0, 411, 96, 21))
		{
			alpha = 1.0f;
		}

		EnableAlphaTest(true);

		glColor4f(1.0f, 1.0f, 1.0f, alpha);

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

		EnableAlphaTest(true);

		char Text[128] = { 0 };

		wsprintf(Text, "Zoom Level: %d", gFullMap.ZoomLevel);

		DWORD backupBgTextColor = SetBackgroundTextColor;

		SetBackgroundTextColor = Color4b(0, 0, 0, 0);

		DWORD backupTextColor = SetTextColor;

		SetTextColor = Color4b(255, 0, 0, (int)(alpha * 255));

		RenderText(5, CenterTextPosY(Text, 421), Text, 0, RT3_SORT_LEFT, NULL);

		SetBackgroundTextColor = backupBgTextColor;

		SetTextColor = backupTextColor;

		SelectObject(m_hFontDC, g_hFont);

		DisableAlphaBlend();
	}
	else
	{
		RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
	}
}