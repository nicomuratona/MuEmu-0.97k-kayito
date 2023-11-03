#include "stdafx.h"
#include "MiniMap.h"
#include "Protect.h"

CMiniMap gMiniMap;

CMiniMap::CMiniMap()
{
	this->MiniMapSwitch = true;

	this->ImageLoaded = false;

	this->CurrentMap = -1;
	this->m_MiniMapFramePos[0] = 512.0f;
	this->m_MiniMapFramePos[1] = 259.0f;
	this->m_MiniMapFrameSize[0] = 128.0f;
	this->m_MiniMapFrameSize[1] = 173.0f;
	this->m_MiniMapSize[0] = 128.0f;
	this->m_MiniMapSize[1] = 128.0f;
	this->m_ZoomBtnSize[0] = 19.0f;
	this->m_ZoomBtnSize[1] = 20.0f;
	this->m_iMiniMapScale = 1;
}

CMiniMap::~CMiniMap()
{

}

void CMiniMap::Init()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	SetCompleteHook(0xE8, 0x004BD332, &this->DrawMiniMap);
}

void CMiniMap::Toggle()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	if (SceneFlag == 5)
	{
		this->MiniMapSwitch ^= 1;

		PlayBuffer(25, 0, 0);
	}
}

void CMiniMap::LoadImages()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	LoadImageTGA(MINIMAP_FRAME_TEXTURE, 0x600, GL_NEAREST, GL_CLAMP, 0, 1);
}

bool CMiniMap::CheckInterfaces()
{
	if (InventoryOpened || CharacterOpened || GuildOpened || PartyOpened || GoldenArcherOpened)
	{
		return false;
	}

	return true;
}

void CMiniMap::DrawMiniMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)
{
	if (!gMiniMap.CheckInterfaces())
	{
		return;
	}

	if (gMiniMap.MiniMapSwitch)
	{
		if (gMiniMap.CurrentMap != World)
		{
			char buff[256];

			sprintf_s(buff, MINIMAP_TEXTURE_PATH, World + 1);

			if (!FileExists(buff))
			{
				gMiniMap.ImageLoaded = false;

				goto ORIGINAL;
			}

			sprintf_s(buff, MINIMAP_TEXTURE, World + 1);

			LoadImageJPG(buff, 0x601, GL_NEAREST, GL_CLAMP, 0, 1);

			gMiniMap.ImageLoaded = true;

			gMiniMap.CurrentMap = World;
		}

		gMiniMap.RenderMiniMap();

		gMiniMap.RenderObjects();

		return;
	}

ORIGINAL:

	RenderBitmap(Texture, x, y, Width, Height, u, v, uWidth, vHeight, Scale, StartScale);
}

void CMiniMap::RenderMiniMap()
{
	this->m_CharacterPos[0] = (((*(float*)(Hero + 0x10) / 100.0f)) / this->m_iMiniMapScale);

	this->m_CharacterPos[1] = ((256.0f - (*(float*)(Hero + 0x14) / 100.0f)) / this->m_iMiniMapScale);

	this->m_MiniMapScaleOffset[0] = min(max((this->m_CharacterPos[0] - (64.0f * this->m_iMiniMapScale)), 0), 128.0f);

	this->m_MiniMapScaleOffset[1] = min(max((this->m_CharacterPos[1] - (64.0f * this->m_iMiniMapScale)), 0), 128.0f);

	float MiniMapScaleU = this->m_MiniMapScaleOffset[0] / (256.0f);

	float MiniMapScaleV = this->m_MiniMapScaleOffset[1] / (256.0f);

	EnableAlphaTest(true);

	RenderBitmap(0x601, this->m_MiniMapFramePos[0], this->m_MiniMapFramePos[1] + 46.0f, this->m_MiniMapSize[0], this->m_MiniMapSize[1], MiniMapScaleU, MiniMapScaleV, this->m_iMiniMapScale * 0.5f, this->m_iMiniMapScale * 0.5f, true, true);

	RenderBitmap(0x600, this->m_MiniMapFramePos[0], this->m_MiniMapFramePos[1], this->m_MiniMapFrameSize[0], this->m_MiniMapFrameSize[1], 0.0f, 0.0f, 1.0, (this->m_MiniMapFrameSize[1] / 256.0f), true, true);
}

void CMiniMap::CheckZoomButton()
{
	if (gProtect.m_MainInfo.EnableMinimap == 0)
	{
		return;
	}

	if (!this->MiniMapSwitch || !this->ImageLoaded)
	{
		return;
	}

	if (!this->CheckInterfaces())
	{
		return;
	}

	if (MouseX >= this->m_MiniMapFramePos[0]
	    && MouseX <= (this->m_MiniMapFramePos[0] + this->m_MiniMapFrameSize[0])
	    && MouseY >= this->m_MiniMapFramePos[1]
	    && MouseY <= (this->m_MiniMapFramePos[1] + this->m_MiniMapFrameSize[1])
	    && MouseLButton)
	{
		if (MouseLButtonPush)
		{
			MouseLButtonPush = false;

			MouseUpdateTime = 0;

			MouseUpdateTimeMax = 6;

			if (MouseX >= (this->m_MiniMapFramePos[0] + 107.0f)
			    && MouseX <= (this->m_MiniMapFramePos[0] + 126.0f)
			    && MouseY >= (this->m_MiniMapFramePos[1] + 15.0f)
			    && MouseY <= (this->m_MiniMapFramePos[1] + 35.0f))
			{
				this->m_iMiniMapScale = (this->m_iMiniMapScale == 1) ? 2 : 1;

				PlayBuffer(25, 0, 0);
			}
		}
	}
}

void CMiniMap::RenderObjects()
{
	DWORD ViewportAddress;

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

		if (*(BYTE*)(ViewportAddress + 0x84) == 1)
		{
			glColor3f(0.8f, 0.0f, 0.0f);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 2)
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		else if (*(BYTE*)(ViewportAddress + 0x84) == 4)
		{
			glColor3f(1.0f, 0.0f, 1.0f);
		}
		else
		{
			continue;
		}

		if (ViewportAddress == Hero)
		{
			glColor3f(1.0f, 1.0f, 0.0f);
		}

		float CharPosX = this->m_MiniMapFramePos[0] + ((*(float*)(ViewportAddress + 0x10) / 100.0f) / this->m_iMiniMapScale) - this->m_MiniMapScaleOffset[0];

		float CharPosY = this->m_MiniMapFramePos[1] + 46.0f + ((256.0f - (*(float*)(ViewportAddress + 0x14) / 100.0f)) / this->m_iMiniMapScale) - this->m_MiniMapScaleOffset[1];

		RenderColor(CharPosX, CharPosY, 3.0f, 3.0f);
	}

	DisableAlphaBlend();

	glColor3f(1.0f, 1.0f, 1.0f);
}