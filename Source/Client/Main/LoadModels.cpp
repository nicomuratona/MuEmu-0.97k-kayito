#include "stdafx.h"
#include "LoadModels.h"
#include "CustomGlow.h"
#include "CustomItem.h"

CLoadModels gLoadModels;

CLoadModels::CLoadModels()
{
	memset(this->lpTextures, 0, sizeof(this->lpTextures));

	this->TextureCount = MAX_TEXTURE;

	this->TextureOverFlow = 0;

	this->TextureType = 0;
}

CLoadModels::~CLoadModels()
{

}

void CLoadModels::Init()
{
	this->SetTexturesOffset();

	SetCompleteHook(0xE9, 0x005060B0, &this->MyAccessModel);

	SetCompleteHook(0xE9, 0x00505C80, &this->MyOpenTexture);

	SetCompleteHook(0xE8, 0x00510DD6, &this->OpenPlayerTexturesHook);

	SetCompleteHook(0xE8, 0x00510DE0, &this->OpenItemTexturesHook);

	SetCompleteHook(0xE8, 0x0045A038, &this->PartObjectColorHook);

	SetCompleteHook(0xE8, 0x00504A02, &this->PartObjectColorHook);
}

void CLoadModels::SetTexturesOffset()
{
	SetDword(0x0050F6CE + 1, (DWORD)(&this->lpTextures));
	SetDword(0x004F89F8 + 3, (DWORD)(&this->lpTextures));
	SetDword(0x00505C16 + 3, (DWORD)(&this->lpTextures));
	SetDword(0x00505C6D + 3, (DWORD)(&this->lpTextures));
	SetDword(0x00505DCE + 3, (DWORD)(&this->lpTextures));
	SetDword(0x00529A43 + 3, (DWORD)(&this->lpTextures));
	SetDword(0x00529EA2 + 3, (DWORD)(&this->lpTextures));
	SetDword(0x0052A068 + 3, (DWORD)(&this->lpTextures));
	/*------------------------------------*/
	SetDword(0x0040FF39 + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x00410917 + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x00478C2C + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x0047970F + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x0047F4EF + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x0047F617 + 2, (DWORD)(&this->lpTextures->Width));
	SetDword(0x004F89F1 + 3, (DWORD)(&this->lpTextures->Width));
	/*------------------------------------*/
	SetDword(0x0040FF21 + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x004107C4 + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x004108FF + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x00410B9D + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x00478508 + 3, (DWORD)(&this->lpTextures->Height));
	SetDword(0x00478C40 + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x00479700 + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x0047F41B + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x0047F4D7 + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x0047F5FF + 2, (DWORD)(&this->lpTextures->Height));
	SetDword(0x004F8296 + 2, (DWORD)(&this->lpTextures->Height));
	/*------------------------------------*/
	SetDword(0x00409FAC + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0040A03C + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0040A797 + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0040A7EE + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0040FF45 + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x00441064 + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x00441097 + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x004416C3 + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x004416FB + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x00478C35 + 2, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0047955F + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x0047F4FB + 3, (DWORD)(&this->lpTextures->Component));
	SetDword(0x00511F12 + 3, (DWORD)(&this->lpTextures->Component));
	/*------------------------------------*/
	SetDword(0x0047F4C0 + 1, (DWORD)(&this->lpTextures->Texture));
	SetDword(0x005114A0 + 3, (DWORD)(&this->lpTextures->Texture));
	/*------------------------------------*/
	SetDword(0x0040F704 + 2, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x0040FE8F + 1, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x0040FF1B + 2, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x0047F434 + 2, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x0047F4D1 + 2, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x004F8282 + 2, (DWORD)(&this->lpTextures->Buffer));
	SetDword(0x0052A05F + 3, (DWORD)(&this->lpTextures->Buffer));
	/*------------------------------------*/
	SetDword(0x004F0103 + 2, (DWORD)(&this->lpTextures) + 1936);
	SetDword(0x004F036A + 2, (DWORD)(&this->lpTextures) + 1936);
	/*------------------------------------*/
	SetDword(0x004F0112 + 2, (DWORD)(&this->lpTextures) + 1940);
	SetDword(0x004F0378 + 2, (DWORD)(&this->lpTextures) + 1940);
	/*------------------------------------*/
	SetDword(0x004F0281 + 3, (DWORD)(&this->lpTextures) + 1944);
	SetDword(0x004F041D + 3, (DWORD)(&this->lpTextures) + 1944);
	/*------------------------------------*/
	SetDword(0x004F026A + 1, (DWORD)(&this->lpTextures) + 1948);
	SetDword(0x004F0406 + 1, (DWORD)(&this->lpTextures) + 1948);
	/*------------------------------------*/
	SetDword(0x004F0123 + 2, (DWORD)(&this->lpTextures) + 1956);
	SetDword(0x004F027B + 2, (DWORD)(&this->lpTextures) + 1956);
	SetDword(0x004F0389 + 2, (DWORD)(&this->lpTextures) + 1956);
	SetDword(0x004F0417 + 2, (DWORD)(&this->lpTextures) + 1956);
	/*------------------------------------*/
	SetDword(0x004F7DFE + 3, (DWORD)(&this->lpTextures) + 1960);
	/*------------------------------------*/
	SetDword(0x004C106C + 3, (DWORD)(&this->lpTextures) + 2000);
	/*------------------------------------*/
	SetDword(0x0050CB5A + 1, (DWORD)(&this->lpTextures) + 3640);
	/*------------------------------------*/
	SetDword(0x0050CB5F + 2, (DWORD)(&this->lpTextures) + 4200);
	SetDword(0x0050CBA5 + 2, (DWORD)(&this->lpTextures) + 4200);
	/*------------------------------------*/
	SetDword(0x0050CBEF + 2, (DWORD)(&this->lpTextures) + 5432);
}

