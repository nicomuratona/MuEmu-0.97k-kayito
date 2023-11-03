#pragma once

class CWeaponView
{
public:

	CWeaponView();

	virtual ~CWeaponView();

	void Init();

	static void WeaponViewCheck();

	static void SecondWeaponViewCheck();

	static void SecondWeaponViewFix();
};

extern CWeaponView gWeaponView;