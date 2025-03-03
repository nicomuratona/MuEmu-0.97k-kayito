#include "stdafx.h"
#include "Sound.h"
#include "MP3.h"

CSound gSound;

CSound::CSound()
{
	m_SoundOnOff = GetPrivateProfileInt("Sound", "EnableSound", 1, ".\\Config.ini");

	this->SoundVolumeLevel = GetPrivateProfileInt("Sound", "SoundLevel", 4, ".\\Config.ini");

	m_MusicOnOff = GetPrivateProfileInt("Sound", "EnableMusic", 1, ".\\Config.ini");

	this->MusicVolumeLevel = GetPrivateProfileInt("Sound", "MusicLevel", 4, ".\\Config.ini");

	this->g_MasterVolume = 0L;

	this->MusicFileName.clear();

	this->MusicPlaying = true;
}

CSound::~CSound()
{
	char Text[33] = { 0 };

	wsprintf(Text, "%d", m_SoundOnOff);

	WritePrivateProfileString("Sound", "EnableSound", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->SoundVolumeLevel);

	WritePrivateProfileString("Sound", "SoundLevel", Text, ".\\Config.ini");

	wsprintf(Text, "%d", m_MusicOnOff);

	WritePrivateProfileString("Sound", "EnableMusic", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->MusicVolumeLevel);

	WritePrivateProfileString("Sound", "MusicLevel", Text, ".\\Config.ini");
}

void CSound::Init()
{
	this->InitSoundConvertion();

	this->UpdateSoundVolumeLevel(this->SoundVolumeLevel);

	SetCompleteHook(0xE8, 0x004399A8, &this->ReceiveLevelUpHook);

	SetCompleteHook(0xE9, 0x00404A10, &this->MyLoadWaveFile);

	SetCompleteHook(0xE9, 0x00412890, &this->MyPlayMP3);

	SetCompleteHook(0xE9, 0x004127F0, &this->MyStopMP3);
}

