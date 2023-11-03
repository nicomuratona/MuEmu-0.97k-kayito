#include "stdafx.h"
#include "LoadModels.h"
#include "CustomGlow.h"
#include "CustomItem.h"

CLoadModels gLoadModels;

CLoadModels::CLoadModels()
{
	memset(this->lpTextures, 0, sizeof(this->lpTextures));
}

CLoadModels::~CLoadModels()
{

}

void CLoadModels::Init()
{
	this->SetTexturesOffset();

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

void CLoadModels::OpenItemTexturesHook()
{
	TextureBegin = 0;

	TextureCurrent = 5500;

	char modelFolder[MAX_PATH];

	int n = 0;

	for (n = 0; n < MAX_ITEM; n++)
	{
		if (gCustomItem.m_CustomItemInfo[n].ItemIndex != -1)
		{
			if (gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7, 0) && gCustomItem.m_CustomItemInfo[n].ItemIndex < GET_ITEM(12, 0))
			{
				wsprintf(modelFolder, "Player\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex % MAX_ITEM_TYPE);
			}
			else
			{
				wsprintf(modelFolder, "Item\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex);
			}

			gLoadModels.LoadModel((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), modelFolder, gCustomItem.m_CustomItemInfo[n].ModelName, -1);

			gLoadModels.LoadTexture((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), modelFolder, gCustomItem.m_CustomItemInfo[n].ModelName);
		}
	}

	OpenItemTextures();
}

void CLoadModels::LoadModel(int index, char* folder, char* name, int value)
{
	if (name[0] == 0)
	{
		return;
	}

	char path[MAX_PATH] = { 0 };

	wsprintf(path, "Data\\%s", folder);

	AccessModel(index, path, name, value);
}

void CLoadModels::LoadTexture(int index, char* folder, char* name)
{
	if (name[0] == 0)
	{
		return;
	}

	OpenTexture(index, folder, GL_NEAREST, GL_TRUE);
}

void CLoadModels::PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon)
{
	if (gCustomGlow.GetItemGlow((Type - ITEM_BASE_MODEL), Light) == false)
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}