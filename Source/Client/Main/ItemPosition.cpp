#include "stdafx.h"
#include "ItemPosition.h"
#include "LoadModels.h"

CItemPosition gItemPosition;

CItemPosition::CItemPosition()
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->m_ItemPositionInfo[n].ItemIndex = -1;
	}

	this->Init();
}

CItemPosition::~CItemPosition()
{

}

void CItemPosition::Load(ITEM_POSITION_INFO* info)
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CItemPosition::SetInfo(ITEM_POSITION_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_ItemPositionInfo[info.ItemIndex] = info;
}

ITEM_POSITION_INFO* CItemPosition::GetInfoByIndex(int ItemIndex)
{
	if (ItemIndex < 0 || ItemIndex >= MAX_ITEM)
	{
		return NULL;
	}

	if (this->m_ItemPositionInfo[ItemIndex].ItemIndex != ItemIndex)
	{
		return NULL;
	}

	return &this->m_ItemPositionInfo[ItemIndex];
}

void CItemPosition::Init()
{
	SetCompleteHook(0xE9, 0x004E1BE0, &this->MyRenderItem3D);
}

void CItemPosition::MyRenderItem3D(float sx, float sy, float Width, float Height, int Type, int Level, int Option1, bool PickUp)
{
	bool Success = false;

	if ((!*(DWORD*)0x7E91388 || PickUp) && IsWorkZone((int)sx, (int)sy, (int)Width, (int)Height))
	{
		Success = true;
	}

	int RealLevel = GET_ITEM_OPT_LEVEL(Level);

	if (Type >= GET_ITEM(0, 0) && Type < GET_ITEM(1, 0)) // Swords
	{
		sx += Width * 0.8f;

		sy += Height * 0.85f;
	}
	else if (Type >= GET_ITEM(1, 0) && Type < GET_ITEM(2, 0)) // Axes
	{
		sx += Width * 0.8f;

		sy += Height * 0.7f;
	}
	else if (Type >= GET_ITEM(3, 0) && Type < GET_ITEM(4, 0)) // Spears
	{
		sx += Width * 0.6f;

		sy += Height * 0.65f;
	}
	else if (Type == GET_ITEM(4, 17)) // Celestial Bow
	{
		sx += Width * 0.5f;

		sy += Height * 0.5f;
	}
	else if (Type >= GET_ITEM(4, 8) && Type < GET_ITEM(5, 0)) // Crossbows & Arrows
	{
		sx += Width * 0.7f;

		sy += Height * 0.7f;
	}
	else if (Type >= GET_ITEM(5, 0) && Type < GET_ITEM(6, 0)) // Staffs
	{
		sx += Width * 0.6f;

		sy += Height * 0.55f;
	}
	else if (Type >= GET_ITEM(6, 0) && Type < GET_ITEM(7, 0)) // Shields
	{
		sx += Width * 0.5f;

		if (Type == GET_ITEM(6, 15)) // Grand Soul Shield
		{
			sy += Height * 0.7f;
		}
		else if (Type == GET_ITEM(6, 16)) // Elemental Shield
		{
			sy += Height * 0.9f;
		}
		else
		{
			sy += Height * 0.6f;
		}
	}
	else if (Type >= GET_ITEM(7, 0) && Type < GET_ITEM(8, 0)) // Helms
	{
		sx += Width * 0.5f;

		sy += Height * 0.8f;
	}
	else if (Type >= GET_ITEM(8, 0) && Type < GET_ITEM(9, 0)) // Armors
	{
		sx += Width * 0.5f;

		if (Type == GET_ITEM(8, 2) || Type == GET_ITEM(8, 4) || Type == GET_ITEM(8, 6)) // Pad, Bone, Scale Armors
		{
			sy += Height * 1.05f;
		}
		else if (Type == GET_ITEM(8, 3) || Type == GET_ITEM(8, 8)) // Legendary, Brass Armors
		{
			sy += Height * 1.1f;
		}
		else if (Type == GET_ITEM(8, 17) || Type == GET_ITEM(8, 18) || Type == GET_ITEM(8, 20)) // Dark Phoenix, Grand Soul, Thunder Hawk Armors
		{
			sy += Height * 0.8f;
		}
		else if (Type == GET_ITEM(8, 15)) // Storm Crow Armor
		{
			sy += Height * 1.0f;
		}
		else
		{
			sy += Height * 0.8f;
		}
	}
	else if (Type >= GET_ITEM(9, 0) && Type < GET_ITEM(12, 0)) // Pants, Gloves and Boots
	{
		sx += Width * 0.5f;

		sy += Height * 0.9f;
	}
	else if (Type == GET_ITEM(12, 3)) // Wings of Spirits
	{
		sx += Width * 0.5f;

		sy += Height * 0.45f;
	}
	else if (Type == GET_ITEM(12, 4)) // Wings of Soul
	{
		sx += Width * 0.5f;

		sy += Height * 0.4f;
	}
	else if (Type == GET_ITEM(12, 5)) // Wings of Dragon
	{
		sx += Width * 0.5f;

		sy += Height * 0.75f;
	}
	else if (Type == GET_ITEM(12, 6)) // Wings of Darkness
	{
		sx += Width * 0.5f;

		sy += Height * 0.55f;
	}
	// Orb of Rageful Blow, Orb of Impale, Orb of Greater Fortitude
	// Orb of Fire Slash, Orb of Penetration, Orb of Ice Arrow, Orb of Death Stab
	else if ((Type >= GET_ITEM(12, 12) && Type <= GET_ITEM(12, 14)) || (Type >= GET_ITEM(12, 16) && Type <= GET_ITEM(12, 19)))
	{
		sx += Width * 0.5f;

		sy += Height * 0.75f;
	}
	else if (Type == GET_ITEM(13, 14) || Type == GET_ITEM(13, 15)) // Loch's Feather, Fruit
	{
		sx += Width * 0.6f;

		sy += Height * 1.0f;
	}
	else if (Type == GET_ITEM(13, 16) || Type == GET_ITEM(13, 17)) // Scroll of Archangel, Blood Bone
	{
		sx += Width * 0.5f;

		sy += Height * 0.9f;
	}
	else if (Type == GET_ITEM(13, 18)) // Cloak of Invisibility
	{
		sx += Width * 0.5f;

		sy += Height * 0.75f;
	}
	else if (Type == GET_ITEM(13, 19)) // Divine Weapon of Archangel
	{
		switch (RealLevel)
		{
			case 0: // Archangel's absolute staff.
			{
				sx += Width * 0.5f;

				sy += Height * 0.5f;

				break;
			}

			case 1: // Archangel's Absolute Sword.
			{
				sx += Width * 0.7f;

				sy += Height * 0.8f;

				break;
			}

			case 2: // Archangel's Absolute Crossbow.
			{
				sx += Width * 0.7f;

				sy += Height * 0.7f;

				break;
			}
		}
	}
	else if (Type >= GET_ITEM(13, 0) && Type < GET_ITEM(14, 0)) // Pets, Rings, Pendants
	{
		sx += Width * 0.5f;

		sy += Height * 0.7f;
	}
	else if (Type == GET_ITEM(14, 9) && RealLevel == 1) // Olive of Love
	{
		sx += Width * 0.5f;

		sy += Height * 0.8f;
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel == 3) // Heart of Love
	{
		sx += Width * 0.5f;

		sy += Height * 0.5f;
	}
	else if (Type == GET_ITEM(14, 12)) // Heart
	{
		sx += Width * 0.5f;

		sy += Height * 0.5f;
	}
	else if (Type == GET_ITEM(14, 17) || Type == GET_ITEM(14, 18) || Type == GET_ITEM(14, 19)) // Devil's Eye, Devil's Key, Devil's Invitation
	{
		sx += Width * 0.5f;

		sy += Height * 0.5f;
	}
	else if (Type == GET_ITEM(14, 21)) // Rena
	{
		switch (RealLevel)
		{
			case 0: // Rena
			{
				sx += Width * 0.5f;

				sy += Height * 0.5f;

				break;
			}

			case 1: // Stone
			{
				sx += Width * 0.4f;

				sy += Height * 0.8f;

				break;
			}
		}
	}
	else if (Type >= GET_ITEM(14, 22) && Type < GET_ITEM(14, 25)) // Jewel of Creation, Scroll of Emperor, Broken Sword
	{
		sx += Width * 0.5f;

		sy += Height * 0.95f;
	}
	else if (Type >= GET_ITEM(14, 25) && Type < GET_ITEM(14, 27)) // Tear of Elf, Shoul Shard of Wizard
	{
		sx += Width * 0.5f;

		sy += Height * 0.9f;
	}
	else if (Type >= GET_ITEM(14, 0) && Type < GET_ITEM(15, 0)) // Jewels, Consumables
	{
		sx += Width * 0.5f;

		sy += Height * 0.95f;
	}
	else
	{
		sx += Width * 0.5f;

		sy += Height * 0.6f;
	}

	vec3_t Position;

	CreateScreenVector((int)(sx), (int)(sy), Position);

	if (Type == GET_ITEM(14, 11) && RealLevel == 1) // Star of Christmas
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 4, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel == 2) // Firecrackers
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 5, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel == 3) // Love heart
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 6, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel == 5) // Silver medal
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 8, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel == 6) // Gold medal
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 9, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 11) && RealLevel >= 8 && RealLevel <= 12) // Eldorado
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 10, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 9) && RealLevel == 1) // Olive of Love
	{
		gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 7, Level, Option1, Position, Success, PickUp);
	}
	else if (Type == GET_ITEM(14, 21)) // Rena
	{
		switch (RealLevel)
		{
			case 0: // Rena
			{
				gItemPosition.MyRenderObjectScreen(MODEL_POTION + 21, Level, Option1, Position, Success, PickUp);

				break;
			}

			case 1: // Stone
			{
				gItemPosition.MyRenderObjectScreen(MODEL_EVENT + 11, Level, Option1, Position, Success, PickUp);

				break;
			}
		}
	}
	else if (Type == GET_ITEM(13, 19)) // Archangel's absolute weapon series
	{
		switch (RealLevel)
		{
			case 0: // Archangel's absolute staff
			{
				gItemPosition.MyRenderObjectScreen(MODEL_STAFF + 10, -1, Option1, Position, Success, PickUp);

				break;
			}

			case 1:// Archangel's Absolute Sword
			{
				gItemPosition.MyRenderObjectScreen(MODEL_SWORD + 19, -1, Option1, Position, Success, PickUp);

				break;
			}

			case 2: // Archangel's Absolute Crossbow
			{
				gItemPosition.MyRenderObjectScreen(MODEL_BOW + 18, -1, Option1, Position, Success, PickUp);

				break;
			}
		}
	}
	else
	{
		gItemPosition.MyRenderObjectScreen(Type + ITEM_BASE_MODEL, Level, Option1, Position, Success, PickUp);
	}
}

