#pragma once

#define MAX_OBJECT 250

#define MAX_WIN_WIDTH 640

#define MAX_WIN_HEIGHT 480

struct InterfaceObject
{
	DWORD ModelID;
	float Width;
	float Height;
	float X;
	float Y;
	float MaxX;
	float MaxY;
	float ScaleX;
	float ScaleY;
	DWORD EventTick;
	bool OnClick;
	bool OnShow;
	BYTE Attribute;
};

enum ObjectID
{

};

class Interface
{
public:

	Interface();

	virtual ~Interface();

	void Init();

private:

	static void RenderLogInScene(HDC Hdc);

	static void RenderCharacterScene(HDC Hdc);

	static void RenderMainScene();

	static void LoadImages();

	static void MyUpdateWindowsMouse();

	static void MyRenderWindows();

	static void RenderLeftDragon(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	static void RenderRightDragon(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale);

	void BindObject(short ObjectID, DWORD ModelID, float Width, float Height, float X, float Y);

	void DrawIMG(short ObjectID, float PosX, float PosY, float ScaleX, float ScaleY);

	void DrawIMG(short ObjectID, float PosX, float PosY, float u, float v, float ScaleX, float ScaleY);

	bool IsWorkZone(short ObjectID);

public:

	InterfaceObject Data[MAX_OBJECT];
};

extern Interface gInterface;