void CSound::InitSoundConvertion()
{
	SetDword(0x0050F10D + 1, (DWORD) & "Data\\Sound\\aBat.wav");
	SetDword(0x0050F0EA + 1, (DWORD) & "Data\\Sound\\aBird1.wav");
	SetDword(0x0050F0FD + 1, (DWORD) & "Data\\Sound\\aBird2.wav");
	SetDword(0x0050F150 + 1, (DWORD) & "Data\\Sound\\aCastleDoor.wav");
	SetDword(0x0050F140 + 1, (DWORD) & "Data\\Sound\\aDoor.wav");
	SetDword(0x0050F054 + 1, (DWORD) & "Data\\Sound\\aDungeon.wav");
	SetDword(0x0050F064 + 1, (DWORD) & "Data\\Sound\\aForest.wav");
	SetDword(0x0050F12D + 1, (DWORD) & "Data\\Sound\\aGrate.wav");
	SetDword(0x0050F11D + 1, (DWORD) & "Data\\Sound\\aMouse.wav");
	SetDword(0x0050F044 + 1, (DWORD) & "Data\\Sound\\aRain.wav");
	SetDword(0x0050F077 + 1, (DWORD) & "Data\\Sound\\aTower.wav");
	SetDword(0x0050F087 + 1, (DWORD) & "Data\\Sound\\aWater.wav");
	SetDword(0x0050F034 + 1, (DWORD) & "Data\\Sound\\aWind.wav");
	SetDword(0x0050F1D6 + 1, (DWORD) & "Data\\Sound\\eBlow1.wav");
	SetDword(0x0050F1E6 + 1, (DWORD) & "Data\\Sound\\eBlow2.wav");
	SetDword(0x0050F1F6 + 1, (DWORD) & "Data\\Sound\\eBlow3.wav");
	SetDword(0x0050F209 + 1, (DWORD) & "Data\\Sound\\eBlow4.wav");
	SetDword(0x0050F24C + 1, (DWORD) & "Data\\Sound\\eBow.wav");
	SetDword(0x0050F444 + 1, (DWORD) & "Data\\Sound\\eBreak.wav");
	SetDword(0x0050F25C + 1, (DWORD) & "Data\\Sound\\eCrossbow.wav");
	SetDword(0x0050F454 + 1, (DWORD) & "Data\\Sound\\eExplosion.wav");
	SetDword(0x0050F345 + 1, (DWORD) & "Data\\Sound\\eGem.wav");
	SetDword(0x0050F467 + 1, (DWORD) & "Data\\Sound\\eMeteorite.wav");
	SetDword(0x0050F26C + 1, (DWORD) & "Data\\Sound\\eMix.wav");
	SetDword(0x0050F1A3 + 1, (DWORD) & "Data\\Sound\\eShortBlow1.wav");
	SetDword(0x0050F1B3 + 1, (DWORD) & "Data\\Sound\\eShortBlow2.wav");
	SetDword(0x0050F1C6 + 1, (DWORD) & "Data\\Sound\\eShortBlow3.wav");
	SetDword(0x0050F239 + 1, (DWORD) & "Data\\Sound\\eSwingLightSword.wav");
	SetDword(0x0050F219 + 1, (DWORD) & "Data\\Sound\\eSwingWeapon1.wav");
	SetDword(0x0050F229 + 1, (DWORD) & "Data\\Sound\\eSwingWeapon2.wav");
	SetDword(0x0050F477 + 1, (DWORD) & "Data\\Sound\\eThunder.wav");
	SetDword(0x00510F85 + 1, (DWORD) & "Data\\Sound\\iButtonClick.wav");
	SetDword(0x00510F98 + 1, (DWORD) & "Data\\Sound\\iButtonError.wav");
	SetDword(0x00510F75 + 1, (DWORD) & "Data\\Sound\\iButtonMove.wav");
	SetDword(0x00510FA8 + 1, (DWORD) & "Data\\Sound\\iCreateWindow.wav");
	SetDword(0x00510F65 + 1, (DWORD) & "Data\\Sound\\iTitle.wav");
	SetDword(0x0050A5BE + 1, (DWORD) & "Data\\Sound\\mArgon1.wav");
	SetDword(0x0050A5D1 + 1, (DWORD) & "Data\\Sound\\mArgon2.wav");
	SetDword(0x0050A5E4 + 1, (DWORD) & "Data\\Sound\\mArgonAttack1.wav");
	SetDword(0x0050A5F7 + 1, (DWORD) & "Data\\Sound\\mArgonAttack2.wav");
	SetDword(0x0050A60D + 1, (DWORD) & "Data\\Sound\\mArgonDie.wav");
	SetDword(0x0050F4AA + 1, (DWORD) & "Data\\Sound\\mAssassin1.wav");
	SetDword(0x0050A240 + 1, (DWORD) & "Data\\Sound\\mAssassinAttack1.wav");
	SetDword(0x0050A253 + 1, (DWORD) & "Data\\Sound\\mAssassinAttack2.wav");
	SetDword(0x0050A266 + 1, (DWORD) & "Data\\Sound\\mAssassinDie.wav");
	SetDword(0x0050A962 + 1, (DWORD) & "Data\\Sound\\mBahamut1.wav");
	SetDword(0x0050A8C8 + 1, (DWORD) & "Data\\Sound\\mBali1.wav");
	SetDword(0x0050A8DB + 1, (DWORD) & "Data\\Sound\\mBali2.wav");
	SetDword(0x0050A8EE + 1, (DWORD) & "Data\\Sound\\mBaliAttack1.wav");
	SetDword(0x0050A901 + 1, (DWORD) & "Data\\Sound\\mBaliAttack2.wav");
	SetDword(0x0050AA34 + 1, (DWORD) & "Data\\Sound\\mBaliAttack2.wav");
	SetDword(0x0050A732 + 1, (DWORD) & "Data\\Sound\\mBalrog1.wav");
	SetDword(0x0050A9D5 + 1, (DWORD) & "Data\\Sound\\mBalrog1.wav");
	SetDword(0x0050A745 + 1, (DWORD) & "Data\\Sound\\mBalrog2.wav");
	SetDword(0x0050A781 + 1, (DWORD) & "Data\\Sound\\mBalrogDie.wav");
	SetDword(0x0050A44A + 1, (DWORD) & "Data\\Sound\\mBeetle1.wav");
	SetDword(0x0050A45D + 1, (DWORD) & "Data\\Sound\\mBeetleAttack1.wav");
	SetDword(0x0050A470 + 1, (DWORD) & "Data\\Sound\\mBeetleDie.wav");
	SetDword(0x0050A9AF + 1, (DWORD) & "Data\\Sound\\mBepar1.wav");
	SetDword(0x0050A9C2 + 1, (DWORD) & "Data\\Sound\\mBepar2.wav");
	SetDword(0x0050F487 + 1, (DWORD) & "Data\\Sound\\mBone1.wav");
	SetDword(0x0050F497 + 1, (DWORD) & "Data\\Sound\\mBone2.wav");
	SetDword(0x00509BFC + 1, (DWORD) & "Data\\Sound\\mBudge1.wav");
	SetDword(0x00509C0F + 1, (DWORD) & "Data\\Sound\\mBudgeAttack1.wav");
	SetDword(0x00509C22 + 1, (DWORD) & "Data\\Sound\\mBudgeDie.wav");
	SetDword(0x00509ADF + 1, (DWORD) & "Data\\Sound\\mBull1.wav");
	SetDword(0x00509AF2 + 1, (DWORD) & "Data\\Sound\\mBull2.wav");
	SetDword(0x00509B05 + 1, (DWORD) & "Data\\Sound\\mBullAttack1.wav");
	SetDword(0x0050A865 + 1, (DWORD) & "Data\\Sound\\mBullAttack1.wav");
	SetDword(0x00509B18 + 1, (DWORD) & "Data\\Sound\\mBullAttack2.wav");
	SetDword(0x00509B2E + 1, (DWORD) & "Data\\Sound\\mBullDie.wav");
	SetDword(0x0050AE36 + 1, (DWORD) & "Data\\Sound\\mBullDie.wav");
	SetDword(0x0050AE89 + 1, (DWORD) & "Data\\Sound\\mBullDie.wav");
	SetDword(0x00509C88 + 1, (DWORD) & "Data\\Sound\\mDarkKnight1.wav");
	SetDword(0x00509C9B + 1, (DWORD) & "Data\\Sound\\mDarkKnight2.wav");
	SetDword(0x00509CAE + 1, (DWORD) & "Data\\Sound\\mDarkKnightAttack1.wav");
	SetDword(0x00509CC1 + 1, (DWORD) & "Data\\Sound\\mDarkKnightAttack2.wav");
	SetDword(0x00509CD7 + 1, (DWORD) & "Data\\Sound\\mDarkKnightDie.wav");
	SetDword(0x00509F01 + 1, (DWORD) & "Data\\Sound\\mGhost1.wav");
	SetDword(0x00509F14 + 1, (DWORD) & "Data\\Sound\\mGhost2.wav");
	SetDword(0x00509F27 + 1, (DWORD) & "Data\\Sound\\mGhostAttack1.wav");
	SetDword(0x00509F3A + 1, (DWORD) & "Data\\Sound\\mGhostAttack2.wav");
	SetDword(0x00509F50 + 1, (DWORD) & "Data\\Sound\\mGhostDie.wav");
	SetDword(0x00509DD8 + 1, (DWORD) & "Data\\Sound\\mGiant1.wav");
	SetDword(0x00509DEB + 1, (DWORD) & "Data\\Sound\\mGiant2.wav");
	SetDword(0x00509DFE + 1, (DWORD) & "Data\\Sound\\mGiantAttack1.wav");
	SetDword(0x00509E11 + 1, (DWORD) & "Data\\Sound\\mGiantAttack2.wav");
	SetDword(0x00509E27 + 1, (DWORD) & "Data\\Sound\\mGiantDie.wav");
	SetDword(0x0050A347 + 1, (DWORD) & "Data\\Sound\\mGoblin1.wav");
	SetDword(0x0050A35A + 1, (DWORD) & "Data\\Sound\\mGoblin2.wav");
	SetDword(0x0050A36D + 1, (DWORD) & "Data\\Sound\\mGoblinAttack1.wav");
	SetDword(0x0050A380 + 1, (DWORD) & "Data\\Sound\\mGoblinAttack2.wav");
	SetDword(0x0050A396 + 1, (DWORD) & "Data\\Sound\\mGoblinDie.wav");
	SetDword(0x0050A648 + 1, (DWORD) & "Data\\Sound\\mGolem1.wav");
	SetDword(0x0050A65B + 1, (DWORD) & "Data\\Sound\\mGolem2.wav");
	SetDword(0x0050A66E + 1, (DWORD) & "Data\\Sound\\mGolemAttack1.wav");
	SetDword(0x0050A681 + 1, (DWORD) & "Data\\Sound\\mGolemAttack2.wav");
	SetDword(0x0050A697 + 1, (DWORD) & "Data\\Sound\\mGolemDie.wav");
	SetDword(0x0050A014 + 1, (DWORD) & "Data\\Sound\\mGorgon1.wav");
	SetDword(0x0050A027 + 1, (DWORD) & "Data\\Sound\\mGorgon2.wav");
	SetDword(0x0050A03A + 1, (DWORD) & "Data\\Sound\\mGorgonAttack1.wav");
	SetDword(0x0050A04D + 1, (DWORD) & "Data\\Sound\\mGorgonAttack2.wav");
	SetDword(0x0050A76B + 1, (DWORD) & "Data\\Sound\\mGorgonAttack2.wav");
	SetDword(0x0050A063 + 1, (DWORD) & "Data\\Sound\\mGorgonDie.wav");
	SetDword(0x0050AAB6 + 1, (DWORD) & "Data\\Sound\\mGorgonDie.wav");
	SetDword(0x00509EA0 + 1, (DWORD) & "Data\\Sound\\mHellSpider1.wav");
	SetDword(0x00509EB3 + 1, (DWORD) & "Data\\Sound\\mHellSpiderAttack1.wav");
	SetDword(0x00509EC6 + 1, (DWORD) & "Data\\Sound\\mHellSpiderDie.wav");
	SetDword(0x0050A14F + 1, (DWORD) & "Data\\Sound\\mHomord1.wav");
	SetDword(0x0050A162 + 1, (DWORD) & "Data\\Sound\\mHomord2.wav");
	SetDword(0x0050A175 + 1, (DWORD) & "Data\\Sound\\mHomordAttack1.wav");
	SetDword(0x0050A188 + 1, (DWORD) & "Data\\Sound\\mHomordDie.wav");
	SetDword(0x00509B73 + 1, (DWORD) & "Data\\Sound\\mHound1.wav");
	SetDword(0x00509B86 + 1, (DWORD) & "Data\\Sound\\mHound2.wav");
	SetDword(0x00509B99 + 1, (DWORD) & "Data\\Sound\\mHoundAttack1.wav");
	SetDword(0x00509BAC + 1, (DWORD) & "Data\\Sound\\mHoundAttack2.wav");
	SetDword(0x00509BC2 + 1, (DWORD) & "Data\\Sound\\mHoundDie.wav");
	SetDword(0x0050A4AB + 1, (DWORD) & "Data\\Sound\\mHunter1.wav");
	SetDword(0x0050A4BE + 1, (DWORD) & "Data\\Sound\\mHunter2.wav");
	SetDword(0x0050AE76 + 1, (DWORD) & "Data\\Sound\\mHunter2.wav");
	SetDword(0x0050B28C + 1, (DWORD) & "Data\\Sound\\mHunter2.wav");
	SetDword(0x0050B2E9 + 1, (DWORD) & "Data\\Sound\\mHunter2.wav");
	SetDword(0x0050A4D1 + 1, (DWORD) & "Data\\Sound\\mHunterAttack1.wav");
	SetDword(0x0050A4E4 + 1, (DWORD) & "Data\\Sound\\mHunterAttack2.wav");
	SetDword(0x0050A4FA + 1, (DWORD) & "Data\\Sound\\mHunterDie.wav");
	SetDword(0x0050AB23 + 1, (DWORD) & "Data\\Sound\\mHydra1.wav");
	SetDword(0x0050AB36 + 1, (DWORD) & "Data\\Sound\\mHydraAttack1.wav");
	SetDword(0x0050A09E + 1, (DWORD) & "Data\\Sound\\mIceMonster1.wav");
	SetDword(0x0050A0B1 + 1, (DWORD) & "Data\\Sound\\mIceMonster2.wav");
	SetDword(0x0050A0C4 + 1, (DWORD) & "Data\\Sound\\mIceMonsterDie.wav");
	SetDword(0x0050A1B6 + 1, (DWORD) & "Data\\Sound\\mIceQueen1.wav");
	SetDword(0x0050A1C9 + 1, (DWORD) & "Data\\Sound\\mIceQueen2.wav");
	SetDword(0x0050A1DC + 1, (DWORD) & "Data\\Sound\\mIceQueenAttack1.wav");
	SetDword(0x0050A1EF + 1, (DWORD) & "Data\\Sound\\mIceQueenAttack2.wav");
	SetDword(0x0050A205 + 1, (DWORD) & "Data\\Sound\\mIceQueenDie.wav");
	SetDword(0x00509E62 + 1, (DWORD) & "Data\\Sound\\mLarva1.wav");
	SetDword(0x00509E75 + 1, (DWORD) & "Data\\Sound\\mLarva2.wav");
	SetDword(0x0050AA90 + 1, (DWORD) & "Data\\Sound\\mLizardKing1.wav");
	SetDword(0x0050AAA3 + 1, (DWORD) & "Data\\Sound\\mLizardKing2.wav");
	SetDword(0x00509F8A + 1, (DWORD) & "Data\\Sound\\mOgre1.wav");
	SetDword(0x00509F9D + 1, (DWORD) & "Data\\Sound\\mOgre2.wav");
	SetDword(0x00509FB0 + 1, (DWORD) & "Data\\Sound\\mOgreAttack1.wav");
	SetDword(0x00509FC3 + 1, (DWORD) & "Data\\Sound\\mOgreAttack2.wav");
	SetDword(0x00509FD9 + 1, (DWORD) & "Data\\Sound\\mOgreDie.wav");
	SetDword(0x0050A6E5 + 1, (DWORD) & "Data\\Sound\\mSatanAttack1.wav");
	SetDword(0x0050A3D0 + 1, (DWORD) & "Data\\Sound\\mScorpion1.wav");
	SetDword(0x0050A3E3 + 1, (DWORD) & "Data\\Sound\\mScorpion2.wav");
	SetDword(0x0050A3F6 + 1, (DWORD) & "Data\\Sound\\mScorpionAttack1.wav");
	SetDword(0x0050A409 + 1, (DWORD) & "Data\\Sound\\mScorpionAttack2.wav");
	SetDword(0x0050A41F + 1, (DWORD) & "Data\\Sound\\mScorpionDie.wav");
	SetDword(0x0050A7C9 + 1, (DWORD) & "Data\\Sound\\mShadow1.wav");
	SetDword(0x0050A7DC + 1, (DWORD) & "Data\\Sound\\mShadow2.wav");
	SetDword(0x0050A7EF + 1, (DWORD) & "Data\\Sound\\mShadowAttack1.wav");
	SetDword(0x0050F3BB + 1, (DWORD) & "Data\\Sound\\mShadowAttack1.wav");
	SetDword(0x0050A802 + 1, (DWORD) & "Data\\Sound\\mShadowAttack2.wav");
	SetDword(0x0050A818 + 1, (DWORD) & "Data\\Sound\\mShadowDie.wav");
	SetDword(0x00509C5D + 1, (DWORD) & "Data\\Sound\\mSpider1.wav");
	SetDword(0x0050AA21 + 1, (DWORD) & "Data\\Sound\\mValkyrie1.wav");
	SetDword(0x0050AA47 + 1, (DWORD) & "Data\\Sound\\mValkyrieDie.wav");
	SetDword(0x00509D4F + 1, (DWORD) & "Data\\Sound\\mWizard1.wav");
	SetDword(0x00509D62 + 1, (DWORD) & "Data\\Sound\\mWizard2.wav");
	SetDword(0x00509D75 + 1, (DWORD) & "Data\\Sound\\mWizardAttack1.wav");
	SetDword(0x00509D88 + 1, (DWORD) & "Data\\Sound\\mWizardAttack2.wav");
	SetDword(0x0050A758 + 1, (DWORD) & "Data\\Sound\\mWizardAttack2.wav");
	SetDword(0x00509D9E + 1, (DWORD) & "Data\\Sound\\mWizardDie.wav");
	SetDword(0x0050A535 + 1, (DWORD) & "Data\\Sound\\mWoodMon1.wav");
	SetDword(0x0050A548 + 1, (DWORD) & "Data\\Sound\\mWoodMon2.wav");
	SetDword(0x0050A55B + 1, (DWORD) & "Data\\Sound\\mWoodMonAttack1.wav");
	SetDword(0x0050A56E + 1, (DWORD) & "Data\\Sound\\mWoodMonAttack2.wav");
	SetDword(0x0050A584 + 1, (DWORD) & "Data\\Sound\\mWoodMonDie.wav");
	SetDword(0x0050A0FE + 1, (DWORD) & "Data\\Sound\\mWorm1.wav");
	SetDword(0x0050A111 + 1, (DWORD) & "Data\\Sound\\mWorm2.wav");
	SetDword(0x0050A124 + 1, (DWORD) & "Data\\Sound\\mWormDie.wav");
	SetDword(0x0050A2AB + 1, (DWORD) & "Data\\Sound\\mYeti1.wav");
	SetDword(0x0050A6D2 + 1, (DWORD) & "Data\\Sound\\mYeti1.wav");
	SetDword(0x0050A852 + 1, (DWORD) & "Data\\Sound\\mYeti1.wav");
	SetDword(0x0050A975 + 1, (DWORD) & "Data\\Sound\\mYeti1.wav");
	SetDword(0x0050A2BE + 1, (DWORD) & "Data\\Sound\\mYeti2.wav");
	SetDword(0x0050A2D1 + 1, (DWORD) & "Data\\Sound\\mYetiAttack1.wav");
	SetDword(0x0050A2E4 + 1, (DWORD) & "Data\\Sound\\mYetiDie.wav");
	SetDword(0x0050A6F8 + 1, (DWORD) & "Data\\Sound\\mYetiDie.wav");
	SetDword(0x0050A878 + 1, (DWORD) & "Data\\Sound\\mYetiDie.wav");
	SetDword(0x005094C3 + 1, (DWORD) & "Data\\Sound\\nBlackSmith.wav");
	SetDword(0x00509569 + 1, (DWORD) & "Data\\Sound\\nHarp.wav");
	SetDword(0x0050960F + 1, (DWORD) & "Data\\Sound\\nMix.wav");
	SetDword(0x0050F27C + 1, (DWORD) & "Data\\Sound\\pDrink.wav");
	SetDword(0x0050F325 + 1, (DWORD) & "Data\\Sound\\pDropItem.wav");
	SetDword(0x0050F335 + 1, (DWORD) & "Data\\Sound\\pDropMoney.wav");
	SetDword(0x0050F28F + 1, (DWORD) & "Data\\Sound\\pEatApple.wav");
	SetDword(0x0050F2AF + 1, (DWORD) & "Data\\Sound\\pEnergy.wav");
	SetDword(0x0050F302 + 1, (DWORD) & "Data\\Sound\\pFemaleScream1.wav");
	SetDword(0x0050F315 + 1, (DWORD) & "Data\\Sound\\pFemaleScream2.wav");
	SetDword(0x0050F358 + 1, (DWORD) & "Data\\Sound\\pGetItem.wav");
	SetDword(0x0050F29F + 1, (DWORD) & "Data\\Sound\\pHeartBeat.wav");
	SetDword(0x0050F2F2 + 1, (DWORD) & "Data\\Sound\\pMaleDie.wav");
	SetDword(0x0050F2BF + 1, (DWORD) & "Data\\Sound\\pMaleScream1.wav");
	SetDword(0x0050F2D2 + 1, (DWORD) & "Data\\Sound\\pMaleScream2.wav");
	SetDword(0x0050F2E2 + 1, (DWORD) & "Data\\Sound\\pMaleScream3.wav");
	SetDword(0x0050F0DA + 1, (DWORD) & "Data\\Sound\\pSwim.wav");
	SetDword(0x0050F0BA + 1, (DWORD) & "Data\\Sound\\pWalk(Grass).wav");
	SetDword(0x0050F0CA + 1, (DWORD) & "Data\\Sound\\pWalk(Snow).wav");
	SetDword(0x0050F0A7 + 1, (DWORD) & "Data\\Sound\\pWalk(Soil).wav");
	SetDword(0x0050F434 + 1, (DWORD) & "Data\\Sound\\sAquaFlash.wav");
	SetDword(0x0050F3E1 + 1, (DWORD) & "Data\\Sound\\sEvil.wav");
	SetDword(0x0050F424 + 1, (DWORD) & "Data\\Sound\\sFlame.wav");
	SetDword(0x0050F401 + 1, (DWORD) & "Data\\Sound\\sHellFire.wav");
	SetDword(0x0050F411 + 1, (DWORD) & "Data\\Sound\\sIce.wav");
	SetDword(0x0050F368 + 1, (DWORD) & "Data\\Sound\\sKnightDefense.wav");
	SetDword(0x0050F378 + 1, (DWORD) & "Data\\Sound\\sKnightSkill1.wav");
	SetDword(0x0050F388 + 1, (DWORD) & "Data\\Sound\\sKnightSkill2.wav");
	SetDword(0x0050F39B + 1, (DWORD) & "Data\\Sound\\sKnightSkill3.wav");
	SetDword(0x0050F3AB + 1, (DWORD) & "Data\\Sound\\sKnightSkill4.wav");
	SetDword(0x0050F3F1 + 1, (DWORD) & "Data\\Sound\\sMagic.wav");
	SetDword(0x0050F3CE + 1, (DWORD) & "Data\\Sound\\sTornado.wav");
}

