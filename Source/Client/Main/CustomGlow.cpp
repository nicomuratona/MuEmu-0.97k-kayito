#include "stdafx.h"
#include "CustomGlow.h"

CCustomGlow gCustomGlow;

CCustomGlow::CCustomGlow()
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->m_CustomGlowInfo[n].ItemIndex = -1;
	}
}

CCustomGlow::~CCustomGlow()
{

}

void CCustomGlow::Load(CUSTOM_GLOW_INFO* info)
{
	for (int n = 0; n < MAX_ITEM; n++)
	{
		this->SetInfo(info[n]);
	}
}

void CCustomGlow::SetInfo(CUSTOM_GLOW_INFO info)
{
	if (info.ItemIndex < 0 || info.ItemIndex >= MAX_ITEM)
	{
		return;
	}

	this->m_CustomGlowInfo[info.ItemIndex] = info;
}

CUSTOM_GLOW_INFO* CCustomGlow::GetInfoByIndex(int ItemIndex)
{
	if (ItemIndex < 0 || ItemIndex >= MAX_ITEM)
	{
		return 0;
	}

	if (this->m_CustomGlowInfo[ItemIndex].ItemIndex != ItemIndex)
	{
		return 0;
	}

	return &this->m_CustomGlowInfo[ItemIndex];
}

bool CCustomGlow::GetItemGlow(int ItemIndex, float* ItemColor)
{
	CUSTOM_GLOW_INFO* lpInfo = this->GetInfoByIndex(ItemIndex);

	if (lpInfo == 0)
	{
		return false;
	}

	ItemColor[0] = (lpInfo->ColorR / 255.0f);

	ItemColor[1] = (lpInfo->ColorG / 255.0f);

	ItemColor[2] = (lpInfo->ColorB / 255.0f);

	return true;
}