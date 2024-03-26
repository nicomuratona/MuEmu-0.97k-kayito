#pragma once

#include "DefaultClassInfo.h"
#include "ProtocolDefines.h"
#include "User.h"

#define MAX_SKILL 57
#define MAX_SKILL_LIST 60

#define CHECK_SKILL_ATTACK_COUNT(x) (((++x)>=10)?false:true)
#define CHECK_SKILL_ATTACK_EXTENDED_COUNT(x) (((++x)>=20)?false:true)

enum eSkillNumber
{
	SKILL_NONE = 0,
	SKILL_POISON = 1,
	SKILL_METEORITE = 2,
	SKILL_LIGHTNING = 3,
	SKILL_FIRE_BALL = 4,
	SKILL_FLAME = 5,
	SKILL_TELEPORT = 6,
	SKILL_ICE = 7,
	SKILL_TWISTER = 8,
	SKILL_EVIL_SPIRIT = 9,
	SKILL_HELL_FIRE = 10,
	SKILL_POWER_WAVE = 11,
	SKILL_AQUA_BEAM = 12,
	SKILL_BLAST = 13,
	SKILL_INFERNO = 14,
	SKILL_TELEPORT_ALLY = 15,
	SKILL_MANA_SHIELD = 16,
	SKILL_ENERGY_BALL = 17,
	SKILL_DEFENSE = 18,
	SKILL_FALLING_SLASH = 19,
	SKILL_LUNGE = 20,
	SKILL_UPPERCUT = 21,
	SKILL_CYCLONE = 22,
	SKILL_SLASH = 23,
	SKILL_TRIPLE_SHOT = 24,
	SKILL_HEAL = 26,
	SKILL_GREATER_DEFENSE = 27,
	SKILL_GREATER_DAMAGE = 28,
	SKILL_SUMMON1 = 30,
	SKILL_SUMMON2 = 31,
	SKILL_SUMMON3 = 32,
	SKILL_SUMMON4 = 33,
	SKILL_SUMMON5 = 34,
	SKILL_SUMMON6 = 35,
	SKILL_SUMMON7 = 36,
	SKILL_TWISTING_SLASH = 41,
	SKILL_RAGEFUL_BLOW = 42,
	SKILL_DEATH_STAB = 43,
	SKILL_IMPALE = 47,
	SKILL_GREATER_LIFE = 48,
	SKILL_FIRE_BREATH = 49,
	SKILL_MONSTER_AREA_ATTACK = 50,
	SKILL_ICE_ARROW = 51,
	SKILL_PENETRATION = 52,
	SKILL_FIRE_SLASH = 55,
	SKILL_POWER_SLASH = 56,
};

//**********************************************//
//************ Client -> GameServer ************//
//**********************************************//

struct PMSG_MULTI_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:[PROTOCOL_CODE4]
	BYTE skill;
	BYTE x;
	BYTE y;
	BYTE serial;
	BYTE count;
};

struct PMSG_MULTI_SKILL_ATTACK
{
	BYTE index[2];
};

struct PMSG_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:19
	BYTE skill;
	BYTE index[2];
};

struct PMSG_SKILL_CANCEL_RECV
{
	PBMSG_HEAD header; // C3:1B
	BYTE skill;
	BYTE index[2];
};

struct PMSG_DURATION_SKILL_ATTACK_RECV
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE x;
	BYTE y;
	BYTE dir;
	BYTE dis;
	BYTE angle;
	BYTE index[2];
};

struct PMSG_SKILL_TELEPORT_ALLY_RECV
{
#pragma pack(1)
	PBMSG_HEAD header; // C3:B0
	WORD index;
	BYTE x;
	BYTE y;
#pragma pack()
};

//**********************************************//
//************ GameServer -> Client ************//
//**********************************************//

struct PMSG_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:19
	BYTE skill;
	BYTE index[2];
	BYTE target[2];
};

struct PMSG_SKILL_CANCEL_SEND
{
	PBMSG_HEAD header; // C1:1B
	BYTE skill;
	BYTE index[2];
};

struct PMSG_DURATION_SKILL_ATTACK_SEND
{
	PBMSG_HEAD header; // C3:1E
	BYTE skill;
	BYTE index[2];
	BYTE x;
	BYTE y;
	BYTE dir;
};

struct PMSG_SKILL_NOVA_SEND
{
	PBMSG_HEAD header; // C1:BA
	BYTE index[2];
	BYTE type;
	BYTE count;
};

struct PMSG_SKILL_LIST_SEND
{
	PSBMSG_HEAD header; // C1:F3:11
	BYTE count;
};

struct PMSG_SKILL_LIST
{
	BYTE slot;
	BYTE skill;
	BYTE level;
};

