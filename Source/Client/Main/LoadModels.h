#pragma once

struct TEXTURE_INFO
{
	char Name[32];
	float Width;
	float Height;
	DWORD Component;
	GLuint Texture;
	DWORD Ref;
	BYTE* Buffer;
};

class CLoadModels
{
public:

	CLoadModels();

	virtual ~CLoadModels();

	void Init();

private:

	void SetTexturesOffset();

	static void MyAccessModel(int Type, char* Dir, char* FileName, int i = -1);

	void CheckModelExists(int Model, char* SubFolder, char* filename);

	static void MyOpenTexture(int Model, char* SubFolder, int Type = GL_NEAREST, bool Check = true);

	short TextureCheck(char* name, DWORD* index);

	void CheckTextureExists(int Model, char* SubFolder, char* filename, char* ext);

	static void OpenPlayerTexturesHook();

	static void OpenItemTexturesHook();

	static void PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon);

private:

	TEXTURE_INFO lpTextures[TOTAL_TEXTURE];

	short TextureCount;

	DWORD TextureType;

	DWORD TextureOverFlow;
};

extern CLoadModels gLoadModels;