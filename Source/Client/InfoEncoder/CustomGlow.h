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

	void Init();

	void Load(char* path);

	void SetInfo(CUSTOM_GLOW_INFO info);

public:

	CUSTOM_GLOW_INFO m_CustomGlowInfo[MAX_ITEM];
};

extern CCustomGlow gCustomGlow;