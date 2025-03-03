#pragma once

class COptionsMenu
{

	enum eMenuOptions
	{
		OPTION_NONE = 0,
		OPTION_GENERAL = 1,
		OPTION_ANTILAG = 2,
		OPTION_WINDOW = 3,
		OPTION_FONT = 4,
		MAX_OPTIONS,
	};

public:

	COptionsMenu();

	virtual ~COptionsMenu();

	void Init();

private:

	static void CheckErrorMessage();

	static void OptionsWindowUI();

	static void OptionsWindowControl();

	void RenderOptionsMenu();
	bool CheckOptionsMenu();

	void RenderAllOptions();
	bool CheckAllOptions();

	void RenderGeneral();
	bool CheckGeneral();
	void RenderGeneralTitle(float PosX, float PosY);
	void RenderLanguage(float PosX, float PosY);
	bool CheckLanguage(int PosX, int PosY);
	void RenderPVPWithoutControl(float PosX, float PosY);
	bool CheckPVPWithoutControl(int PosX, int PosY);
	void RenderAutomaticAttack(float PosX, float PosY);
	bool CheckAutomaticAttack(int PosX, int PosY);
	void RenderSoundOnWhisper(float PosX, float PosY);
	bool CheckSoundOnWhisper(int PosX, int PosY);
	void RenderSoundVolume(float PosX, float PosY);
	bool CheckSoundVolume(int PosX, int PosY);
	void RenderMusicVolume(float PosX, float PosY);
	bool CheckMusicVolume(int PosX, int PosY);
	void RenderMusicControls(float PosX, float PosY);
	bool CheckMusicControls(int PosX, int PosY);

	void ApplyAntilagDefaults();
	void RenderAntilag();
	bool CheckAntilag();
	void RenderAntilagTitle(float PosX, float PosY);
	void RenderDeleteShadows(float PosX, float PosY);
	bool CheckDeleteShadows(int PosX, int PosY);
	void RenderDeleteObjects(float PosX, float PosY);
	bool CheckDeleteObjects(int PosX, int PosY);
	void RenderDeleteFloor(float PosX, float PosY);
	bool CheckDeleteFloor(int PosX, int PosY);
	void RenderDeleteSkills(float PosX, float PosY);
	bool CheckDeleteSkills(int PosX, int PosY);
	void RenderDeleteStaticEffects(float PosX, float PosY);
	bool CheckDeleteStaticEffects(int PosX, int PosY);
	void RenderDeleteDynamicEffects(float PosX, float PosY);
	bool CheckDeleteDynamicEffects(int PosX, int PosY);
	void RenderDeleteWings(float PosX, float PosY);
	bool CheckDeleteWings(int PosX, int PosY);
	void RenderDeleteHPBar(float PosX, float PosY);
	bool CheckDeleteHPBar(int PosX, int PosY);
	void RenderDeleteInterface(float PosX, float PosY);
	bool CheckDeleteInterface(int PosX, int PosY);

	void RenderScreen();
	bool CheckScreen();
	void RenderScreenTitle(float PosX, float PosY);
	void RenderWindowMode(float PosX, float PosY);
	bool CheckWindowMode(int PosX, int PosY);
	void RenderBorderless(float PosX, float PosY);
	bool CheckBorderless(int PosX, int PosY);
	void RenderResolutions(float PosX, float& PosY);
	bool CheckResolutions(int PosX, int& PosY);

	void RenderFont();
	bool CheckFont();
	void RenderFontTitle(float PosX, float PosY);
	void RenderFontStyle(float PosX, float& PosY);
	bool CheckFontStyle(int PosX, int PosY);
	void RenderFontBold(float PosX, float& PosY);
	bool CheckFontBold(int PosX, int& PosY);
	void RenderFontItalic(float PosX, float& PosY);
	bool CheckFontItalic(int PosX, int& PosY);
	void RenderFontButtons(float PosX, float& PosY);
	bool CheckFontButtons(int PosX, int& PosY);
	void RenderResetFonts(float PosX, float& PosY);
	bool CheckResetFonts(int PosX, int& PosY);

	void RenderClose(float PosX, float PosY);
	bool CheckClose(int PosX, int PosY);

	void RenderBack(float PosX, float PosY);
	bool CheckBack(int PosX, int PosY);

	void RenderBox(float PosX, float PosY, float Width, float Height);

private:

	eMenuOptions CurrentOption;

	/* Main Pos */
	float MainPosX;
	float MainPosY;

	/* Box Max Sizes */
	int BoxWidth;
	int BoxHeight;

	/* General */
	bool PVPWithoutControl;

	/* Antilag */
	bool DeleteShadows;
	bool DeleteObjects;
	bool DeleteFloor;
	bool DeleteSkills;
	bool DeleteStaticEffects;
	bool DeleteDynamicEffects;
	bool DeleteInterface;

	/* Resolution */
	int Resolutions[MAX_RESOLUTION_VALUE];
};

extern COptionsMenu gOptionsMenu;