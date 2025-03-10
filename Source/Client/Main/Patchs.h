#pragma once

class CPatchs
{
public:

	CPatchs();

	virtual ~CPatchs();

	void Init();

private:

	static DWORD ReduceCPU();

	static void ReduceRam(LPVOID lpThreadParameter);

	static void FixShopNpcClose();

	static void IgnoreRandomStuck(DWORD c, DWORD Damage);

	static void FixChasingAttackMovement();

	static void FixWeaponGlow();

	static void DecBMD();

	static int OpenTerrainMapping(char* FileName);

	static int OpenTerrainAttribute(char* FileName);

	static int OpenObjectsEnc(char* FileName);

	static BOOL ReadMainVersion();

	static void MySaveScreen();

	static void MyBeginBitmap();

	static bool RenderNumArrow();

	static int RenderEquipedHelperLife(bool RenderedArrow);

	static void RenderBrokenItem(int PosY);

	static bool CheckSpecialText(char* Text);

	static void SetCharacterDeleteMaxLevel();

	static void SendContinueDeathStab();

	static void SendContinueBlast();

	static void SendContinueTripleShot();

	static void SendContinuePenetration();

	static void SendContinueTwistingSlash();

	static void SendContinueFireSlash();

	static void SendContinuePowerSlash();

	static void SendContinueRagefulBlow();

	static void SendContinueHellFire();

	static void SendContinueInferno();

	static void SendContinueTwisterAndEvilSpirit();

	static void SendContinueAquaBeam();

	static void SendContinueFlame();

	static void RenderWheelWeapon(DWORD o);

	static void SendElfBuff();

	void SendRequestMagicContinue(int skill, int x, int y, int dir, int dis, int angle, int target);

	void SendRequestMagic(int skill, int target);

	static bool AttackStage(DWORD c, DWORD o);

	static void FixMoveWhileAttacking();

	static void CalcFPS();

	static void FixPigeons();
};

extern CPatchs gPatchs;