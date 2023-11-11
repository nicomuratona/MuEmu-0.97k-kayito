#pragma once

class CFullMap
{
public:

	CFullMap();

	virtual ~CFullMap();

	void Init();

	void Toggle();

	void CheckZoomButton();

private:

	static void RenderFullMap(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void RenderMapZone();

	void PaintBorder(int sx, int sy);

	void RenderCharacters();

	static void RenderZoom(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void ChangeZoom();

	bool CheckInterfaces();

private:

	bool FullMapSwitch;

	// Sizes
	float MaxSizeX;
	float MaxSizeY;

	int VisualRangeX;
	int VisualRangeY;

	float ScaleX;
	float ScaleY;

	// Position
	float StartPosX;
	float StartPosY;

	float CharPosX;
	float CharPosY;

	float MinScaleOffsetX;
	float MaxScaleOffsetX;

	float MinScaleOffsetY;
	float MaxScaleOffsetY;

	BYTE ZoomLevel;

	HFONT FullMapFont;
};

extern CFullMap gFullMap;