void CSound::ReceiveLevelUpHook(BYTE* ReceiveBuffer)
{
	((void(__cdecl*)(BYTE * ReceiveBuffer))0x00431180)(ReceiveBuffer);

	LoadWaveFile(400, (TCHAR*)"Data\\Sound\\pLevelUp.wav", 1, false);

	PlayBuffer(400, NULL, FALSE);
}

void CSound::MyLoadWaveFile(int Buffer, TCHAR* strFileName, int MaxChannel, bool Enable)
{
	if (!g_EnableSound)
	{
		return;
	}

	if (Buffer < 0)
	{
		return;
	}

	if (MaxBufferChannel[Buffer] > 0)
	{
		return;
	}

	if (!g_Enable3DSound)
	{
		Enable = false;
	}

	// Create the sound buffer object from the wave file data
	if (FAILED(CreateStaticBuffer(Buffer, strFileName, MaxChannel, Enable)))
	{
		return;
	}
	else if (FAILED(FillBuffer(Buffer, MaxChannel, Enable)))  // The sound buffer was successfully created
	{
		return;
	}

	BufferChannel[Buffer] = 0;

	MaxBufferChannel[Buffer] = MaxChannel;

	Enable3DSound[Buffer] = Enable;

	strcpy_s((char*)(BufferName + Buffer * 64), 64, strFileName);

	SoundLoadCount += 1;

	gSound.SetVolume(Buffer, gSound.g_MasterVolume);
}

