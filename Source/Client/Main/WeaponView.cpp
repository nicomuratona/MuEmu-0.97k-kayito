#include "stdafx.h"
#include "WeaponView.h"

CWeaponView gWeaponView;

CWeaponView::CWeaponView()
{
	this->SecondWeaponFixVal = 1;
}

CWeaponView::~CWeaponView()
{

}

void CWeaponView::Init()
{
	SetCompleteHook(0xE9, 0x004582C9, &this->CheckRenderBackItem);

	SetCompleteHook(0xE9, 0x0045568B, &this->SecondWeaponViewFix);
}

_declspec(naked) void CWeaponView::CheckRenderBackItem()
{
	static DWORD jmpBack = 0x0045868B;
	static DWORD bind = 0;

	_asm
	{
		Pushad;
		Push Ebp;
		Push Edi;
		Lea Ecx, gWeaponView;
		Call[CWeaponView::RenderCharacterBackItem];
		Mov bind, Eax;
		Popad;
		Mov Ecx, bind;
		Mov[Esp + 0x18], Ecx;
		Jmp[jmpBack];
	}
}

BOOL CWeaponView::RenderCharacterBackItem(DWORD c, DWORD o)
{
	// Is there an item stuck to your back?
	BOOL bBindBack = FALSE;

	// General conditions for items to be attached to the back
	// Safety zone, certain actions such as greeting, if you are in an Atlas or Kalima map and you are swimming, an item will stick to your back.
	if (*(BYTE*)(c + 0x34E)) // c->SafeZone
	{
		bBindBack = TRUE;
	}

	if (*(BYTE*)(o + 0x105) >= 93u && *(BYTE*)(o + 0x105) <= 124u) // o->CurrentAction >= PLAYER_GREETING1 && o->CurrentAction <= PLAYER_SALUTE1
	{
		bBindBack = TRUE;
	}

	if (World == MAP_ATLANS && (*(BYTE*)(o + 0x105) == 21 || *(BYTE*)(o + 0x105) == 29)) // World == WD_7ATLANSE && (o->CurrentAction == PLAYER_WALK_SWIM || o->CurrentAction == PLAYER_RUN_SWIM)
	{
		bBindBack = TRUE;
	}

	// Conditions for items not being attached to the back in a specific map
	// Chaos Castle, Blood Castle
	if (World >= MAP_BLOOD_CASTLE1 && World <= MAP_BLOOD_CASTLE6) // InBloodCastle() == true
	{
		bBindBack = FALSE;
	}

	// If the player
	if (*(WORD*)(o + 2) == 390) // o->Type == MODEL_PLAYER
	{
		bool bBack = false; // Does it stick to the back?

		// Only two hands are tested
		for (int i = 0; i < 2; ++i)
		{
			int iType = *(short*)(c + (24 * i) + 0x270);

			int iLevel = *(BYTE*)(c + (24 * i) + 0x272);

			int iOption1 = *(BYTE*)(c + (24 * i) + 0x273);

			if (iType < 0)
			{
				continue;
			}

			// Skeleton NPC in Laurencia
			if (*(BYTE*)(o + 0x84) == 4 && World == MAP_LORENCIA && (*(DWORD*)(o + 4) >= 206 && *(DWORD*)(o + 4) <= 208)) // o->Kind == KIND_NPC && World == WD_0LORENCIA && (o->SubType >= MODEL_SKELETON1 && o->SubType <= MODEL_SKELETON3)
			{
				// To be rendered only once
				if (i == 0)
				{
					bBack = true;

					iType = GET_ITEM_MODEL(4, 9);

					iLevel = 8;
				}
			}

			// Is this an item type that attaches to the back?
			if (IsBackItem(iType))
			{
				bBack = true;
			}

			// If it is an arrow, it will stick.
			if (iType == GET_ITEM_MODEL(4, 7) || iType == GET_ITEM_MODEL(4, 15))
			{
				bBack = true;
			}
			else
			{
				// If it is not an arrow and does not attach to the back, it will not attach to the back.
				if (bBindBack == FALSE)
				{
					bBack = false;
				}
			}

			// If it is attached to the back and there is an item
			if (bBack && iType != -1)
			{
				float fAnimationFrameBackUp = *(float*)(c + 0x2A8); // w->AnimationFrame;

				// link number
				*(BYTE*)(c + 0x2A4) = 47; // w->LinkBone = 47;

				// set animation speed
				if (*(BYTE*)(o + 0x105) == 30 || *(BYTE*)(o + 0x105) == 31) // o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW
				{
					*(float*)(c + 0x2B0) = 1.0f; // w->PlaySpeed = 1.0f;
				}
				else
				{
					*(float*)(c + 0x2B0) = 0.25f; // w->PlaySpeed = 0.25f;
				}

				if (i == 0) // If it is a right-handed weapon
				{
					this->SecondWeaponFixVal = 0;
				}
				else
				{
					this->SecondWeaponFixVal = 1;
				}

				RenderLinkObject(0.0f, 0.0f, 15.0f, c, (c + 0x2A0), iType, iLevel, iOption1, true, true, 0);

				*(float*)(c + 0x2A8) = fAnimationFrameBackUp;
			}
		}

		// The absolute weapon obtained from Blood Castle is displayed on the back.
		if ((World >= MAP_BLOOD_CASTLE1 && World <= MAP_BLOOD_CASTLE6) && *(BYTE*)(c + 0x2E8) != 0) // InBloodCastle() == true && c->EtcPart != 0
		{
			int iType = 0;

			int iLevel = 0;

			int iOption1 = 0;

			*(BYTE*)(c + 0x2A4) = 47; // w->LinkBone = 47;

			if (*(BYTE*)(o + 0x105) == 30 || *(BYTE*)(o + 0x105) == 31) // o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW
			{
				*(float*)(c + 0x2B0) = 1.0f; // w->PlaySpeed = 1.0f;
			}
			else
			{
				*(float*)(c + 0x2B0) = 0.25f; // w->PlaySpeed = 0.25f;
			}

			switch (*(BYTE*)(c + 0x2E8)) // c->EtcPart
			{
				case 1:
				{
					iType = GET_ITEM_MODEL(5, 10);

					break;
				}

				case 2:
				{
					iType = GET_ITEM_MODEL(0, 19);

					break;
				}

				case 3:
				{
					iType = GET_ITEM_MODEL(4, 18);

					break;
				}
			}

			RenderLinkObject(0.0f, 0.0f, 15.0f, c, (c + 0x2A0), iType, iLevel, iOption1, true, true, 0);
		}

		if (*(short*)(c + 0x2A0) != -1) // c->Wing->Type
		{
			*(BYTE*)(c + 0x2A4) = 47; // w->LinkBone = 47;

			if (*(BYTE*)(o + 0x105) == 30 || *(BYTE*)(o + 0x105) == 31) // o->CurrentAction == PLAYER_FLY || o->CurrentAction == PLAYER_FLY_CROSSBOW
			{
				*(float*)(c + 0x2B0) = 1.0f; // w->PlaySpeed = 1.0f;
			}
			else
			{
				*(float*)(c + 0x2B0) = 0.25f; // w->PlaySpeed = 0.25f;
			}

			RenderLinkObject(0.0f, 0.0f, 15.0f, c, (c + 672), *(short*)(c + 0x2A0), *(BYTE*)(c + 0x2A2), *(BYTE*)(c + 0x2A3), false, true, 0);
		}

		if (*(short*)(c + 0x2B8) == GET_ITEM_MODEL(13, 1)) // c->Helper.Type == MODEL_HELPER + 1
		{
			*(BYTE*)(c + 0x2BC) = 34; // w->LinkBone = 34;

			*(float*)(c + 0x2C8) = 0.5f; // w->PlaySpeed = 0.5f;

			RenderLinkObject(20.0f, 0.0f, 0.0f, c, (c + 696), GET_ITEM_MODEL(13, 1), *(BYTE*)(c + 0x2BA), *(BYTE*)(c + 0x2BB), false, true, 0);

			vec3_t vRelativePos, vPos, vLight;

			Vector(20.0f, 0.0f, 15.0f, vRelativePos);

			Vector(0.0f, 0.0f, 0.0f, vPos);

			TransformPosition
			(
				*(DWORD*)(Models + (0xBC * *(short*)(o + 2))),
				&*(float*)(o + 0x114) + (0x30 * *(BYTE*)(c + 0x2BC)),
				vRelativePos,
				vPos,
				true
			);

			// sprite
			float fLuminosity = (float)(rand() % 30 + 70) * 0.01f;

			Vector(fLuminosity * 0.5f, fLuminosity * 0.0f, fLuminosity * 0.0f, vLight);

			CreateSprite(1150, vPos, 1.5f, vLight, o, 0.0f, 0);
		}
	}

	return bBindBack;
}

