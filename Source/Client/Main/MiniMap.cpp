#include "stdafx.h"
#include "MiniMap.h"
#include "EventTimer.h"
#include "LoadModels.h"
#include "MapManager.h"
#include "MoveList.h"

CMiniMap gMiniMap;

CMiniMap::CMiniMap()
{
	memset(this->MiniMapTable, 0, sizeof(this->MiniMapTable));

	this->MiniMapSwitch = false;

	this->Alpha = GetPrivateProfileInt("MiniMap", "Alpha", 10, ".\\Config.ini");

	this->ZoomLevel = GetPrivateProfileInt("MiniMap", "ZoomLevel", 0, ".\\Config.ini");

	this->VisualRange = 128 - (this->ZoomLevel * 16);

	this->MiniMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");

	this->m_GatesIndex.clear();
}

CMiniMap::~CMiniMap()
{
	char Text[33] = { 0 };

	wsprintf(Text, "%d", this->Alpha);

	WritePrivateProfileString("MiniMap", "Alpha", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->ZoomLevel);

	WritePrivateProfileString("MiniMap", "ZoomLevel", Text, ".\\Config.ini");
}

void CMiniMap::Init()
{
	SetCompleteHook(0xE8, 0x0050E6F8, &this->MiniMapLoad);
}

void CMiniMap::ChangeZoom(int i)
{
	if (i == -1)
	{
		this->ZoomLevel = (this->ZoomLevel + 1 >= 7) ? 0 : this->ZoomLevel + 1;
	}
	else
	{
		this->ZoomLevel = i;
	}

	this->VisualRange = 128 - (this->ZoomLevel * 16);

	DeleteObject((HGDIOBJ)this->MiniMapFont);

	this->MiniMapFont = CreateFont(10 + (this->ZoomLevel * 2), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH, "Verdana");
}

void CMiniMap::ChangeAlpha()
{
	this->Alpha = (this->Alpha + 1 > 10) ? 3 : this->Alpha + 1;
}

void CMiniMap::MiniMapLoad(char* FileName)
{
	OpenTerrainLight(FileName);

	if (World >= 0 && World < MAX_MAPS)
	{
		gMiniMap.MiniMapTable[World] = (gMapManager.GetMiniMap(World) != 0);

		if (!gMiniMap.MiniMapTable[World])
		{
			gMiniMap.MiniMapSwitch = false;
		}

		gMiniMap.LoadMiniMapImage();

		gMiniMap.LoadMiniMapGates();
	}
}

void CMiniMap::LoadMiniMapImage()
{
	BITMAP_t* pNewBitmap = gLoadModels.GetBitmap(MINIMAP_TEXTURE);

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

void CMiniMap::PaintBorder(int sx, int sy, BYTE* bitmap)
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

void CMiniMap::LoadMiniMapGates()
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

void CMiniMap::LoadImages()
{
	// Frame Texture
	OpenTGA(MINIMAP_FRAME_TEXTURE_NAME, MINIMAP_FRAME_TEXTURE, GL_NEAREST, GL_CLAMP, 0, 1);

	// Map Texture
	glGenTextures(1, &this->MapTextureID); // Reserve the texture ID

	glBindTexture(GL_TEXTURE_2D, this->MapTextureID); // Bind the texture

	glTexImage2D(GL_TEXTURE_2D, 0, 4, MAP_SIZE, MAP_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)NULL); // Reserve memory space

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Set the texture filter

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Set the texture filter

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // Set the texture wrap

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // Set the texture wrap

	GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f }; // White color

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // Specify the border color

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

bool CMiniMap::GetMiniMapState()
{
	return this->MiniMapSwitch;
}

void CMiniMap::Toggle()
{
	if (!this->MiniMapTable[World])
	{
		this->MiniMapSwitch = false;

		return;
	}

	if (CheckInputInterfaces())
	{
		return;
	}

	if (gEventTimer.GetEventTimerState())
	{
		return;
	}

	if (gMoveList.GetMoveListState())
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->MiniMapSwitch = false;

		return;
	}

	this->MiniMapSwitch ^= 1;

	PlayBuffer(25, 0, 0);
}