void CSound::MyPlayMP3(char* Name, BOOL bEnforce)
{
	if (Destroy)
	{
		return;
	}

	if (!m_MusicOnOff && !bEnforce)
	{
		return;
	}

	std::string newest(Name);

	std::wstring w_newest(newest.begin(), newest.end());

	if (w_newest.compare(gSound.MusicFileName) == 0)
	{
		return;
	}

	gSound.MusicFileName = w_newest;

	if (gMP3.Init(gSound.MusicFileName.c_str()))
	{
		gConsole.Write("[Music] Now playing: %s", Name);

		gSound.UpdateMusicVolumeLevel(gSound.MusicVolumeLevel);

		if (gSound.MusicPlaying)
		{
			if (!gMP3.Play())
			{
				gConsole.Write("[Music] Failed to play: %s", Name);
			}
		}
	}
	else
	{
		gConsole.Write("[Music] Failed to load: %s", Name);
	}
}

void CSound::MyStopMP3(char* Name, BOOL bEnforce)
{
	if (!m_MusicOnOff && !bEnforce)
	{
		return;
	}

	if (gSound.MusicFileName.empty())
	{
		return;
	}

	if (!gSound.MusicPlaying)
	{
		return;
	}

	std::string newest(Name);

	std::wstring w_newest(newest.begin(), newest.end());

	if (w_newest.compare(gSound.MusicFileName) == 0)
	{
		if (gMP3.Stop())
		{
			gConsole.Write("[Music] Stoped playing: %s", Name);
		}
	}
}

