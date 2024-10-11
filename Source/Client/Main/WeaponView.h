#pragma once

class CWeaponView
{
public:

	CWeaponView();

	virtual ~CWeaponView();

	void Init();

private:

	static void CheckRenderBackItem();

	BOOL RenderCharacterBackItem(DWORD c, DWORD o);

	bool IsBackItem(int iType);

	static void SecondWeaponViewFix();

private:

	BYTE SecondWeaponFixVal;

public:

	int DeleteWings;
};

extern CWeaponView gWeaponView;