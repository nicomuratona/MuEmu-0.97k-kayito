#pragma once

#define MAX_CUSTOM_BOW 32

struct CUSTOM_BOW_INFO
{
	int ItemIndex;
	int ItemType;
	int SkillShot;
};

enum eBowType
{
	TYPE_BOW = 0,
	TYPE_CROSSBOW = 1
};

class CCustomBow
{
public:

	CCustomBow();

	virtual ~CCustomBow();

	void Load(CUSTOM_BOW_INFO* info);

	CUSTOM_BOW_INFO* GetInfoByItem(int ItemIndex);

	int GetBowHandByItem(int ItemIndex);

	bool IsCustomBow(int ItemIndex);

	bool IsCustomCrossbow(int ItemIndex);

	int GetSkillEffect(int ItemIndex);

private:

	void SetInfo(CUSTOM_BOW_INFO info);

	void Init();

	// Funciones

	static void SetPlayerStop_Wings_Crossbow();
	static void SetPlayerStop_NoWings_Bow();
	static void SetPlayerStop_NoWings_Crossbow();

	static void SetPlayerWalk_Wings_Crossbow();
	static void SetPlayerWalk_Walk_Bow();
	static void SetPlayerWalk_Walk_Crossbow();
	static void SetPlayerWalk_Run_Bow();
	static void SetPlayerWalk_Run_Crossbow();

	static void SetPlayerBow_Bow();
	static void SetPlayerBow_Crossbow();

	static void SetPlayetAttack_Bow();
	static void SetPlayetAttack_Crossbow();
	static void SetPlayetAttack_Pet_Bow();
	static void SetPlayetAttack_Pet_Crossbow();

	static void SetPlayetAttack_Sound_Bow();
	static void SetPlayetAttack_Sound_Crossbow();

	static void CharacterStatsBow();
	static void CharacterStatsCrossbow();

	static void AssignArrowReloadBow();
	static void AssignBoltReloadCrossbow();

	static void SendReloadArrows();
	static void SendReloadBolts();

	static void NoMoreArrowsMessage();
	static void NoMoreBoltsMessage();

	static void SetBowAttackRange();
	static void SetCrossbowAttackRange();

	static void UpdateAttackPositionBow();
	static void UpdateAttackPositionCrossbow();

	static void AttackMovePositionBow();
	static void AttackMovePositionCrossbow();

	static void RenderPositionBow();
	static void RenderPositionCrossbow();

	static void CheckHandForBow();
	static void CheckHandForCrossbow();

	static void RenderBackPositionBow();
	static void RenderBackPositionCrossbow();

	static void SkillPenetrationBow();
	static void SkillPenetrationCrossbow();

	static void SkillIceArrowBow();
	static void SkillIceArrowCrossbow();

	static void CreateArrow(DWORD c, DWORD o, DWORD to, BYTE SkillIndex, BYTE Skill, BYTE SKKey);

public:

	CUSTOM_BOW_INFO m_CustomBowInfo[MAX_CUSTOM_BOW];
};

extern CCustomBow gCustomBow;