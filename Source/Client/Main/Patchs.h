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
};

extern CPatchs gPatchs;