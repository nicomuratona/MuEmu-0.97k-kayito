#include "stdafx.h"
#include "GuildLogo.h"

CGuildLogo gGuildLogo;

CGuildLogo::CGuildLogo()
{

}

CGuildLogo::~CGuildLogo()
{

}

void CGuildLogo::Init()
{
	SetCompleteHook(0xE8, 0x004BD28B, &this->DrawGuildLogo);
}

void CGuildLogo::DrawGuildLogo(int x, int y, DWORD c)
{
	DWORD character = *(DWORD*)(c + 564); // Get Character Struct

	if (*(BYTE*)(character + 0x84) == 1) // Check Character Type
	{
		if (*(short*)(character + 0x1DA) != -1) // Check Character Guild
		{
			CreateGuildMark(*(WORD*)(character + 0x1DA), true); // CreateGuildMark

			RenderBitmap(0x22, ((float)x - 21.0f), (float)y, 16.0f, 16.0f, 0.0f, 0.0f, 1.0f, 1.0f, true, true); // RenderBitmap
		}
	}

	RenderBoolean(x, y, c); //RenderBoolean
}