//**********************************************//
//**********************************************//
//**********************************************//

struct SKILL_INFO
{
	int Index;
	char Name[32];
	int Damage;
	int Mana;
	int BP;
	int Range;
	int Radio;
	int Delay;
	int Type;
	int Effect;
	int RequireLevel;
	int RequireEnergy;
	int RequireClass[MAX_CLASS];
};

class CSkillManager
{
public:

	CSkillManager();

	~CSkillManager();

	void Load(char* path);

	bool GetInfo(int index, SKILL_INFO* lpInfo);

	int GetSkillDamage(int index);

	int GetSkillMana(int index);

	int GetSkillBP(int index);

	int GetSkillType(int index);

	int GetSkillEffect(int index);

	int GetSkillAngle(int x, int y, int tx, int ty);

	int GetSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, BYTE angle, int x, int y, float sx, float sy);

	int GetSkillNumber(int index, int level);

	bool CheckSkillMana(LPOBJ lpObj, int index);

	bool CheckSkillBP(LPOBJ lpObj, int index);

	bool CheckSkillRange(int index, int x, int y, int tx, int ty);

	bool CheckSkillRadio(int index, int x, int y, int tx, int ty);

	bool CheckSkillFrustrum(int* SkillFrustrumX, int* SkillFrustrumY, int x, int y);

	bool CheckSkillDelay(LPOBJ lpObj, int index);

	bool CheckSkillTarget(LPOBJ lpObj, int aIndex, int bIndex, int type);

	bool CheckSkillRequireLevel(LPOBJ lpObj, int index);

	bool CheckSkillRequireEnergy(LPOBJ lpObj, int index);

	bool CheckSkillRequireClass(LPOBJ lpObj, int index);

	bool CheckSkillRequireWeapon(LPOBJ lpObj, int index);

	int AddSkillWeapon(LPOBJ lpObj, int index, int level);

	int DelSkillWeapon(LPOBJ lpObj, int index, int level);

	int AddSkill(LPOBJ lpObj, int index, int level);

	int DelSkill(LPOBJ lpObj, int index);

	CSkill* GetSkill(LPOBJ lpObj, int index);

	bool ConvertSkillByte(CSkill* lpSkill, BYTE* lpMsg);

	void SkillByteConvert(BYTE* lpMsg, CSkill* lpSkill);

	void UseAttackSkill(int aIndex, int bIndex, CSkill* lpSkill);

	void UseDurationSkillAttack(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE dir, BYTE angle);

	bool RunningSkill(int aIndex, int bIndex, CSkill* lpSkill, BYTE x, BYTE y, BYTE angle);

	bool BasicSkillAttack(int aIndex, int bIndex, CSkill* lpSkill);

	bool MultiSkillAttack(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillChangeUse(int aIndex);

	bool SkillEvilSpirit(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillManaShield(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillDefense(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillTripleShot(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle);

	bool SkillHeal(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillGreaterDefense(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillGreaterDamage(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillSummon(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillTwistingSlash(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillRagefulBlow(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillDeathStab(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillGreaterLife(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillMonsterAreaAttack(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillFireSlash(int aIndex, int bIndex, CSkill* lpSkill);

	bool SkillPowerSlash(int aIndex, int bIndex, CSkill* lpSkill, BYTE angle);

	void ApplyFireSlashEffect(LPOBJ lpObj, LPOBJ lpTarget, CSkill* lpSkill, int damage);

	void CGMultiSkillAttackRecv(PMSG_MULTI_SKILL_ATTACK_RECV* lpMsg, int aIndex, bool type);

	void CGSkillAttackRecv(PMSG_SKILL_ATTACK_RECV* lpMsg, int aIndex);

	void CGDurationSkillAttackRecv(PMSG_DURATION_SKILL_ATTACK_RECV* lpMsg, int aIndex);

	void CGSkillCancelRecv(PMSG_SKILL_CANCEL_RECV* lpMsg, int aIndex);

	void CGSkillTeleportAllyRecv(PMSG_SKILL_TELEPORT_ALLY_RECV* lpMsg, int aIndex);

	void GCSkillAttackSend(LPOBJ lpObj, int skill, int aIndex, BYTE type);

	void GCSkillCancelSend(LPOBJ lpObj, int skill);

	void GCDurationSkillAttackSend(LPOBJ lpObj, int skill, BYTE x, BYTE y, BYTE dir);

	void GCSkillAddSend(int aIndex, BYTE slot, int skill, BYTE level);

	void GCSkillDelSend(int aIndex, BYTE slot, int skill, BYTE level);

	void GCSkillListSend(LPOBJ lpObj);

private:

	std::map<int, SKILL_INFO> m_SkillInfo;
};

extern CSkillManager gSkillManager;