void CLoadModels::MyAccessModel(int Type, char* Dir, char* FileName, int i)
{
	char Name[64];

	if (i == -1)
	{
		wsprintf(Name, "%s.bmd", FileName);
	}
	else if (i < 10)
	{
		wsprintf(Name, "%s0%d.bmd", FileName, i);
	}
	else
	{
		wsprintf(Name, "%s%d.bmd", FileName, i);
	}

	gLoadModels.CheckModelExists(Type, Dir, Name);

	bool Success = ((bool(__thiscall*)(DWORD This, char* DirName, char* FileName))0x004423E0)(Models + 188 * Type, Dir, Name);

	if (Success == false)
	{
		if (strcmp(FileName, "Monster") == NULL || strcmp(FileName, "Player") == NULL || strcmp(FileName, "PlayerTest") == NULL || strcmp(FileName, "Angel") == NULL)
		{
			char Text[256];

			wsprintf(Text, "[Model] '%s%s' file does not exist.", Dir, Name);

			MessageBox(g_hWnd, Text, NULL, MB_OK);

			SendMessage(g_hWnd, WM_DESTROY, 0, 0);
		}
	}
}

void CLoadModels::CheckModelExists(int Model, char* SubFolder, char* filename)
{
	char path[256] = { 0, };

	strcpy_s(path, SubFolder);

	strcat_s(path, filename);

	FILE* fp = NULL;

	fopen_s(&fp, path, "rt");

	if (fp == NULL)
	{
		gConsole.Write("[Model] '%s' file does not exist. Model number: %d", path, Model);
	}
	else
	{
		fclose(fp);
	}
}

