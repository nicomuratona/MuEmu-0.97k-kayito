#pragma once

class CPatchs
{
public:

	CPatchs();

	virtual ~CPatchs();

	void Init();

private:

	static void ReduceCPU();

	static void ReduceRam(LPVOID lpThreadParameter);

	static void FixShopNpcClose();

	static void IgnoreRandomStuck(DWORD c, DWORD Damage);

	static void FixChasingAttackMovement();

	static void DecBMD();

	static int OpenTerrainMapping(char* FileName);

	static int OpenTerrainAttribute(char* FileName);

	static int OpenObjectsEnc(char* FileName);

	static BOOL ReadMainVersion();

	static bool RenderNumArrow();

	static int RenderEquipedHelperLife(bool RenderedArrow);

	static void RenderBrokenItem(int PosY);

	static bool CheckSpecialText(char* Text);

	static void FixBloodCastleBackWeapon(float x, float y, float z, DWORD c, DWORD f, int Type, int Level, int Option1, bool Link, bool Translate, int RenderType);
};

extern CPatchs gPatchs;