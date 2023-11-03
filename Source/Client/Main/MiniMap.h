#pragma once

#define MINIMAP_FRAME_TEXTURE "Interface\\MiniMap.tga"

#define MINIMAP_TEXTURE "World%d\\MiniMap.jpg"
#define MINIMAP_TEXTURE_PATH ".\\Data\\World%d\\MiniMap.ozj"

class CMiniMap
{
public:

	CMiniMap();

	virtual ~CMiniMap();

	void Init();

	void Toggle();

	void LoadImages();

	bool CheckInterfaces();

	static void DrawMiniMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void RenderMiniMap();

	void CheckZoomButton();

	void RenderObjects();

public:

	bool MiniMapSwitch;

	bool ImageLoaded;

	int CurrentMap;

	float m_MiniMapFramePos[2];

	float m_MiniMapFrameSize[2];

	float m_MiniMapSize[2];

	float m_ZoomBtnSize[2];

	int m_iMiniMapScale;

	float m_CharacterPos[2];

	float m_MiniMapScaleOffset[2];
};

extern CMiniMap gMiniMap;