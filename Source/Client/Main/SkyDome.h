#pragma once

#define SKY_TOP_TEXTURE ".\\Data\\World%d\\Sky.bmp"

#define PI 3.1415926535897f
#define DTOR (PI/180.0f)
#define SQR(x) (x*x)

struct SkyStruct
{
	float R;
	float G;
	float B;
};

struct stb_ImageData
{
	BYTE* data;
	GLint imageWidth;
	GLint imageHeight;
	GLint colorChannel;
};

typedef struct
{
	float x, y, z;
	unsigned int color;
	float u, v;
} VERTEX;

class CSkyDome
{
public:

	CSkyDome();

	virtual ~CSkyDome();

	void Init();

	void GenerateSkyDomeStructure(float radius, float dtheta, float dphi, float hTile, float vTile);

	void LoadImages();

	GLuint LoadTexture(BYTE* imageBytes, int imageWidth, int imageHeight, int colorChannel);

	static void DrawSkyDome();

	void RenderSkyDome();

public:

	bool SkyDomeTable[MAX_MAPS];

	char TexturePath[256];

	GLuint texturesID[MAX_MAPS];

	SkyStruct SkyColor;

	VERTEX* Vertices;

	int NumVertices;
};

extern CSkyDome gSkyDome;