bool CWeaponView::IsBackItem(int iType)
{
	if (iType >= GET_ITEM_MODEL(0, 0) && iType < GET_ITEM_MODEL(1, 0)) // Swords
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(1, 0) && iType < GET_ITEM_MODEL(2, 0)) // Axes
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(2, 0) && iType < GET_ITEM_MODEL(3, 0)) // Maces
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(3, 0) && iType < GET_ITEM_MODEL(4, 0)) // Spears
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(4, 0) && iType < GET_ITEM_MODEL(5, 0)) // Bows, Crossbows, Arrows & Bolts
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(5, 0) && iType < GET_ITEM_MODEL(6, 0)) // Staffs
	{
		return true;
	}

	if (iType >= GET_ITEM_MODEL(6, 0) && iType < GET_ITEM_MODEL(7, 0)) // Shields
	{
		return true;
	}

	return false;
}

_declspec(naked) void CWeaponView::SecondWeaponViewFix()
{
	static DWORD jmpBack = 0x004556AA;
	static DWORD AngleMatrix = 0x004F9DB0;

	_asm
	{
		Pushad;
		Cmp Ebx, GET_ITEM_MODEL(6, 0);
		Jl RotateWeaponCheck;
		Cmp Ebx, GET_ITEM_MODEL(7, 0);
		Jge RotateWeaponCheck;
		Cmp Ebx, GET_ITEM_MODEL(6, 13); // Dragon Shield
		Jnz NextShield1;
		Mov Dword Ptr Ss : [Esp + 0x28 + 0x20] , 0x41F00000; // Angle[0]
		Mov Dword Ptr Ss : [Esp + 0x2C + 0x20] , 0xC1200000; // Angle[1]
		Mov Dword Ptr Ss : [Esp + 0x30 + 0x20] , 0x43070000; // Angle[2]
		Lea Edx, Dword Ptr Ss : [Esp + 0x58 + 0x20] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28 + 0x20] ;
		Push Edx;
		Push Eax;
		Call[AngleMatrix];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0xC1200000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0x41200000; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0; // matrix[2][3]
		Jmp End;
	NextShield1:
		Cmp Ebx, GET_ITEM_MODEL(6, 16); // Elemental Shield
		Jnz NextShield2;
		Mov Dword Ptr Ss : [Esp + 0x28 + 0x20] , 0x41F00000; // Angle[0]
		Mov Dword Ptr Ss : [Esp + 0x2C + 0x20] , 0; // Angle[1]
		Mov Dword Ptr Ss : [Esp + 0x30 + 0x20] , 0x42B40000; // Angle[2]
		Lea Edx, Dword Ptr Ss : [Esp + 0x58 + 0x20] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28 + 0x20] ;
		Push Edx;
		Push Eax;
		Call[AngleMatrix];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0xC1A00000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0xC1A00000; // matrix[2][3]
		Jmp End;
	NextShield2:
		Cmp Ebx, GET_ITEM_MODEL(6, 15); // Grand Soul Shield
		Je NextShield3;
		Cmp Ebx, GET_ITEM_MODEL(6, 14); // Legendary Shield
		Je NextShield3;
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0xC1200000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0x41200000; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0; // matrix[2][3]
		Jmp End;
	NextShield3:
		Mov Dword Ptr Ss : [Esp + 0x28 + 0x20] , 0x42480000; // Angle[0]
		Mov Dword Ptr Ss : [Esp + 0x2C + 0x20] , 0; // Angle[1]
		Mov Dword Ptr Ss : [Esp + 0x30 + 0x20] , 0x42B40000; // Angle[2]
		Lea Edx, Dword Ptr Ss : [Esp + 0x58 + 0x20] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28 + 0x20] ;
		Push Edx;
		Push Eax;
		Call[AngleMatrix];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0xC1E00000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0x41200000; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0xC1C80000; // matrix[2][3]
		Jmp End;
	RotateWeaponCheck:
		Cmp gWeaponView.SecondWeaponFixVal, 0;
		Je RotateDefault;
		Mov Dword Ptr Ss : [Esp + 0x28 + 0x20] , 0xC2DC0000; // Angle[0]
		Mov Dword Ptr Ss : [Esp + 0x2C + 0x20] , 0x43340000; // Angle[1]
		Mov Dword Ptr Ss : [Esp + 0x30 + 0x20] , 0x42B40000; // Angle[2]
		Lea Edx, Dword Ptr Ss : [Esp + 0x58 + 0x20] ;
		Lea Eax, Dword Ptr Ss : [Esp + 0x28 + 0x20] ;
		Push Edx;
		Push Eax;
		Call[AngleMatrix];
		Add Esp, 0x8;
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0x41A00000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0x41700000; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0x42200000; // matrix[2][3]
		Jmp End;
	RotateDefault:
		Mov Dword Ptr Ss : [Esp + 0x64 + 0x20] , 0xC1A00000; // matrix[0][3]
		Mov Dword Ptr Ss : [Esp + 0x74 + 0x20] , 0x40A00000; // matrix[1][3]
		Mov Dword Ptr Ss : [Esp + 0x84 + 0x20] , 0x42200000; // matrix[2][3]
	End:
		Popad;
		Jmp[jmpBack];
	}
}