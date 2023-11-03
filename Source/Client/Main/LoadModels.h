#pragma once

#define OpenItemTextures ((void(_cdecl*)())0x00508D10)
#define AccessModel ((void(_cdecl*)(int Type, char *Dir, char *FileName, int i))0x005060B0)
#define OpenTexture ((void(_cdecl*)(int Model, char *SubFolder, int Type, bool Check))0x00505C80)
#define PartObjectColor ((void(_cdecl*)(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon))0x00503CF0)

#define TextureBegin *(int*)0x083A4104
#define TextureCurrent *(int*)0x083A4108

#define MAX_TEXTURE 12000

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

	static void OpenItemTexturesHook();

	void LoadModel(int index, char* folder, char* name, int value);

	void LoadTexture(int index, char* folder, char* name);

	static void PartObjectColorHook(int Type, float Alpha, float Bright, float Light[3], bool ExtraMon);

private:

	TEXTURE_INFO lpTextures[MAX_TEXTURE];
};

extern CLoadModels gLoadModels;