void CLoadModels::MyOpenTexture(int Model, char* SubFolder, int Type, bool Check)
{
	DWORD pModel = Models + 188 * Model;

	for (int i = 0; i < *(short*)(pModel + 0x24); i++) // pModel->NumMeshs
	{
		char* pTexture = (char*)(*(DWORD*)(pModel + 0x34) + 32 * i);

		DWORD pBitmap = NULL;

		short textureFound = gLoadModels.TextureCheck(pTexture, &pBitmap);

		if (textureFound < 0)
		{
			char path[256] = { 0, };

			strcpy_s(path, SubFolder);

			strcat_s(path, pTexture);

			char __filename[_MAX_FNAME] = { 0, };

			char __ext[_MAX_EXT] = { 0, };

			_splitpath_s(pTexture, NULL, NULL, NULL, NULL, __filename, _MAX_FNAME, __ext, _MAX_EXT);

			gLoadModels.CheckTextureExists(Model, SubFolder, __filename, __ext);

			if (tolower(__ext[1]) == 't')
			{
				OpenTGA(path, TextureCurrent, GL_NEAREST, GL_REPEAT, (char*)(pModel + 0), Check);
			}
			else
			{
				OpenJPG(path, TextureCurrent, Type, GL_REPEAT, (char*)(pModel + 0), Check);
			}

			MemoryCpy(((DWORD)&gLoadModels.lpTextures + 56 * TextureCurrent), pTexture, 32);

			*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = TextureCurrent;

			TextureCurrent += 1;
		}
		else
		{
			if (pBitmap)
			{
				*(BYTE*)(pBitmap + 0x30) += 1;
			}

			*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = textureFound;
		}

		if (pTexture[0] == 's'
		    && pTexture[1] == 'k'
		    && pTexture[2] == 'i')
		{
			*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = 301;
		}
		else if (!_strnicmp(pTexture, "level", 5))
		{
			*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = 301;
		}
		else if (pTexture[0] == 'h'
			 && pTexture[1] == 'i'
			 && pTexture[2] == 'd')
		{
			*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = 300;
		}
	}
}

short CLoadModels::TextureCheck(char* name, DWORD* index)
{
	if (this->TextureOverFlow == 0)
	{
		switch (this->TextureType)
		{
			case 1:
			{
				if (TextureCurrent >= 490)
				{
					this->TextureOverFlow = 1;
				}

				break;
			}

			case 2:
			{
				if (TextureCurrent >= 688)
				{
					this->TextureOverFlow = 1;
				}

				break;
			}
		}
	}

	if (this->TextureOverFlow == 1)
	{
		TextureCurrent = this->TextureCount;

		this->TextureOverFlow = 2;
	}

	return FindTextureByName(name, index);
}

void CLoadModels::CheckTextureExists(int Model, char* SubFolder, char* filename, char* ext)
{
	char path[256] = { 0, };

	strcpy_s(path, "Data\\");

	strcat_s(path, SubFolder);

	strcat_s(path, filename);

	if (tolower(ext[1]) == 't')
	{
		strcat_s(path, ".ozt");
	}
	else
	{
		strcat_s(path, ".ozj");
	}

	FILE* fp = NULL;

	fopen_s(&fp, path, "rt");

	if (fp == NULL)
	{
		gConsole.Write("[Texture] '%s' file does not exist. Model number: %d", path, Model);
	}
	else
	{
		fclose(fp);
	}
}

void CLoadModels::OpenPlayerTexturesHook()
{
	TextureCurrent = 301;

	gLoadModels.TextureType = 1;

	OpenPlayerTextures();

	if (gLoadModels.TextureOverFlow)
	{
		gLoadModels.TextureOverFlow = 0;

		gLoadModels.TextureCount = TextureCurrent;
	}

	gLoadModels.TextureType = 0;
}

void CLoadModels::OpenItemTexturesHook()
{
	TextureCurrent = 500;

	gLoadModels.TextureType = 2;

	OpenItemTextures();

	char modelFolder[MAX_PATH];

	char textureFolder[MAX_PATH];

	int n = 0;

	for (n = 0; n < MAX_ITEM; n++)
	{
		if (gCustomItem.m_CustomItemInfo[n].ItemIndex != -1)
		{
			if (gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7, 0) && gCustomItem.m_CustomItemInfo[n].ItemIndex < GET_ITEM(12, 0))
			{
				wsprintf(modelFolder, "Data\\Player\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex % MAX_ITEM_TYPE);

				wsprintf(textureFolder, "Player\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex % MAX_ITEM_TYPE);
			}
			else
			{
				wsprintf(modelFolder, "Data\\Item\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex);

				wsprintf(textureFolder, "Item\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex);
			}

			gLoadModels.MyAccessModel((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), modelFolder, gCustomItem.m_CustomItemInfo[n].ModelName);

			gLoadModels.MyOpenTexture((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), textureFolder);
		}
	}

	if (gLoadModels.TextureOverFlow)
	{
		gLoadModels.TextureOverFlow = 0;

		gLoadModels.TextureCount = TextureCurrent;
	}

	gLoadModels.TextureType = 0;
}

void CLoadModels::PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon)
{
	if (!gCustomGlow.GetItemGlow((Type - ITEM_BASE_MODEL), Light))
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}