void CMiniMap::UpdateMouse()
{
	if (!this->MiniMapTable[World])
	{
		return;
	}

	if (!this->MiniMapSwitch)
	{
		return;
	}

	if (CheckRightInterfaces())
	{
		this->MiniMapSwitch = false;

		return;
	}

	BYTE Type = gMapManager.GetMiniMap(World);

	if (Type == 1)
	{
		this->UpdateMouseMiniMap();
	}
	else if (Type == 2)
	{
		this->UpdateMouseFullMap();
	}
}

void CMiniMap::Render()
{
	if (!this->MiniMapTable[World])
	{
		return;
	}

	if (!this->MiniMapSwitch)
	{
		return;
	}

	BYTE Type = gMapManager.GetMiniMap(World);

	if (Type == 1)
	{
		this->RenderMiniMap();
	}
	else if (Type == 2)
	{
		this->RenderFullMap();
	}
}

/*******************************************************/
/*********************** MINIMAP ***********************/
/*******************************************************/

void CMiniMap::UpdateMouseMiniMap()
{
	float StartPosX = (640.0f - 151.0f);

	float StartPosY = (432.0f - 173.0f);

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 151, 173)) // Frame
	{
		MouseOnWindow = true;

		StartPosX += 4.0f;

		StartPosY += 29.0f;

		if (IsWorkZone((int)StartPosX, (int)StartPosY, 36, 16)) // Alpha
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->ChangeAlpha();
			}

			return;
		}

		StartPosY += 21.0f;

		for (int i = 0; i <= 6; i++)
		{
			if (IsWorkZone((int)StartPosX, (int)StartPosY, 17, 16)) // Zoom Numbers
			{
				if (MouseLButton && MouseLButtonPush)
				{
					MouseLButtonPush = false;

					MouseUpdateTime = 0;

					MouseUpdateTimeMax = 6;

					PlayBuffer(25, 0, 0);

					this->ChangeZoom(i);
				}

				return;
			}

			StartPosY += 17.0f;
		}

		StartPosX = (640.0f - 151.0f + 130.0f);

		StartPosY = (432.0f - 173.0f + 15.0f);

		if (IsWorkZone((int)StartPosX, (int)StartPosY, 20, 20)) // Zoom Button
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->ChangeZoom();
			}

			return;
		}
	}
}

void CMiniMap::RenderMiniMap()
{
	this->MaxSize = 128.0f;

	this->Scale = 256.0f / (this->VisualRange * 2);

	this->Size = 512.0f / this->Scale;

	this->CharPosX = *(float*)(Hero + 0x10) / 100.0f;

	this->CharPosY = *(float*)(Hero + 0x14) / 100.0f;

	this->MapOffsetX = this->CharPosX - this->VisualRange;
	this->MapOffsetX /= 256.0f;

	this->MapOffsetY = (256.0f - this->CharPosY) - this->VisualRange;
	this->MapOffsetY /= 256.0f;

	this->RenderMiniMapBackground();

	this->RenderMiniMapCharacters();

	this->RenderMiniMapGates();

	this->RenderMiniMapFrame();

	this->RenderMiniMapButtons();
}

void CMiniMap::RenderMiniMapBackground()
{
	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

	MyRenderBitRotate
	(
		MINIMAP_TEXTURE,
		320.0f,
		240.0f - 48.0f,
		this->MaxSize,
		this->MaxSize,
		0.0f,
		((this->MapOffsetX * 512.0f) / 512.0f),
		((this->MapOffsetY * 512.0f) / 512.0f),
		(this->Size / 512.0f),
		(this->Size / 512.0f)
	);

	DisableAlphaBlend();
}

