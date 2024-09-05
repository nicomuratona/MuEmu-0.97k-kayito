#pragma once

#define MAP_SIZE 512
#define MINIMAP_FRAME_TEXTURE_NAME "Interface\\MiniMap.tga"
#define MINIMAP_FRAME_TEXTURE 10998
#define MINIMAP_TEXTURE 10999

class CMiniMap
{
public:

	CMiniMap();

	virtual ~CMiniMap();

	void Init();

	void LoadImages();

	bool GetMiniMapState();

	void Toggle();

	void UpdateMouse();

	void Render();

private:

	void ChangeZoom(int i = -1);

	void ChangeAlpha();

	static void MiniMapLoad(char* FileName);

	void LoadMiniMapImage();

	void PaintBorder(int sx, int sy, BYTE* bitmap);

	void LoadMiniMapGates();

	// MiniMap

	void UpdateMouseMiniMap();

	void RenderMiniMap();

	void RenderMiniMapBackground();

	void RenderMiniMapCharacters();

	void RenderMiniMapGates();

	void RenderMiniMapFrame();

	void RenderMiniMapButtons();

	// FullMap

	void UpdateMouseFullMap();

	void RenderFullMap();

	void RenderFullMapBackground();

	void RenderFullMapCharacters();

	void RenderFullMapGates();

	void RenderFullMapZoom();

	void RenderFullMapAlpha();

private:

	// State
	bool MiniMapTable[MAX_MAPS];
	bool MiniMapSwitch;

	// Texture
	GLuint MapTextureID;

	// Gates
	std::vector<int> m_GatesIndex;

	// Sizes
	float MaxSize;
	float Size;
	int VisualRange;
	float Scale;

	// Position
	float CharPosX;
	float CharPosY;

	float MapOffsetX;
	float MapOffsetY;

	// For Buttons
	BYTE ZoomLevel;
	BYTE Alpha;

	// Font
	HFONT MiniMapFont;
};

extern CMiniMap gMiniMap;