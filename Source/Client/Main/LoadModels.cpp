#include "stdafx.h"
#include "LoadModels.h"
#include "CustomGlow.h"
#include "CustomItem.h"

CLoadModels gLoadModels;

CLoadModels::CLoadModels()
{
	memset(this->lpTextures, 0, sizeof(this->lpTextures));

	this->TextureCount = MAX_TEXTURE;
}

CLoadModels::~CLoadModels()
{

}

void CLoadModels::Init()
{
	this->SetTexturesOffset();

	SetCompleteHook(0xE8, 0x00510DDB, &this->OpenItemsHook);

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
	SetDword(0x00409FAC + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0040A03C + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0040A797 + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0040A7EE + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0040FF45 + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x00441064 + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x00441097 + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x004416C3 + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x004416FB + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x00478C35 + 2, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0047955F + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x0047F4FB + 3, (DWORD)(&this->lpTextures->Components));
	SetDword(0x00511F12 + 3, (DWORD)(&this->lpTextures->Components));
	/*------------------------------------*/
	SetDword(0x0047F4C0 + 1, (DWORD)(&this->lpTextures->TextureNumber));
	SetDword(0x005114A0 + 3, (DWORD)(&this->lpTextures->TextureNumber));
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

	this->CheckModelExists(Type, Dir, Name);

	bool Success = ((bool(__thiscall*)(DWORD This, char* DirName, char* FileName))0x004423E0)(Models + 188 * Type, Dir, Name);

	if (!Success)
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

		char path[256] = { 0, };

		strcpy_s(path, SubFolder);

		strcat_s(path, pTexture);

		char __filename[_MAX_FNAME] = { 0, };

		char __ext[_MAX_EXT] = { 0, };

		_splitpath_s(pTexture, NULL, NULL, NULL, NULL, __filename, _MAX_FNAME, __ext, _MAX_EXT);

		this->CheckTextureExists(Model, SubFolder, __filename, __ext);

		if (tolower(__ext[1]) == 't')
		{
			OpenTGA(path, this->TextureCount, GL_NEAREST, GL_REPEAT, (char*)(pModel + 0), true);
		}
		else
		{
			OpenJPG(path, this->TextureCount, Type, GL_REPEAT, (char*)(pModel + 0), true);
		}

		strcpy_s(this->lpTextures[this->TextureCount].FileName, pTexture);

		*(short*)(*(DWORD*)(pModel + 0x38) + 2 * i) = this->TextureCount; // pModel->IndexTexture[i]

		this->TextureCount++;

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

void CLoadModels::OpenItemsHook()
{
	/************************** Swords **************************/
	AccessModel(MODEL_SWORD + 0, "Data\\Item\\", "Sword", 1); // Kris
	AccessModel(MODEL_SWORD + 1, "Data\\Item\\", "Sword", 2); // Short Sword
	AccessModel(MODEL_SWORD + 2, "Data\\Item\\", "Sword", 3); // Rapier
	AccessModel(MODEL_SWORD + 3, "Data\\Item\\", "Sword", 4); // Katana
	AccessModel(MODEL_SWORD + 4, "Data\\Item\\", "Sword", 5); // Sword of Assassin
	AccessModel(MODEL_SWORD + 5, "Data\\Item\\", "Sword", 6); // Blade
	AccessModel(MODEL_SWORD + 6, "Data\\Item\\", "Sword", 7); // Gladius
	AccessModel(MODEL_SWORD + 7, "Data\\Item\\", "Sword", 8); // Falchion
	AccessModel(MODEL_SWORD + 8, "Data\\Item\\", "Sword", 9); // Serpent Sword
	AccessModel(MODEL_SWORD + 9, "Data\\Item\\", "Sword", 10); // Sword of Salamander
	AccessModel(MODEL_SWORD + 10, "Data\\Item\\", "Sword", 11); // Light Saber
	AccessModel(MODEL_SWORD + 11, "Data\\Item\\", "Sword", 12); // Legendary Sword
	AccessModel(MODEL_SWORD + 12, "Data\\Item\\", "Sword", 13); // Heliacal Sword
	AccessModel(MODEL_SWORD + 13, "Data\\Item\\", "Sword", 14); // Double Blade
	AccessModel(MODEL_SWORD + 14, "Data\\Item\\", "Sword", 15); // Lightning Sword
	AccessModel(MODEL_SWORD + 15, "Data\\Item\\", "Sword", 16); // Giant Sword
	AccessModel(MODEL_SWORD + 16, "Data\\Item\\", "Sword", 17); // Sword of Destruction
	AccessModel(MODEL_SWORD + 17, "Data\\Item\\", "Sword", 18); // Dark Breaker
	AccessModel(MODEL_SWORD + 18, "Data\\Item\\", "Sword", 19); // Thunder Blade
	AccessModel(MODEL_SWORD + 19, "Data\\Item\\", "Sword", 20); // Divine Sword of Archangel

	AccessModel(MODEL_SWORD + 31, "Data\\Item\\", "Sword", 32); // Rune Blade

	/************************** Axes **************************/
	AccessModel(MODEL_AXE + 0, "Data\\Item\\", "Axe", 1); // Small Axe
	AccessModel(MODEL_AXE + 1, "Data\\Item\\", "Axe", 2); // Hand Axe
	AccessModel(MODEL_AXE + 2, "Data\\Item\\", "Axe", 3); // Double Axe
	AccessModel(MODEL_AXE + 3, "Data\\Item\\", "Axe", 4); // Tomahawk
	AccessModel(MODEL_AXE + 4, "Data\\Item\\", "Axe", 5); // Elven Axe
	AccessModel(MODEL_AXE + 5, "Data\\Item\\", "Axe", 6); // Battle Axe
	AccessModel(MODEL_AXE + 6, "Data\\Item\\", "Axe", 7); // Nikea Axe
	AccessModel(MODEL_AXE + 7, "Data\\Item\\", "Axe", 8); // Larkan Axe
	AccessModel(MODEL_AXE + 8, "Data\\Item\\", "Axe", 9); // Crescent Axe

	/************************** Maces **************************/
	AccessModel(MODEL_MACE + 0, "Data\\Item\\", "Mace", 1); // Mace
	AccessModel(MODEL_MACE + 1, "Data\\Item\\", "Mace", 2); // Morning Star
	AccessModel(MODEL_MACE + 2, "Data\\Item\\", "Mace", 3); // Flail
	AccessModel(MODEL_MACE + 3, "Data\\Item\\", "Mace", 4); // Great Hammer
	AccessModel(MODEL_MACE + 4, "Data\\Item\\", "Mace", 5); // Crystal Morning Star
	AccessModel(MODEL_MACE + 5, "Data\\Item\\", "Mace", 6); // Crystal Sword
	AccessModel(MODEL_MACE + 6, "Data\\Item\\", "Mace", 7); // Chaos Dragon Axe
	AccessModel(MODEL_MACE + 7, "Data\\Item\\", "Mace", 8); // Elemental Mace

	/************************** Spears **************************/
	AccessModel(MODEL_SPEAR + 0, "Data\\Item\\", "Spear", 1); // Light Spear
	AccessModel(MODEL_SPEAR + 1, "Data\\Item\\", "Spear", 2); // Spear
	AccessModel(MODEL_SPEAR + 2, "Data\\Item\\", "Spear", 3); // Dragon Lance
	AccessModel(MODEL_SPEAR + 3, "Data\\Item\\", "Spear", 4); // Giant Trident
	AccessModel(MODEL_SPEAR + 4, "Data\\Item\\", "Spear", 5); // Serpent Spear
	AccessModel(MODEL_SPEAR + 5, "Data\\Item\\", "Spear", 6); // Double Poleaxe
	AccessModel(MODEL_SPEAR + 6, "Data\\Item\\", "Spear", 7); // Halberd
	AccessModel(MODEL_SPEAR + 7, "Data\\Item\\", "Spear", 8); // Berdysh
	AccessModel(MODEL_SPEAR + 8, "Data\\Item\\", "Spear", 9); // Great Scythe
	AccessModel(MODEL_SPEAR + 9, "Data\\Item\\", "Spear", 10); // Bill of Balrog
	AccessModel(MODEL_SPEAR + 10, "Data\\Item\\", "Spear", 11); // Dragon Spear

	/************************** Bows/Crossbows **************************/
	AccessModel(MODEL_BOW + 0, "Data\\Item\\", "Bow", 1); // Short Bow
	AccessModel(MODEL_BOW + 1, "Data\\Item\\", "Bow", 2); // Bow
	AccessModel(MODEL_BOW + 2, "Data\\Item\\", "Bow", 3); // Elven Bow
	AccessModel(MODEL_BOW + 3, "Data\\Item\\", "Bow", 4); // Battle Bow
	AccessModel(MODEL_BOW + 4, "Data\\Item\\", "Bow", 5); // Tiger Bow
	AccessModel(MODEL_BOW + 5, "Data\\Item\\", "Bow", 6); // Silver Bow
	AccessModel(MODEL_BOW + 6, "Data\\Item\\", "Bow", 7); // Chaos Nature Bow

	AccessModel(MODEL_BOW + 7, "Data\\Item\\", "Arrows", 1); // Bolts

	AccessModel(MODEL_BOW + 8, "Data\\Item\\", "CrossBow", 1); // Crossbow
	AccessModel(MODEL_BOW + 9, "Data\\Item\\", "CrossBow", 2); // Golden Crossbow
	AccessModel(MODEL_BOW + 10, "Data\\Item\\", "CrossBow", 3); // Arquebus
	AccessModel(MODEL_BOW + 11, "Data\\Item\\", "CrossBow", 4); // Light Crossbow
	AccessModel(MODEL_BOW + 12, "Data\\Item\\", "CrossBow", 5); // Serpent Crossbow
	AccessModel(MODEL_BOW + 13, "Data\\Item\\", "CrossBow", 6); // Bluewing Crossbow
	AccessModel(MODEL_BOW + 14, "Data\\Item\\", "CrossBow", 7); // Aquagold Crossbow

	AccessModel(MODEL_BOW + 15, "Data\\Item\\", "Arrows", 2); // Arrows

	AccessModel(MODEL_BOW + 16, "Data\\Item\\", "CrossBow", 17); // Saint Crossbow
	AccessModel(MODEL_BOW + 17, "Data\\Item\\", "Bow", 18); // Celestial Bow
	AccessModel(MODEL_BOW + 18, "Data\\Item\\", "Bow", 19); // Divine Crossbow of Archangel

	/************************** Staffs **************************/
	AccessModel(MODEL_STAFF + 0, "Data\\Item\\", "Staff", 1); // Skull Staff
	AccessModel(MODEL_STAFF + 1, "Data\\Item\\", "Staff", 2); // Angelic Staff
	AccessModel(MODEL_STAFF + 2, "Data\\Item\\", "Staff", 3); // Serpent Staff
	AccessModel(MODEL_STAFF + 3, "Data\\Item\\", "Staff", 4); // Thunder Staff
	AccessModel(MODEL_STAFF + 4, "Data\\Item\\", "Staff", 5); // Gorgon Staff
	AccessModel(MODEL_STAFF + 5, "Data\\Item\\", "Staff", 6); // Legendary Staff
	AccessModel(MODEL_STAFF + 6, "Data\\Item\\", "Staff", 7); // Staff of Resurrection
	AccessModel(MODEL_STAFF + 7, "Data\\Item\\", "Staff", 8); // Chaos Lightning Staff
	AccessModel(MODEL_STAFF + 8, "Data\\Item\\", "Staff", 9); // Staff of Destruction
	AccessModel(MODEL_STAFF + 9, "Data\\Item\\", "Staff", 10); // Dragon Soul Staff
	AccessModel(MODEL_STAFF + 10, "Data\\Item\\", "Staff", 11); // Divine Staff of Archangel

	/************************** Shields **************************/
	AccessModel(MODEL_SHIELD + 0, "Data\\Item\\", "Shield", 1); // Small Shield
	AccessModel(MODEL_SHIELD + 1, "Data\\Item\\", "Shield", 2); // Horn Shield
	AccessModel(MODEL_SHIELD + 2, "Data\\Item\\", "Shield", 3); // Kite Shield
	AccessModel(MODEL_SHIELD + 3, "Data\\Item\\", "Shield", 4); // Elven Shield
	AccessModel(MODEL_SHIELD + 4, "Data\\Item\\", "Shield", 5); // Buckler
	AccessModel(MODEL_SHIELD + 5, "Data\\Item\\", "Shield", 6); // Dragon Slayer Shield
	AccessModel(MODEL_SHIELD + 6, "Data\\Item\\", "Shield", 7); // Skull Shield
	AccessModel(MODEL_SHIELD + 7, "Data\\Item\\", "Shield", 8); // Spiked Shield
	AccessModel(MODEL_SHIELD + 8, "Data\\Item\\", "Shield", 9); // Tower Shield
	AccessModel(MODEL_SHIELD + 9, "Data\\Item\\", "Shield", 10); // Plate Shield
	AccessModel(MODEL_SHIELD + 10, "Data\\Item\\", "Shield", 11); // Large Round Shield
	AccessModel(MODEL_SHIELD + 11, "Data\\Item\\", "Shield", 12); // Serpent Shield
	AccessModel(MODEL_SHIELD + 12, "Data\\Item\\", "Shield", 13); // Bronze Shield
	AccessModel(MODEL_SHIELD + 13, "Data\\Item\\", "Shield", 14); // Dragon Shield
	AccessModel(MODEL_SHIELD + 14, "Data\\Item\\", "Shield", 15); // Legendary Shield
	AccessModel(MODEL_SHIELD + 15, "Data\\Item\\", "Shield", 16); // Grand Soul Shield
	AccessModel(MODEL_SHIELD + 16, "Data\\Item\\", "Shield", 17); // Elemental Shield

	/************************** Wings **************************/
	AccessModel(MODEL_WING + 0, "Data\\Item\\", "Wing", 1); // Wings of Elf
	AccessModel(MODEL_WING + 1, "Data\\Item\\", "Wing", 2); // Wings of Heaven
	AccessModel(MODEL_WING + 2, "Data\\Item\\", "Wing", 3); // Wings of Satan

	AccessModel(MODEL_WING + 3, "Data\\Item\\", "Wing", 4); // Wings of Spirits
	AccessModel(MODEL_WING + 4, "Data\\Item\\", "Wing", 5); // Wings of Soul
	AccessModel(MODEL_WING + 5, "Data\\Item\\", "Wing", 6); // Wings of Dragon
	AccessModel(MODEL_WING + 6, "Data\\Item\\", "Wing", 7); // Wings of Darkness

	/************************** Pets **************************/
	AccessModel(MODEL_HELPER + 0, "Data\\Player\\", "Helper", 1); // Guardian Angel
	AccessModel(MODEL_HELPER + 1, "Data\\Player\\", "Helper", 2); // Imp
	AccessModel(MODEL_HELPER + 2, "Data\\Player\\", "Helper", 3); // Horn of Uniria
	AccessModel(MODEL_HELPER + 3, "Data\\Player\\", "Helper", 4); // Horn of Dinorant

	/************************** Rings **************************/
	AccessModel(MODEL_HELPER + 8, "Data\\Item\\", "Ring", 1); // Ring of Ice
	AccessModel(MODEL_HELPER + 9, "Data\\Item\\", "Ring", 2); // Ring of Poison
	AccessModel(MODEL_HELPER + 10, "Data\\Item\\", "Ring", 1); // Transformation Ring
	AccessModel(MODEL_HELPER + 12, "Data\\Item\\", "Necklace", 1); // Pendant of Lightning
	AccessModel(MODEL_HELPER + 13, "Data\\Item\\", "Necklace", 2); // Pendant of Fire

	/************************** Consumables **************************/
	AccessModel(MODEL_POTION + 0, "Data\\Item\\", "Potion", 1); // Apple
	AccessModel(MODEL_POTION + 1, "Data\\Item\\", "Potion", 2); // Small Healing Potion
	AccessModel(MODEL_POTION + 2, "Data\\Item\\", "Potion", 3); // Medium Healing Potion
	AccessModel(MODEL_POTION + 3, "Data\\Item\\", "Potion", 4); // Large Healing Potion
	AccessModel(MODEL_POTION + 4, "Data\\Item\\", "Potion", 5); // Small Mana Potion
	AccessModel(MODEL_POTION + 5, "Data\\Item\\", "Potion", 6); // Medium Mana Potion
	AccessModel(MODEL_POTION + 6, "Data\\Item\\", "Potion", 7); // Large Mana Potion
	AccessModel(MODEL_POTION + 8, "Data\\Item\\", "Antidote", 1); // Antidote
	AccessModel(MODEL_POTION + 9, "Data\\Item\\", "Beer", 1); // Ale

	AccessModel(MODEL_POTION + 10, "Data\\Item\\", "Scroll", 1); // Town Portal Scroll

	AccessModel(MODEL_POTION + 20, "Data\\Item\\", "Drink", 0); // Remedy of Love

	/************************** Dropables **************************/
	AccessModel(MODEL_POTION + 11, "Data\\Item\\", "MagicBox", 1); // Box of Luck
	AccessModel(MODEL_POTION + 12, "Data\\Item\\", "Event", 1); // Heart

	AccessModel(MODEL_POTION + 15, "Data\\Item\\", "Gold", 1); // Zen

	/************************** Jewels **************************/
	AccessModel(MODEL_WING + 15, "Data\\Item\\", "Jewel", 15); // Jewel of Chaos
	AccessModel(MODEL_POTION + 13, "Data\\Item\\", "Jewel", 1); // Jewel of Bless
	AccessModel(MODEL_POTION + 14, "Data\\Item\\", "Jewel", 2); // Jewel of Soul
	AccessModel(MODEL_POTION + 16, "Data\\Item\\", "Jewel", 3); // Jewel of Life
	AccessModel(MODEL_POTION + 22, "Data\\Item\\", "Jewel", 22); // Jewel of Creation

	/************************** Events **************************/
	AccessModel(MODEL_HELPER + 16, "Data\\Item\\", "EventBloodCastle", 0); // Scroll of Archangel
	AccessModel(MODEL_HELPER + 17, "Data\\Item\\", "EventBloodCastle", 1); // Blood Bone
	AccessModel(MODEL_HELPER + 18, "Data\\Item\\", "EventBloodCastle", 2); // Cloak of Invisibility

	AccessModel(MODEL_POTION + 17, "Data\\Item\\", "Devil", 0); // Devil's Eye
	AccessModel(MODEL_POTION + 18, "Data\\Item\\", "Devil", 1); // Devil's Key
	AccessModel(MODEL_POTION + 19, "Data\\Item\\", "Devil", 2); // Devil's Invitation

	AccessModel(MODEL_POTION + 21, "Data\\Item\\", "ConChip", 0); // Rena

	/************************** Events2 **************************/
	AccessModel(MODEL_EVENT + 0, "Data\\Item\\", "Event", 2); // Mu Coin
	AccessModel(MODEL_EVENT + 1, "Data\\Item\\", "Event", 3); // Mu Scroll
	AccessModel(MODEL_EVENT + 4, "Data\\Item\\", "MagicBox", 2); // Christmas Star
	AccessModel(MODEL_EVENT + 5, "Data\\Item\\", "MagicBox", 4); // Magic pouch
	AccessModel(MODEL_EVENT + 6, "Data\\Item\\", "MagicBox", 5); // Love Heart
	AccessModel(MODEL_EVENT + 7, "Data\\Item\\", "Beer", 2); // Olive of Love
	AccessModel(MODEL_EVENT + 8, "Data\\Item\\", "MagicBox", 6); // Silver medal
	AccessModel(MODEL_EVENT + 9, "Data\\Item\\", "MagicBox", 7); // Gold Medal
	AccessModel(MODEL_EVENT + 10, "Data\\Item\\", "MagicBox", 8); // Eldorado
	AccessModel(MODEL_EVENT + 11, "Data\\Item\\", "EventBloodCastle", 3); // Stone

	/************************** Quest **************************/
	AccessModel(MODEL_POTION + 23, "Data\\Item\\", "Quest", 0); // Scroll of Emperor
	AccessModel(MODEL_POTION + 24, "Data\\Item\\", "Quest", 1); // Broken Sword
	AccessModel(MODEL_POTION + 25, "Data\\Item\\", "Quest", 2); // Tear of Elf
	AccessModel(MODEL_POTION + 26, "Data\\Item\\", "Quest", 3); // Soul Shard of Wizard

	AccessModel(MODEL_HELPER + 14, "Data\\Item\\", "Quest", 4); // Loch's Feather
	AccessModel(MODEL_HELPER + 15, "Data\\Item\\", "Quest", 5); // Fruit

	/************************** Orbs **************************/
	AccessModel(MODEL_WING + 7, "Data\\Item\\", "Gem", 1); // Orb of Twisting Slash
	AccessModel(MODEL_WING + 8, "Data\\Item\\", "Gem", 2); // Healing Orb
	AccessModel(MODEL_WING + 9, "Data\\Item\\", "Gem", 3); // Orb of Greater Defense
	AccessModel(MODEL_WING + 10, "Data\\Item\\", "Gem", 4); // Orb of Greater Damage
	AccessModel(MODEL_WING + 11, "Data\\Item\\", "Gem", 5); // Orb of Summoning
	AccessModel(MODEL_WING + 12, "Data\\Item\\", "Gem", 6); // Orb of Rageful Blow
	AccessModel(MODEL_WING + 13, "Data\\Item\\", "Gem", 7); // Orb of Impale
	AccessModel(MODEL_WING + 14, "Data\\Item\\", "Gem", 8); // Orb of Greater Fortitude
	AccessModel(MODEL_WING + 16, "Data\\Item\\", "Gem", 10); // Orb of Fire Slash
	AccessModel(MODEL_WING + 17, "Data\\Item\\", "Gem", 11); // Orb of Penetration
	AccessModel(MODEL_WING + 18, "Data\\Item\\", "Gem", 12); // Orb of Ice Arrow
	AccessModel(MODEL_WING + 19, "Data\\Item\\", "Gem", 13); // Orb of Death Stab

	/************************** Books **************************/
	AccessModel(MODEL_ETC + 0, "Data\\Item\\", "Book", 1); // Scroll of Poison
	AccessModel(MODEL_ETC + 1, "Data\\Item\\", "Book", 2); // Scroll of Meteorite
	AccessModel(MODEL_ETC + 2, "Data\\Item\\", "Book", 3); // Scroll of Lightning
	AccessModel(MODEL_ETC + 3, "Data\\Item\\", "Book", 4); // Scroll of Fire Ball
	AccessModel(MODEL_ETC + 4, "Data\\Item\\", "Book", 5); // Scroll of Flame
	AccessModel(MODEL_ETC + 5, "Data\\Item\\", "Book", 6); // Scroll of Teleport
	AccessModel(MODEL_ETC + 6, "Data\\Item\\", "Book", 7); // Scroll of Ice
	AccessModel(MODEL_ETC + 7, "Data\\Item\\", "Book", 8); // Scroll of Twister
	AccessModel(MODEL_ETC + 8, "Data\\Item\\", "Book", 9); // Scroll of Evil Spirit
	AccessModel(MODEL_ETC + 9, "Data\\Item\\", "Book", 10); // Scroll of Hellfire
	AccessModel(MODEL_ETC + 10, "Data\\Item\\", "Book", 11); // Scroll of Power Wave
	AccessModel(MODEL_ETC + 11, "Data\\Item\\", "Book", 12); // Scroll of Aqua Beam
	AccessModel(MODEL_ETC + 12, "Data\\Item\\", "Book", 13); // Scroll of Cometfall
	AccessModel(MODEL_ETC + 13, "Data\\Item\\", "Book", 14); // Scroll of Inferno
	AccessModel(MODEL_ETC + 14, "Data\\Item\\", "Book", 15); // Scroll of Teleport Ally
	AccessModel(MODEL_ETC + 15, "Data\\Item\\", "Book", 16); // Scroll of Soul Barrier

	/************************** Blend mesh without chrome effects **************************/

	*(BYTE*)(*(DWORD*)(Models + 93288) + 40) = 1; // Light stick.
	*(BYTE*)(*(DWORD*)(Models + 77120) + 40) = 1; // Sword of light.
	*(BYTE*)(*(DWORD*)(Models + 106448) + 80) = 1; // Staff of Resurrection.
	*(BYTE*)(*(DWORD*)(Models + 88400) + 40) = 1; // Chaos Axe.
	*(BYTE*)(*(DWORD*)(Models + 179768) + 40) = 1; // Gold Medal

	/************************** Customs **************************/

	int n = 0;

	char modelFolder[MAX_PATH];

	for (n = 0; n < MAX_ITEM; n++)
	{
		if (gCustomItem.m_CustomItemInfo[n].ItemIndex != -1)
		{
			if (gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7, 0) && gCustomItem.m_CustomItemInfo[n].ItemIndex < GET_ITEM(12, 0))
			{
				wsprintf(modelFolder, "Data\\Player\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex % MAX_ITEM_TYPE);
			}
			else
			{
				wsprintf(modelFolder, "Data\\Item\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex);
			}

			gLoadModels.MyAccessModel((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), modelFolder, gCustomItem.m_CustomItemInfo[n].ModelName);
		}
	}
}

void CLoadModels::OpenItemTexturesHook()
{
	TextureBegin = 0;
	TextureCurrent = 500;

	/************************** Swords **************************/
	OpenTexture(MODEL_SWORD + 0, "Item\\", GL_NEAREST, true); // Kris
	OpenTexture(MODEL_SWORD + 1, "Item\\", GL_NEAREST, true); // Short Sword
	OpenTexture(MODEL_SWORD + 2, "Item\\", GL_NEAREST, true); // Rapier
	OpenTexture(MODEL_SWORD + 3, "Item\\", GL_NEAREST, true); // Katana
	OpenTexture(MODEL_SWORD + 4, "Item\\", GL_NEAREST, true); // Sword of Assassin
	OpenTexture(MODEL_SWORD + 5, "Item\\", GL_NEAREST, true); // Blade
	OpenTexture(MODEL_SWORD + 6, "Item\\", GL_NEAREST, true); // Gladius
	OpenTexture(MODEL_SWORD + 7, "Item\\", GL_NEAREST, true); // Falchion
	OpenTexture(MODEL_SWORD + 8, "Item\\", GL_NEAREST, true); // Serpent Sword
	OpenTexture(MODEL_SWORD + 9, "Item\\", GL_NEAREST, true); // Sword of Salamander
	OpenTexture(MODEL_SWORD + 10, "Item\\", GL_NEAREST, true); // Light Saber
	OpenTexture(MODEL_SWORD + 11, "Item\\", GL_NEAREST, true); // Legendary Sword
	OpenTexture(MODEL_SWORD + 12, "Item\\", GL_NEAREST, true); // Heliacal Sword
	OpenTexture(MODEL_SWORD + 13, "Item\\", GL_NEAREST, true); // Double Blade
	OpenTexture(MODEL_SWORD + 14, "Item\\", GL_NEAREST, true); // Lightning Sword
	OpenTexture(MODEL_SWORD + 15, "Item\\", GL_NEAREST, true); // Giant Sword
	OpenTexture(MODEL_SWORD + 16, "Item\\", GL_NEAREST, true); // Sword of Destruction
	OpenTexture(MODEL_SWORD + 17, "Item\\", GL_NEAREST, true); // Dark Breaker
	OpenTexture(MODEL_SWORD + 18, "Item\\", GL_NEAREST, true); // Thunder Blade
	OpenTexture(MODEL_SWORD + 19, "Item\\", GL_NEAREST, true); // Divine Sword of Archangel

	OpenTexture(MODEL_SWORD + 31, "Item\\", GL_NEAREST, true); // Rune Blade

	/************************** Axes **************************/
	OpenTexture(MODEL_AXE + 0, "Item\\", GL_NEAREST, true); // Small Axe
	OpenTexture(MODEL_AXE + 1, "Item\\", GL_NEAREST, true); // Hand Axe
	OpenTexture(MODEL_AXE + 2, "Item\\", GL_NEAREST, true); // Double Axe
	OpenTexture(MODEL_AXE + 3, "Item\\", GL_NEAREST, true); // Tomahawk
	OpenTexture(MODEL_AXE + 4, "Item\\", GL_NEAREST, true); // Elven Axe
	OpenTexture(MODEL_AXE + 5, "Item\\", GL_NEAREST, true); // Battle Axe
	OpenTexture(MODEL_AXE + 6, "Item\\", GL_NEAREST, true); // Nikea Axe
	OpenTexture(MODEL_AXE + 7, "Item\\", GL_NEAREST, true); // Larkan Axe
	OpenTexture(MODEL_AXE + 8, "Item\\", GL_NEAREST, true); // Crescent Axe

	/************************** Maces **************************/
	OpenTexture(MODEL_MACE + 0, "Item\\", GL_NEAREST, true); // Mace
	OpenTexture(MODEL_MACE + 1, "Item\\", GL_NEAREST, true); // Morning Star
	OpenTexture(MODEL_MACE + 2, "Item\\", GL_NEAREST, true); // Flail
	OpenTexture(MODEL_MACE + 3, "Item\\", GL_NEAREST, true); // Great Hammer
	OpenTexture(MODEL_MACE + 4, "Item\\", GL_NEAREST, true); // Crystal Morning Star
	OpenTexture(MODEL_MACE + 5, "Item\\", GL_NEAREST, true); // Crystal Sword
	OpenTexture(MODEL_MACE + 6, "Item\\", GL_NEAREST, true); // Chaos Dragon Axe
	OpenTexture(MODEL_MACE + 7, "Item\\", GL_NEAREST, true); // Elemental Mace

	/************************** Spears **************************/
	OpenTexture(MODEL_SPEAR + 0, "Item\\", GL_NEAREST, true); // Light Spear
	OpenTexture(MODEL_SPEAR + 1, "Item\\", GL_NEAREST, true); // Spear
	OpenTexture(MODEL_SPEAR + 2, "Item\\", GL_NEAREST, true); // Dragon Lance
	OpenTexture(MODEL_SPEAR + 3, "Item\\", GL_NEAREST, true); // Giant Trident
	OpenTexture(MODEL_SPEAR + 4, "Item\\", GL_NEAREST, true); // Serpent Spear
	OpenTexture(MODEL_SPEAR + 5, "Item\\", GL_NEAREST, true); // Double Poleaxe
	OpenTexture(MODEL_SPEAR + 6, "Item\\", GL_NEAREST, true); // Halberd
	OpenTexture(MODEL_SPEAR + 7, "Item\\", GL_NEAREST, true); // Berdysh
	OpenTexture(MODEL_SPEAR + 8, "Item\\", GL_NEAREST, true); // Great Scythe
	OpenTexture(MODEL_SPEAR + 9, "Item\\", GL_NEAREST, true); // Bill of Balrog
	OpenTexture(MODEL_SPEAR + 10, "Item\\", GL_NEAREST, true); // Dragon Spear

	/************************** Bows/Crossbows **************************/
	OpenTexture(MODEL_BOW + 0, "Item\\", GL_NEAREST, true); // Short Bow
	OpenTexture(MODEL_BOW + 1, "Item\\", GL_NEAREST, true); // Bow
	OpenTexture(MODEL_BOW + 2, "Item\\", GL_NEAREST, true); // Elven Bow
	OpenTexture(MODEL_BOW + 3, "Item\\", GL_NEAREST, true); // Battle Bow
	OpenTexture(MODEL_BOW + 4, "Item\\", GL_NEAREST, true); // Tiger Bow
	OpenTexture(MODEL_BOW + 5, "Item\\", GL_NEAREST, true); // Silver Bow
	OpenTexture(MODEL_BOW + 6, "Item\\", GL_NEAREST, true); // Chaos Nature Bow

	OpenTexture(MODEL_BOW + 7, "Item\\", GL_NEAREST, true); // Bolts

	OpenTexture(MODEL_BOW + 8, "Item\\", GL_NEAREST, true); // Crossbow
	OpenTexture(MODEL_BOW + 9, "Item\\", GL_NEAREST, true); // Golden Crossbow
	OpenTexture(MODEL_BOW + 10, "Item\\", GL_NEAREST, true); // Arquebus
	OpenTexture(MODEL_BOW + 11, "Item\\", GL_NEAREST, true); // Light Crossbow
	OpenTexture(MODEL_BOW + 12, "Item\\", GL_NEAREST, true); // Serpent Crossbow
	OpenTexture(MODEL_BOW + 13, "Item\\", GL_NEAREST, true); // Bluewing Crossbow
	OpenTexture(MODEL_BOW + 14, "Item\\", GL_NEAREST, true); // Aquagold Crossbow

	OpenTexture(MODEL_BOW + 15, "Item\\", GL_NEAREST, true); // Arrows

	OpenTexture(MODEL_BOW + 16, "Item\\", GL_NEAREST, true); // Saint Crossbow
	OpenTexture(MODEL_BOW + 17, "Item\\", GL_NEAREST, true); // Celestial Bow
	OpenTexture(MODEL_BOW + 18, "Item\\", GL_NEAREST, true); // Divine Crossbow of Archangel

	/************************** Staffs **************************/
	OpenTexture(MODEL_STAFF + 0, "Item\\", GL_NEAREST, true); // Skull Staff
	OpenTexture(MODEL_STAFF + 1, "Item\\", GL_NEAREST, true); // Angelic Staff
	OpenTexture(MODEL_STAFF + 2, "Item\\", GL_NEAREST, true); // Serpent Staff
	OpenTexture(MODEL_STAFF + 3, "Item\\", GL_NEAREST, true); // Thunder Staff
	OpenTexture(MODEL_STAFF + 4, "Item\\", GL_NEAREST, true); // Gorgon Staff
	OpenTexture(MODEL_STAFF + 5, "Item\\", GL_NEAREST, true); // Legendary Staff
	OpenTexture(MODEL_STAFF + 6, "Item\\", GL_NEAREST, true); // Staff of Resurrection
	OpenTexture(MODEL_STAFF + 7, "Item\\", GL_NEAREST, true); // Chaos Lightning Staff
	OpenTexture(MODEL_STAFF + 8, "Item\\", GL_NEAREST, true); // Staff of Destruction
	OpenTexture(MODEL_STAFF + 9, "Item\\", GL_NEAREST, true); // Dragon Soul Staff
	OpenTexture(MODEL_STAFF + 10, "Item\\", GL_NEAREST, true); // Divine Staff of Archangel

	/************************** Shields **************************/
	OpenTexture(MODEL_SHIELD + 0, "Item\\", GL_NEAREST, true); // Small Shield
	OpenTexture(MODEL_SHIELD + 1, "Item\\", GL_NEAREST, true); // Horn Shield
	OpenTexture(MODEL_SHIELD + 2, "Item\\", GL_NEAREST, true); // Kite Shield
	OpenTexture(MODEL_SHIELD + 3, "Item\\", GL_NEAREST, true); // Elven Shield
	OpenTexture(MODEL_SHIELD + 4, "Item\\", GL_NEAREST, true); // Buckler
	OpenTexture(MODEL_SHIELD + 5, "Item\\", GL_NEAREST, true); // Dragon Slayer Shield
	OpenTexture(MODEL_SHIELD + 6, "Item\\", GL_NEAREST, true); // Skull Shield
	OpenTexture(MODEL_SHIELD + 7, "Item\\", GL_NEAREST, true); // Spiked Shield
	OpenTexture(MODEL_SHIELD + 8, "Item\\", GL_NEAREST, true); // Tower Shield
	OpenTexture(MODEL_SHIELD + 9, "Item\\", GL_NEAREST, true); // Plate Shield
	OpenTexture(MODEL_SHIELD + 10, "Item\\", GL_NEAREST, true); // Large Round Shield
	OpenTexture(MODEL_SHIELD + 11, "Item\\", GL_NEAREST, true); // Serpent Shield
	OpenTexture(MODEL_SHIELD + 12, "Item\\", GL_NEAREST, true); // Bronze Shield
	OpenTexture(MODEL_SHIELD + 13, "Item\\", GL_NEAREST, true); // Dragon Shield
	OpenTexture(MODEL_SHIELD + 14, "Item\\", GL_NEAREST, true); // Legendary Shield
	OpenTexture(MODEL_SHIELD + 15, "Item\\", GL_NEAREST, true); // Grand Soul Shield
	OpenTexture(MODEL_SHIELD + 16, "Item\\", GL_NEAREST, true); // Elemental Shield

	/************************** Wings **************************/
	OpenTexture(MODEL_WING + 0, "Item\\", GL_NEAREST, true); // Wings of Elf
	OpenTexture(MODEL_WING + 1, "Item\\", GL_NEAREST, true); // Wings of Heaven
	OpenTexture(MODEL_WING + 2, "Item\\", GL_NEAREST, true); // Wings of Satan

	OpenTexture(MODEL_WING + 3, "Item\\", GL_NEAREST, true); // Wings of Spirits
	OpenTexture(MODEL_WING + 4, "Item\\", GL_NEAREST, true); // Wings of Soul
	OpenTexture(MODEL_WING + 5, "Item\\", GL_NEAREST, true); // Wings of Dragon
	OpenTexture(MODEL_WING + 6, "Item\\", GL_NEAREST, true); // Wings of Darkness

	/************************** Pets **************************/
	OpenTexture(MODEL_HELPER + 0, "Item\\", GL_NEAREST, true); // Guardian Angel
	OpenTexture(MODEL_HELPER + 1, "Item\\", GL_NEAREST, true); // Imp
	OpenTexture(MODEL_HELPER + 2, "Item\\", GL_NEAREST, true); // Horn of Uniria
	OpenTexture(MODEL_HELPER + 3, "Item\\", GL_NEAREST, true); // Horn of Dinorant

	/************************** Rings **************************/
	OpenTexture(MODEL_HELPER + 8, "Item\\", GL_NEAREST, true); // Ring of Ice
	OpenTexture(MODEL_HELPER + 9, "Item\\", GL_NEAREST, true); // Ring of Poison
	OpenTexture(MODEL_HELPER + 10, "Item\\", GL_NEAREST, true); // Transformation Ring
	OpenTexture(MODEL_HELPER + 12, "Item\\", GL_NEAREST, true); // Pendant of Lightning
	OpenTexture(MODEL_HELPER + 13, "Item\\", GL_NEAREST, true); // Pendant of Fire

	/************************** Consumables **************************/
	OpenTexture(MODEL_POTION + 0, "Item\\", GL_NEAREST, true); // Apple
	OpenTexture(MODEL_POTION + 1, "Item\\", GL_NEAREST, true); // Small Healing Potion
	OpenTexture(MODEL_POTION + 2, "Item\\", GL_NEAREST, true); // Medium Healing Potion
	OpenTexture(MODEL_POTION + 3, "Item\\", GL_NEAREST, true); // Large Healing Potion
	OpenTexture(MODEL_POTION + 4, "Item\\", GL_NEAREST, true); // Small Mana Potion
	OpenTexture(MODEL_POTION + 5, "Item\\", GL_NEAREST, true); // Medium Mana Potion
	OpenTexture(MODEL_POTION + 6, "Item\\", GL_NEAREST, true); // Large Mana Potion
	OpenTexture(MODEL_POTION + 8, "Item\\", GL_NEAREST, true); // Antidote
	OpenTexture(MODEL_POTION + 9, "Item\\", GL_NEAREST, true); // Ale

	OpenTexture(MODEL_POTION + 10, "Item\\", GL_NEAREST, true); // Town Portal Scroll

	OpenTexture(MODEL_POTION + 20, "Item\\", GL_NEAREST, true); // Remedy of Love

	/************************** Dropables **************************/
	OpenTexture(MODEL_POTION + 11, "Item\\", GL_NEAREST, true); // Box of Luck
	OpenTexture(MODEL_POTION + 12, "Item\\", GL_NEAREST, true); // Heart

	OpenTexture(MODEL_POTION + 15, "Item\\", GL_NEAREST, true); // Zen

	/************************** Jewels **************************/
	OpenTexture(MODEL_WING + 15, "Item\\", GL_NEAREST, true); // Jewel of Chaos
	OpenTexture(MODEL_POTION + 13, "Item\\", GL_NEAREST, true); // Jewel of Bless
	OpenTexture(MODEL_POTION + 14, "Item\\", GL_NEAREST, true); // Jewel of Soul
	OpenTexture(MODEL_POTION + 16, "Item\\", GL_NEAREST, true); // Jewel of Life
	OpenTexture(MODEL_POTION + 22, "Item\\", GL_NEAREST, true); // Jewel of Creation

	/************************** Events **************************/
	OpenTexture(MODEL_HELPER + 16, "Item\\", GL_NEAREST, true); // Scroll of Archangel
	OpenTexture(MODEL_HELPER + 17, "Item\\", GL_NEAREST, true); // Blood Bone
	OpenTexture(MODEL_HELPER + 18, "Item\\", GL_NEAREST, true); // Cloak of Invisibility

	OpenTexture(MODEL_POTION + 17, "Item\\", GL_NEAREST, true); // Devil's Eye
	OpenTexture(MODEL_POTION + 18, "Item\\", GL_NEAREST, true); // Devil's Key
	OpenTexture(MODEL_POTION + 19, "Item\\", GL_NEAREST, true); // Devil's Invitation

	OpenTexture(MODEL_POTION + 21, "Item\\", GL_NEAREST, true); // Rena

	/************************** Events2 **************************/
	OpenTexture(MODEL_EVENT + 0, "Item\\", GL_NEAREST, true); // Mu Coin
	OpenTexture(MODEL_EVENT + 1, "Item\\", GL_NEAREST, true); // Mu Scroll
	OpenTexture(MODEL_EVENT + 4, "Item\\", GL_NEAREST, true); // Christmas Star
	OpenTexture(MODEL_EVENT + 5, "Item\\", GL_NEAREST, true); // Magic pouch
	OpenTexture(MODEL_EVENT + 6, "Item\\", GL_NEAREST, true); // Love Heart
	OpenTexture(MODEL_EVENT + 7, "Item\\", GL_NEAREST, true); // Olive of Love
	OpenTexture(MODEL_EVENT + 8, "Item\\", GL_NEAREST, true); // Silver medal
	OpenTexture(MODEL_EVENT + 9, "Item\\", GL_NEAREST, true); // Gold Medal
	OpenTexture(MODEL_EVENT + 10, "Item\\", GL_NEAREST, true); // Eldorado
	OpenTexture(MODEL_EVENT + 11, "Item\\", GL_NEAREST, true); // Stone

	/************************** Quest **************************/
	OpenTexture(MODEL_POTION + 23, "Item\\", GL_NEAREST, true); // Scroll of Emperor
	OpenTexture(MODEL_POTION + 24, "Item\\", GL_NEAREST, true); // Broken Sword
	OpenTexture(MODEL_POTION + 25, "Item\\", GL_NEAREST, true); // Tear of Elf
	OpenTexture(MODEL_POTION + 26, "Item\\", GL_NEAREST, true); // Soul Shard of Wizard

	OpenTexture(MODEL_HELPER + 14, "Item\\", GL_NEAREST, true); // Loch's Feather
	OpenTexture(MODEL_HELPER + 15, "Item\\", GL_NEAREST, true); // Fruit

	/************************** Orbs **************************/
	OpenTexture(MODEL_WING + 7, "Item\\", GL_NEAREST, true); // Orb of Twisting Slash
	OpenTexture(MODEL_WING + 8, "Item\\", GL_NEAREST, true); // Healing Orb
	OpenTexture(MODEL_WING + 9, "Item\\", GL_NEAREST, true); // Orb of Greater Defense
	OpenTexture(MODEL_WING + 10, "Item\\", GL_NEAREST, true); // Orb of Greater Damage
	OpenTexture(MODEL_WING + 11, "Item\\", GL_NEAREST, true); // Orb of Summoning
	OpenTexture(MODEL_WING + 12, "Item\\", GL_NEAREST, true); // Orb of Rageful Blow
	OpenTexture(MODEL_WING + 13, "Item\\", GL_NEAREST, true); // Orb of Impale
	OpenTexture(MODEL_WING + 14, "Item\\", GL_NEAREST, true); // Orb of Greater Fortitude
	OpenTexture(MODEL_WING + 16, "Item\\", GL_NEAREST, true); // Orb of Fire Slash
	OpenTexture(MODEL_WING + 17, "Item\\", GL_NEAREST, true); // Orb of Penetration
	OpenTexture(MODEL_WING + 18, "Item\\", GL_NEAREST, true); // Orb of Ice Arrow
	OpenTexture(MODEL_WING + 19, "Item\\", GL_NEAREST, true); // Orb of Death Stab

	/************************** Books **************************/
	OpenTexture(MODEL_ETC + 0, "Item\\", GL_NEAREST, true); // Scroll of Poison
	OpenTexture(MODEL_ETC + 1, "Item\\", GL_NEAREST, true); // Scroll of Meteorite
	OpenTexture(MODEL_ETC + 2, "Item\\", GL_NEAREST, true); // Scroll of Lightning
	OpenTexture(MODEL_ETC + 3, "Item\\", GL_NEAREST, true); // Scroll of Fire Ball
	OpenTexture(MODEL_ETC + 4, "Item\\", GL_NEAREST, true); // Scroll of Flame
	OpenTexture(MODEL_ETC + 5, "Item\\", GL_NEAREST, true); // Scroll of Teleport
	OpenTexture(MODEL_ETC + 6, "Item\\", GL_NEAREST, true); // Scroll of Ice
	OpenTexture(MODEL_ETC + 7, "Item\\", GL_NEAREST, true); // Scroll of Twister
	OpenTexture(MODEL_ETC + 8, "Item\\", GL_NEAREST, true); // Scroll of Evil Spirit
	OpenTexture(MODEL_ETC + 9, "Item\\", GL_NEAREST, true); // Scroll of Hellfire
	OpenTexture(MODEL_ETC + 10, "Item\\", GL_NEAREST, true); // Scroll of Power Wave
	OpenTexture(MODEL_ETC + 11, "Item\\", GL_NEAREST, true); // Scroll of Aqua Beam
	OpenTexture(MODEL_ETC + 12, "Item\\", GL_NEAREST, true); // Scroll of Cometfall
	OpenTexture(MODEL_ETC + 13, "Item\\", GL_NEAREST, true); // Scroll of Inferno
	OpenTexture(MODEL_ETC + 14, "Item\\", GL_NEAREST, true); // Scroll of Teleport Ally
	OpenTexture(MODEL_ETC + 15, "Item\\", GL_NEAREST, true); // Scroll of Soul Barrier

	/************************** Customs **************************/

	char textureFolder[MAX_PATH];

	int n = 0;

	for (n = 0; n < MAX_ITEM; n++)
	{
		if (gCustomItem.m_CustomItemInfo[n].ItemIndex != -1)
		{
			if (gCustomItem.m_CustomItemInfo[n].ItemIndex >= GET_ITEM(7, 0) && gCustomItem.m_CustomItemInfo[n].ItemIndex < GET_ITEM(12, 0))
			{
				wsprintf(textureFolder, "Player\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex % MAX_ITEM_TYPE);
			}
			else
			{
				wsprintf(textureFolder, "Item\\Custom\\%d\\", gCustomItem.m_CustomItemInfo[n].ItemIndex);
			}

			gLoadModels.MyOpenTexture((gCustomItem.m_CustomItemInfo[n].ItemIndex + ITEM_BASE_MODEL), textureFolder);
		}
	}
}

void CLoadModels::PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon)
{
	if (!gCustomGlow.GetItemGlow((Type - ITEM_BASE_MODEL), Light))
	{
		PartObjectColor(Type, Alpha, Bright, Light, ExtraMon);
	}
}

BITMAP_t* CLoadModels::GetBitmap(int uiTextureNumber)
{
	return &this->lpTextures[uiTextureNumber];
}