void CItemPosition::MyRenderObjectScreen(int Type, int ItemLevel, int Option1, float Target[3], int Select, bool PickUp)
{
	ITEM_POSITION_INFO* lpInfo = this->GetInfoByIndex(Type - ITEM_BASE_MODEL);

	if (lpInfo != NULL)
	{
		int RealLevel = GET_ITEM_OPT_LEVEL(ItemLevel);

		vec3_t Direction, Position;

		VectorSubtract(Target, MousePosition, Direction);

		if (PickUp)
		{
			VectorMA(MousePosition, 0.07f, Direction, Position);
		}
		else
		{
			VectorMA(MousePosition, 0.1f, Direction, Position);
		}

		// Position
		Position[0] += lpInfo->PositionX;
		Position[1] += lpInfo->PositionY;
		// ObjectSelect_Angle
		ObjectSelect_Angle[0] = lpInfo->RotationX;
		ObjectSelect_Angle[1] = lpInfo->RotationY;
		ObjectSelect_Angle[2] = lpInfo->RotationZ;

		if (Select == 1)
		{
			ObjectSelect_Angle[1] = WorldTime * 0.45f;
		}

		ObjectSelect_Type = Type;

		DWORD b = gLoadModels.GetModels() + (188 * ObjectSelect_Type);

		*(BYTE*)(b + 0xA0) = 0; // b->CurrentAction = 0;

		ObjectSelect_AnimationFrame = 0;

		ObjectSelect_PriorAnimationFrame = 0;

		ObjectSelect_PriorAction = 0;

		*(float*)(b + 0x84) = 0; // b->BodyHeight = 0.0f;

		float Scale = lpInfo->Scale;

		BMD_Animation(b, BoneTransform, ObjectSelect_AnimationFrame, ObjectSelect_PriorAnimationFrame, ObjectSelect_PriorAction, ObjectSelect_Angle, ObjectSelect_HeadAngle, false, false);

		DWORD o = CreateObject(Type, Position, ObjectSelect_Angle, Scale);

		*(short*)(o + 0x2) = Type; // o->Type = Type;

		ItemObjectAttribute(o);

		*(BYTE*)(o + 0xDC) = false; // o->LightEnable = false;

		*(float*)(o + 0xC) = Scale; // o->Scale = Scale;

		VectorCopy(Position, (float*)(o + 0x10)); // VectorCopy(Position, o->Position);

		vec3_t Light;

		Vector(1.0f, 1.0f, 1.0f, Light);

		float alpha = *(float*)(o + 0x168); // o->Alpha

		RenderPartObject(o, Type, NULL, Light, alpha, ItemLevel, Option1, true, true, true, 0, 2);
	}
	else
	{
		RenderObjectScreen(Type, ItemLevel, Option1, Target, Select, PickUp);
	}
}