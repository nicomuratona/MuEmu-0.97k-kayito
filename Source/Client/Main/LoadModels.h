#pragma once

class CLoadModels
{
public:

	CLoadModels();

	virtual ~CLoadModels();

	void Init();

	BITMAP_t* GetBitmap(int uiTextureNumber);

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

	BITMAP_t lpTextures[TOTAL_TEXTURE];

	short TextureCount;

	DWORD TextureType;

	DWORD TextureOverFlow;
};

extern CLoadModels gLoadModels;