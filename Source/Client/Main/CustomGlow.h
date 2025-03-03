#pragma once

struct CUSTOM_GLOW_INFO
{
	int ItemIndex;
	int ColorR;
	int ColorG;
	int ColorB;
};

class CCustomGlow
{
public:

	CCustomGlow();

	virtual ~CCustomGlow();

	void Init(CUSTOM_GLOW_INFO* info);

	void SetInfo(CUSTOM_GLOW_INFO info);

	CUSTOM_GLOW_INFO* GetInfoByIndex(int ItemIndex);

	bool GetItemGlow(int ItemIndex, float* ItemColor);

public:

	CUSTOM_GLOW_INFO m_CustomGlowInfo[MAX_ITEM];
};

extern CCustomGlow gCustomGlow;