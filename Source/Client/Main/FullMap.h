#pragma once

#define MAP_SIZE 512
#define MAP_TEXTURE_NUMBER 1960

class CFullMap
{
public:

	CFullMap();

	virtual ~CFullMap();

	void Init();

	void LoadImages();

	void Toggle();

	void CheckZoomButton();

private:

	static void RenderFullMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void RenderGates();

	void RenderCharacters();

	static void RenderZoom(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void ChangeZoom();

	bool CheckInterfaces();

	void LoadMap();

	void LoadGates();

	void PaintBorder(int sx, int sy, BYTE* bitmap);

private:

	bool FullMapSwitch;

	// Sizes
	float MaxSize;

	float Size;

	int VisualRange;

	float Scale;

	float CharPosX;
	float CharPosY;

	float MapOffsetX;
	float MapOffsetY;

	BYTE ZoomLevel;

	HFONT FullMapFont;

	int LastMap;
	GLuint MapTextureID;

	std::vector<BYTE> m_GatesIndex;
};

extern CFullMap gFullMap;