void CSound::UpdateSoundVolumeLevel(int volumeLevel)
{
	if (volumeLevel > MAX_SOUND_LEVEL)
	{
		volumeLevel = MAX_SOUND_LEVEL;
	}

	this->SoundVolumeLevel = volumeLevel;

	if (volumeLevel == 0)
	{
		this->SetMasterVolume(-10000);
	}
	else
	{
		long vol = (625 * (volumeLevel - 1)) - 5000;

		this->SetMasterVolume(vol);
	}
}

void CSound::UpdateMusicVolumeLevel(int volumeLevel)
{
	if (volumeLevel > MAX_MUSIC_LEVEL)
	{
		volumeLevel = MAX_MUSIC_LEVEL;
	}

	this->MusicVolumeLevel = volumeLevel;

	if (volumeLevel == 0)
	{
		gMP3.SetVolume(-10000);
	}
	else
	{
		long vol = (625 * (volumeLevel - 1)) - 5000;

		gMP3.SetVolume(vol);
	}
}

void CSound::ButtonStopMusic()
{
	if (!m_MusicOnOff)
	{
		return;
	}

	if (this->MusicFileName.empty())
	{
		return;
	}

	if (!this->MusicPlaying)
	{
		return;
	}

	this->MusicPlaying = !gMP3.Stop();
}

void CSound::ButtonPlayMusic()
{
	if (!m_MusicOnOff)
	{
		return;
	}

	if (this->MusicPlaying)
	{
		return;
	}

	this->MusicPlaying = gMP3.Play();
}

void CSound::SetMasterVolume(long vol)
{
	this->g_MasterVolume = vol;

	if (!g_EnableSound)
	{
		return;
	}

	for (int i = 0; i < 420; ++i)
	{
		this->SetVolume(i, vol);
	}
}

void CSound::SetVolume(int Buffer, long vol)
{
	if (!g_EnableSound)
	{
		return;
	}

	if (Buffer < 0)
	{
		return;
	}

	if (g_lpDSBuffer[Buffer][0])
	{
		if (vol > DSBVOLUME_MAX)
		{
			vol = DSBVOLUME_MAX;
		}

		if (vol < DSBVOLUME_MIN)
		{
			vol = DSBVOLUME_MIN;
		}

		g_lpDSBuffer[Buffer][0]->SetVolume(vol);
	}
}