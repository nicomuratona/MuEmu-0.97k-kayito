#pragma once

struct ITEM /* Struct Size = 68 */
{
	/*+00*/ short Type;
	/*+04*/ int Level;
	/*+08*/ BYTE Part;
	/*+09*/ BYTE Class;
	/*+10*/ BYTE TwoHand;
	/*+12*/ WORD DamageMin;
	/*+14*/ WORD DamageMax;
	/*+16*/ BYTE SuccessfulBlocking;
	/*+18*/ WORD Defense;
	/*+20*/ WORD MagicDefense;
	/*+22*/ BYTE WeaponSpeed;
	/*+24*/ WORD WalkSpeed;
	/*+26*/ BYTE Durability;
	/*+27*/ BYTE Option1;
	/*+28*/ WORD RequireStrength;
	/*+30*/ WORD RequireDexterity;
	/*+32*/ WORD RequireEnergy;
	/*+34*/ WORD RequireLevel;
	/*+36*/ BYTE SpecialNum;
	/*+37*/ BYTE Special[MAX_SPECIAL_OPTION];
	/*+45*/ BYTE SpecialValue[MAX_SPECIAL_OPTION];
	/*+56*/ DWORD Key;
	/*+60*/ BYTE Unkown;
	/*+61*/ BYTE byColorState;
	/*+62*/ BYTE x;
	/*+63*/ BYTE y;
	/*+64*/ BYTE Color;
};

struct ITEM_ATTRIBUTE /* Struct Size = 64 */
{
	/*+00*/ char Name[30];
	/*+30*/ bool TwoHand;
	/*+31*/ BYTE Level;
	/*+32*/ BYTE Width;
	/*+33*/ BYTE Height;
	/*+34*/ BYTE DamageMin;
	/*+35*/ BYTE DamageMax;
	/*+36*/ BYTE DefenseRate;
	/*+37*/ BYTE Defense;
	/*+38*/ BYTE MagicDefense;
	/*+39*/ BYTE AttackSpeed;
	/*+40*/ BYTE WalkSpeed;
	/*+41*/ BYTE Durability;
	/*+42*/ BYTE MagicDurability;
	/*+43*/ BYTE MagicPower;
	/*+44*/ WORD RequireStrength;
	/*+46*/ WORD RequireAgility;
	/*+48*/ BYTE RequireEnergy;
	/*+49*/ BYTE RequireLevel;
	/*+50*/ BYTE Value;
	/*+51*/ //BYTEFILL
	/*+52*/ int Money;
	/*+56*/ BYTE RequireClass[MAX_CLASS];
	/*+57*/ //BYTE RequireDK;
	/*+58*/ //BYTE RequireELF;
	/*+59*/ //BYTE RequireMG;
	/*+60*/ BYTE Resistance[MAX_RESISTANCE];
	/*+61*/ //BYTE ResistancePoison;
	/*+62*/ //BYTE ResistanceLightning;
	/*+63*/ //BYTE ResistanceFire;
};

struct GATE_ATTRIBUTE
{
	BYTE Flag;
	BYTE Map;
	BYTE StartX;
	BYTE StartY;
	BYTE EndX;
	BYTE EndY;
	BYTE Target;
	BYTE Direction;
	BYTE RequireLevel;
};

struct PARTY_t
{
	char Name[11];
	BYTE number;
	BYTE map;
	BYTE x;
	BYTE y;
	DWORD CurLife;
	DWORD MaxLife;
	BYTE stepHP;
	int index;
	DWORD state;
};

struct BITMAP_t
{
	char FileName[MAX_BITMAP_FILE_NAME];
	float Width;
	float Height;
	char Components;
	GLuint TextureNumber;
	BYTE Ref;
	BYTE* Buffer;
};

struct MONSTER_ATTRIBUTE
{
	WORD Life;
	WORD MoveSpeed;
	WORD AttackSpeed;
	WORD AttackDamageMin;
	WORD AttackDamageMax;
	WORD Defense;
	WORD MagicDefense;
	WORD AttackRating;
	WORD SuccessfulBlocking;
};

struct MONSTER_SCRIPT
{
	BYTE Type;
	char Name[32];
	WORD Level;
	MONSTER_ATTRIBUTE Attribute;
};

struct BMD
{
	BYTE Data[188];
};

struct SERVER_t
{
	char IP[20];
	WORD Port;
	WORD Index;
	BYTE Percent;
};

struct SERVER_LIST_t
{
	char Name[20];
	BYTE Number;
	bool extServer;
	SERVER_t Server[MAX_SERVER_LO];
};