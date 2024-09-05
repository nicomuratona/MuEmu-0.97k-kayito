#pragma once

class CLoadModels
{
public:

	CLoadModels();

	virtual ~CLoadModels();

	void Init();

	void MyAccessModel(int Type, char* Dir, char* FileName, int i = -1);

	void MyOpenTexture(int Model, char* SubFolder, int Type = GL_NEAREST, bool Check = true);

	BITMAP_t* GetBitmap(int uiTextureNumber);

private:

	void SetTexturesOffset();

	void CheckModelExists(int Model, char* SubFolder, char* filename);

	void CheckTextureExists(int Model, char* SubFolder, char* filename, char* ext);

	static void OpenItemsHook();

	static void OpenItemTexturesHook();

	static void PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon);

private:

	BITMAP_t lpTextures[TOTAL_TEXTURE];

	short TextureCount;
};

extern CLoadModels gLoadModels;