#pragma once

class COptionsMenu
{
public:

	COptionsMenu();

	virtual ~COptionsMenu();

	void Init();

private:

	static void OptionsWindowUI();

	static void OptionsWindowControl();

	void RenderOptionsMenu();

	void RenderOptionTitle();

	void RenderAutomaticAttack();

	void RenderSoundOnWhisper();

	void RenderVolume();

	void RenderClose();

	bool CheckOptionTitle();

	bool CheckAutomaticAttack();

	bool CheckSoundOnWhisper();

	bool CheckVolume();

	bool CheckClose();

private:

	float GeneralPosX;

	float GeneralPosY;

	float GeneralWidth;

	float GeneralHeight;
};

extern COptionsMenu gOptionsMenu;