void CMiniMap::RenderMiniMapCharacters()
{
	DWORD ViewportAddress;

	float TargetPosX;

	float TargetPosY;

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	for (int i = 0; i < MAX_MAIN_VIEWPORT; i++)
	{
		if ((ViewportAddress = (CharactersClient + (i * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		TargetPosX = (((((*(float*)(ViewportAddress + 0x10) / 100.0f) / 256.0f) - this->MapOffsetX) * (this->MaxSize * this->Scale)));

		TargetPosY = (((((256.0f - (*(float*)(ViewportAddress + 0x14) / 100.0f)) / 256.0f) - this->MapOffsetY) * (this->MaxSize * this->Scale)));

		if (*(BYTE*)(ViewportAddress + 0x84) == 1) // Player
		{
			if (ViewportAddress == Hero)
			{
				EnableAlphaTest(true);

				glColor4f(0.0f, 1.0f, 0.0f, this->Alpha * 0.1f);

				MyRenderPointRotate
				(
					0x9,
					TargetPosX,
					TargetPosY,
					this->Scale * 3,
					this->Scale * 3,
					320.0f,
					240.0f - 48.0f,
					this->MaxSize,
					this->MaxSize,
					0.0f,
					*(float*)(ViewportAddress + 0x24),
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

				glColor4f(1.0f, 1.0f, 0.0f, this->Alpha * 0.1f);

				MyRenderPointRotate
				(
					0x9,
					TargetPosX,
					TargetPosY,
					this->Scale * 3,
					this->Scale * 3,
					320.0f,
					240.0f - 48.0f,
					this->MaxSize,
					this->MaxSize,
					0.0f,
					*(float*)(ViewportAddress + 0x24),
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

			glColor4f(0.5f, 0.0f, 0.0f, this->Alpha * 0.1f);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				TargetPosY,
				this->Scale * 2,
				this->Scale * 2,
				320.0f,
				240.0f - 48.0f,
				this->MaxSize,
				this->MaxSize,
				0.0f,
				*(float*)(ViewportAddress + 0x24),
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

			glColor4f(1.0f, 0.0f, 1.0f, this->Alpha * 0.1f);

			SelectObject(m_hFontDC, this->MiniMapFont);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				TargetPosY,
				this->Scale * 3,
				this->Scale * 3,
				320.0f,
				240.0f - 48.0f,
				this->MaxSize,
				this->MaxSize,
				0.0f,
				*(float*)(ViewportAddress + 0x24),
				(2.0f / 32.0f),
				(14.0f / 32.0f),
				((32.0f - 2.0f) / 32.0f),
				((32.0f - 14.0f) / 32.0f),
				(char*)(ViewportAddress + 0x1C1)
			);
		}
	}

	DisableAlphaBlend();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMiniMap::RenderMiniMapGates()
{
	float TargetPosX;
	float TargetPosY;

	float DifX;
	float DifY;

	GATE_ATTRIBUTE* gate;
	GATE_ATTRIBUTE* targetGate;

	char text[64];

	for each (int index in this->m_GatesIndex)
	{
		gate = &GateAttribute[index];

		DifX = ceil((gate->EndX - gate->StartX) / 2.0f);
		DifY = ceil((gate->EndY - gate->StartY) / 2.0f);

		TargetPosX = gate->StartX + DifX;
		TargetPosY = gate->StartY + DifY;

		if (TargetPosX > (this->CharPosX + this->VisualRange) || TargetPosX < (this->CharPosX - this->VisualRange))
		{
			continue;
		}

		if (TargetPosY > (this->CharPosY + this->VisualRange) || TargetPosY < (this->CharPosY - this->VisualRange))
		{
			continue;
		}

		TargetPosX = (((((gate->StartX + DifX) / 256.0f) - this->MapOffsetX) * (this->MaxSize * this->Scale)));
		TargetPosY = (((((256.0f - (gate->StartY + DifY)) / 256.0f) - this->MapOffsetY) * (this->MaxSize * this->Scale)));

		targetGate = &GateAttribute[gate->Target];

		sprintf_s(text, "Gate to %s", gMapManager.GetMapName(targetGate->Map));

		EnableAlphaTest(true);

		glColor4f(0.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

		SelectObject(m_hFontDC, this->MiniMapFont);

		MyRenderPointRotate
		(
			0x9,
			TargetPosX,
			TargetPosY,
			this->Scale * 5,
			this->Scale * 5,
			320.0f,
			240.0f - 48.0f,
			this->MaxSize,
			this->MaxSize,
			0.0f,
			0.0f,
			(0.0f / 32.0f),
			(0.0f / 32.0f),
			(32.0f / 32.0f),
			(32.0f / 32.0f),
			text
		);

		DisableAlphaBlend();
	}

	DisableAlphaBlend();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFont);
}

void CMiniMap::RenderMiniMapFrame()
{
	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

	float StartPosX = (640.0f - 151.0f);

	float StartPosY = (432.0f - 173.0f);

	RenderBitmap(MINIMAP_FRAME_TEXTURE, StartPosX, StartPosY, 151.0f, 173.0f, (0.0f / 256.0f), (0.0f / 256.0f), (151.0f / 256.0f), (173.0f / 256.0f), true, true);

	DisableAlphaBlend();
}

void CMiniMap::RenderMiniMapButtons()
{
	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

	float StartPosX = (640.0f - 151.0f + 23.0f);

	float StartPosY = (432.0f - 173.0f + 32.0f);

	RenderNumber2D(StartPosX, StartPosY, this->Alpha * 10, 8.0f, 8.0f);

	StartPosX = (640.0f - 151.0f + 12.0f);

	StartPosY = (432.0f - 173.0f + 53.0f);

	for (int i = 0; i <= 6; i++)
	{
		if (i == this->ZoomLevel)
		{
			glColor4f(1.0f, 0.0f, 0.0f, this->Alpha * 0.1f);
		}
		else
		{
			glColor4f(1.0f, 1.0f, 1.0f, this->Alpha * 0.1f);
		}

		RenderNumber2D(StartPosX, StartPosY, i, 8.0f, 8.0f);

		StartPosY += 17.0f;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

/*******************************************************/
/*********************** FULLMAP ***********************/
/*******************************************************/

void CMiniMap::UpdateMouseFullMap()
{
	float StartPosX = 0.0f;

	float StartPosY = 411.0f;

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 96, 21)) // Zoom
	{
		MouseOnWindow = true;

		StartPosX += 75.0f;

		if (IsWorkZone((int)StartPosX, (int)StartPosY, 21, 21)) // Zoom Button
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->ChangeZoom();
			}
		}

		return;
	}

	StartPosX = (640.0f - 96.0f);

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 96, 21)) // Alpha
	{
		MouseOnWindow = true;

		if (IsWorkZone((int)StartPosX, (int)StartPosY, 21, 21)) // Alpha Button
		{
			if (MouseLButton && MouseLButtonPush)
			{
				MouseLButtonPush = false;

				MouseUpdateTime = 0;

				MouseUpdateTimeMax = 6;

				PlayBuffer(25, 0, 0);

				this->ChangeAlpha();
			}
		}

		return;
	}
}

void CMiniMap::RenderFullMap()
{
	this->MaxSize = 640.0f;

	this->Scale = 256.0f / (this->VisualRange * 2);

	this->Size = 512.0f / this->Scale;

	this->CharPosX = *(float*)(Hero + 0x10) / 100.0f;

	this->CharPosY = *(float*)(Hero + 0x14) / 100.0f;

	this->MapOffsetX = this->CharPosX - this->VisualRange;
	this->MapOffsetX /= 256.0f;

	this->MapOffsetY = (256.0f - this->CharPosY) - this->VisualRange;
	this->MapOffsetY /= 256.0f;

	this->RenderFullMapBackground();

	this->RenderFullMapCharacters();

	this->RenderFullMapGates();

	this->RenderFullMapZoom();

	this->RenderFullMapAlpha();
}

void CMiniMap::RenderFullMapBackground()
{
	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

	MyRenderBitRotate
	(
		MINIMAP_TEXTURE,
		320.0f - 320.0f + (this->MaxSize * 0.5f),
		192.0f - 240.0f + (this->MaxSize * 0.5f),
		this->MaxSize,
		this->MaxSize,
		-45.0f,
		((this->MapOffsetX * 512.0f) / 512.0f),
		((this->MapOffsetY * 512.0f) / 512.0f),
		(this->Size / 512.0f),
		(this->Size / 512.0f)
	);
}

void CMiniMap::RenderFullMapCharacters()
{
	DWORD ViewportAddress;

	float TargetPosX;

	float TargetPosY;

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	for (int i = 0; i < MAX_MAIN_VIEWPORT; i++)
	{
		if ((ViewportAddress = (CharactersClient + (i * 916))) == 0)
		{
			continue;
		}

		if (*(BYTE*)(ViewportAddress) == 0)
		{
			continue;
		}

		TargetPosX = (((((*(float*)(ViewportAddress + 0x10) / 100.0f) / 256.0f) - this->MapOffsetX) * (this->MaxSize * this->Scale)));

		TargetPosY = (((((256.0f - (*(float*)(ViewportAddress + 0x14) / 100.0f)) / 256.0f) - this->MapOffsetY) * (this->MaxSize * this->Scale)));

		if (*(BYTE*)(ViewportAddress + 0x84) == 1) // Player
		{
			if (ViewportAddress == Hero)
			{
				EnableAlphaTest(true);

				glColor4f(0.0f, 1.0f, 0.0f, this->Alpha * 0.1f);

				MyRenderPointRotate
				(
					0x9,
					TargetPosX,
					TargetPosY,
					this->Scale * 7,
					this->Scale * 7,
					320.0f - 320.0f + (this->MaxSize * 0.5f),
					192.0f - 240.0f + (this->MaxSize * 0.5f),
					this->MaxSize,
					this->MaxSize,
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

				glColor4f(1.0f, 1.0f, 0.0f, this->Alpha * 0.1f);

				MyRenderPointRotate
				(
					0x9,
					TargetPosX,
					TargetPosY,
					this->Scale * 7,
					this->Scale * 7,
					320.0f - 320.0f + (this->MaxSize * 0.5f),
					192.0f - 240.0f + (this->MaxSize * 0.5f),
					this->MaxSize,
					this->MaxSize,
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

			glColor4f(0.5f, 0.0f, 0.0f, this->Alpha * 0.1f);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				TargetPosY,
				this->Scale * 6,
				this->Scale * 6,
				320.0f - 320.0f + (this->MaxSize * 0.5f),
				192.0f - 240.0f + (this->MaxSize * 0.5f),
				this->MaxSize,
				this->MaxSize,
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

			glColor4f(1.0f, 0.0f, 1.0f, this->Alpha * 0.1f);

			SelectObject(m_hFontDC, this->MiniMapFont);

			MyRenderPointRotate
			(
				0x9,
				TargetPosX,
				TargetPosY,
				this->Scale * 7,
				this->Scale * 7,
				320.0f - 320.0f + (this->MaxSize * 0.5f),
				192.0f - 240.0f + (this->MaxSize * 0.5f),
				this->MaxSize,
				this->MaxSize,
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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFont);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

void CMiniMap::RenderFullMapGates()
{
	float TargetPosX;
	float TargetPosY;

	float DifX;
	float DifY;

	GATE_ATTRIBUTE* gate;
	GATE_ATTRIBUTE* targetGate;

	char text[64];

	for each (int index in this->m_GatesIndex)
	{
		gate = &GateAttribute[index];

		DifX = ceil((gate->EndX - gate->StartX) / 2.0f);
		DifY = ceil((gate->EndY - gate->StartY) / 2.0f);

		TargetPosX = gate->StartX + DifX;
		TargetPosY = gate->StartY + DifY;

		if (TargetPosX > (this->CharPosX + this->VisualRange) || TargetPosX < (this->CharPosX - this->VisualRange))
		{
			continue;
		}

		if (TargetPosY > (this->CharPosY + this->VisualRange) || TargetPosY < (this->CharPosY - this->VisualRange))
		{
			continue;
		}

		TargetPosX = (((((gate->StartX + DifX) / 256.0f) - this->MapOffsetX) * (this->MaxSize * this->Scale)));
		TargetPosY = (((((256.0f - (gate->StartY + DifY)) / 256.0f) - this->MapOffsetY) * (this->MaxSize * this->Scale)));

		targetGate = &GateAttribute[gate->Target];

		sprintf_s(text, "Gate to %s", gMapManager.GetMapName(targetGate->Map));

		EnableAlphaTest(true);

		glColor4f(0.0f, 1.0f, 1.0f, this->Alpha * 0.1f);

		SelectObject(m_hFontDC, this->MiniMapFont);

		MyRenderPointRotate
		(
			0x9,
			TargetPosX,
			TargetPosY,
			this->Scale * 10,
			this->Scale * 10,
			320.0f - 320.0f + (this->MaxSize * 0.5f),
			192.0f - 240.0f + (this->MaxSize * 0.5f),
			this->MaxSize,
			this->MaxSize,
			-45.0f,
			0.0f,
			(0.0f / 32.0f),
			(0.0f / 32.0f),
			(32.0f / 32.0f),
			(32.0f / 32.0f),
			text
		);
	}

	DisableAlphaBlend();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFont);
}

void CMiniMap::RenderFullMapZoom()
{
	float StartPosX = 0.0f;

	float StartPosY = 411.0f;

	float alpha = 0.3f;

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 96, 21))
	{
		alpha = 1.0f;
	}

	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	RenderBitmap(0xF5, StartPosX, StartPosY, 75.0f, 21.0f, (0.0f / 128.0f), (0.0f / 32.0f), (75.0f / 128.0f), (21.0f / 32.0f), true, true);

	StartPosX += 75.0f;

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 21, 21) && MouseLButton)
	{
		RenderBitmap(0x121, StartPosX, StartPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
	}
	else
	{
		RenderBitmap(0x120, StartPosX, StartPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	SelectObject(m_hFontDC, g_hFontBold);

	EnableAlphaTest(true);

	char Text[128] = { 0 };

	wsprintf(Text, "Zoom Level: %d", this->ZoomLevel);

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

void CMiniMap::RenderFullMapAlpha()
{
	float StartPosX = (640.0f - 75.0f);

	float StartPosY = 411.0f;

	float alpha = 0.3f;

	if (IsWorkZone((int)StartPosX - 21, (int)StartPosY, 96, 21))
	{
		alpha = 1.0f;
	}

	EnableAlphaTest(true);

	glColor4f(1.0f, 1.0f, 1.0f, alpha);

	RenderBitmap(0xF5, StartPosX, StartPosY, 75.0f, 21.0f, (75.0f / 128.0f), (0.0f / 32.0f), (-75.0f / 128.0f), (21.0f / 32.0f), true, true);

	StartPosX -= 21.0f;

	if (IsWorkZone((int)StartPosX, (int)StartPosY, 21, 21) && MouseLButton)
	{
		RenderBitmap(0x121, StartPosX, StartPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
	}
	else
	{
		RenderBitmap(0x120, StartPosX, StartPosY, 21.0f, 21.0f, (0.0f / 32.0f), (0.0f / 32.0f), (24.0f / 32.0f), (24.0f / 32.0f), true, true);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);

	StartPosX += 27.0f;

	SelectObject(m_hFontDC, g_hFontBold);

	EnableAlphaTest(true);

	char Text[128] = { 0 };

	wsprintf(Text, "Alpha: %d", this->Alpha * 10);

	DWORD backupBgTextColor = SetBackgroundTextColor;

	SetBackgroundTextColor = Color4b(0, 0, 0, 0);

	DWORD backupTextColor = SetTextColor;

	SetTextColor = Color4b(255, 0, 0, (int)(alpha * 255));

	RenderText((int)StartPosX, CenterTextPosY(Text, 421), Text, 75, RT3_SORT_LEFT, NULL);

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;

	SelectObject(m_hFontDC, g_hFont);

	DisableAlphaBlend();
}