#include "stdafx.h"
#include "Interface.h"
#include "Camera.h"
#include "MiniMap.h"
#include "MoveList.h"
#include "Protect.h"
#include "Reconnect.h"
#include "Window.h"

Interface gInterface;

Interface::Interface()
{
	memset(this->Data, 0, sizeof(this->Data));
}

Interface::~Interface()
{

}

void Interface::BindObject(short MonsterID, DWORD ModelID, float Width, float Height, float X, float Y)
{
	this->Data[MonsterID].EventTick = 0;

	this->Data[MonsterID].OnClick = false;

	this->Data[MonsterID].OnShow = false;

	this->Data[MonsterID].ModelID = ModelID;

	this->Data[MonsterID].Width = Width;

	this->Data[MonsterID].Height = Height;

	this->Data[MonsterID].X = X;

	this->Data[MonsterID].Y = Y;

	this->Data[MonsterID].MaxX = X + Width;

	this->Data[MonsterID].MaxY = Y + Height;

	this->Data[MonsterID].Attribute = 0;
}

void Interface::DrawIMG(short ObjectID, float PosX, float PosY, float ScaleX, float ScaleY)
{
	if (this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1)
	{
		this->Data[ObjectID].X = PosX;

		this->Data[ObjectID].Y = PosY;

		this->Data[ObjectID].MaxX = PosX + this->Data[ObjectID].Width;

		this->Data[ObjectID].MaxY = PosY + this->Data[ObjectID].Height;
	}

	RenderBitmap(this->Data[ObjectID].ModelID, PosX, PosY, this->Data[ObjectID].Width, this->Data[ObjectID].Height, 0, 0, ScaleX, ScaleY, 1, 1);
}

void Interface::DrawIMG(short ObjectID, float PosX, float PosY, float u, float v, float ScaleX, float ScaleY)
{
	if (this->Data[ObjectID].X == -1 || this->Data[ObjectID].Y == -1)
	{
		this->Data[ObjectID].X = PosX;

		this->Data[ObjectID].Y = PosY;

		this->Data[ObjectID].MaxX = PosX + this->Data[ObjectID].Width;

		this->Data[ObjectID].MaxY = PosY + this->Data[ObjectID].Height;
	}

	RenderBitmap(this->Data[ObjectID].ModelID, PosX, PosY, this->Data[ObjectID].Width, this->Data[ObjectID].Height, u, v, ScaleX, ScaleY, 1, 1);
}

bool Interface::IsWorkZone(short ObjectID)
{
	if ((MouseX < this->Data[ObjectID].X || MouseX > this->Data[ObjectID].MaxX) || (MouseY < this->Data[ObjectID].Y || MouseY > this->Data[ObjectID].MaxY))
	{
		return false;
	}

	return true;
}

void Interface::Init()
{
	SetCompleteHook(0xE8, 0x00510DEA, &this->LoadImages);

	SetCompleteHook(0xE8, 0x00526266, &this->Work);

	SetCompleteHook(0xE8, 0x0052698A, &this->RenderLogInScene);

	SetCompleteHook(0xE8, 0x005269A2, &this->RenderCharacterScene);

	SetCompleteHook(0xE8, 0x005269B6, &this->RenderMainScene);
}

void Interface::LoadImages()
{
	((void(__cdecl*)()) 0x0050EB80)(); // OpenImages

	gMiniMap.LoadImages();
}

void Interface::Work()
{
	// Check Click Buttons
	gMiniMap.CheckZoomButton();

	gMoveList.CheckMoveListMouse();
	// Check Click Buttons

	((void(_cdecl*)())0x00524E30)(); // MoveMainScene
}

void Interface::RenderLogInScene(HDC Hdc)
{
	SetWindowText(g_hWnd, gProtect.m_MainInfo.WindowName);

	((void(__cdecl*)(HDC Hdc)) 0x00521630)(Hdc);
}

void Interface::RenderCharacterScene(HDC Hdc)
{
	SetWindowText(g_hWnd, gProtect.m_MainInfo.WindowName);

	((void(__cdecl*)(HDC Hdc)) 0x00523B30)(Hdc);
}

void Interface::RenderMainScene()
{
	ReconnectMainProc();

	gWindow.ChangeWindowText();

	((void(__cdecl*)()) 0x00525A00)();
}