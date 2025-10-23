#include "stdafx.h"
#include "Camera3D.h"
#include "MapManager.h"

CCamera3D Camera3D;

CCamera3D::CCamera3D()
{
	this->m_Enable = false;

	this->m_IsMove = false;

	this->m_CursorX = 0;
	this->m_CursorY = 0;

	this->m_Zoom.MinPercent = 50.0f;

	this->m_Zoom.MaxPercent = 200.0f;

	this->m_Zoom.Precision = 2.0f;

	this->m_Default.IsLoad = false;

	this->Init();
}

CCamera3D::~CCamera3D()
{

}

void CCamera3D::Init()
{
	this->m_Address.Zoom = (float*)0x00524CC1;

	this->m_Address.RotX = (float*)0x083A42C0;

	this->m_Address.RotY = (float*)0x00552D54;

	this->m_Address.PosZ = (float*)0x0055297C;

	this->m_Address.ClipX[0] = (float*)0x004F8ED8;

	this->m_Address.ClipX[1] = (float*)0x004F8EEE;

	this->m_Address.ClipY[0] = (float*)0x004F8EFE;

	this->m_Address.ClipY[1] = (float*)0x004F8F0E;

	this->m_Address.ClipZ = (float*)0x00552CBC;

	this->m_Address.ClipGL = (float*)0x00524D32;

	//Fix Items
	SetDword(0x00511332 + 2, 0x005528CC);
	SetDword(0x00511360 + 2, 0x005528CC);
	SetDword(0x0051136C + 2, 0x005528CC);

	SetFloat((DWORD)this->m_Address.Zoom, 50.0f);

	SetCompleteHook(0xE9, 0x005120C0, &this->MyRenderNumber);

	this->SetCurrentValue();
}

void CCamera3D::MyRenderNumber(vec3_t Position, int Num, vec3_t Color, float Alpha, float Scale)
{
	vec3_t p;
	VectorCopy(Position, p);

	vec3_t Light[4];
	for (int i = 0; i < 4; i++)
	{
		VectorCopy(Color, Light[i]);
	}

	if (Num == -1)
	{
		float UV[4][2] =
		{
			{ 0.0f, 32.0f / 32.0f },
			{ 32.0f / 256.0f, 32.0f / 32.0f },
			{ 32.0f / 256.0f, 17.0f / 32.0f },
			{ 0.0f, 17.0f / 32.0f }
		};

		RenderSpriteUV(1, p, 45.0f, 20.0f, UV, Light, Alpha);

		return;
	}

	char Text[32];
	_itoa_s(Num, Text, 10);

	unsigned int Length = strlen(Text);

	float textOffset = Length * Scale * 0.125f;
	p[0] -= textOffset;
	p[1] -= textOffset;

	float yawRad = DegToRad(CameraAngle[2]);
	float cosYaw = cos(yawRad);
	float sinYaw = sin(yawRad);
	float spacing = Scale * (float)sqrt(2.0f) * 0.5f;

	for (unsigned int i = 0; i < Length; i++)
	{
		float texOffsetX = (float)(Text[i] - '0') * 16.0f / 256.0f;
		float UV[4][2] =
		{
			{ texOffsetX, 16.0f / 32.0f },
			{ texOffsetX + 16.0f / 256.0f, 16.0f / 32.0f },
			{ texOffsetX + 16.0f / 256.0f, 0.0f },
			{ texOffsetX, 0.0f }
		};

		RenderSpriteUV(1, p, Scale, Scale, UV, Light, Alpha);

		p[0] += spacing * cosYaw;
		p[1] -= spacing * sinYaw;
	}
}

void CCamera3D::Toggle()
{
	if (SceneFlag == MAIN_SCENE)
	{
		this->m_Enable ^= 1;

		if (!this->m_Default.IsLoad)
		{
			this->m_Default.Zoom = (*this->m_Address.Zoom);

			this->m_Default.RotX = (*this->m_Address.RotX);

			this->m_Default.RotY = (*this->m_Address.RotY);

			this->m_Default.PosZ = (*this->m_Address.PosZ);

			this->m_Default.ClipX[0] = (*this->m_Address.ClipX[0]);

			this->m_Default.ClipX[1] = (*this->m_Address.ClipX[1]);

			this->m_Default.ClipY[0] = (*this->m_Address.ClipY[0]);

			this->m_Default.ClipY[1] = (*this->m_Address.ClipY[1]);

			this->m_Default.ClipY[2] = (*this->m_Address.ClipY[2]);

			this->m_Default.ClipGL = (*this->m_Address.ClipGL);

			this->m_Default.IsLoad = true;
		}

		CreateNotice(((this->m_Enable) ? "Camara 3D Enabled" : "Camera 3D Disabled"), 1);
	}
}

void CCamera3D::Restore()
{
	if (this->m_Enable && SceneFlag == MAIN_SCENE)
	{
		this->SetDefaultValue();

		CreateNotice("Camera 3D Restored", 1);
	}
}

void CCamera3D::SetIsMove(bool IsMove)
{
	if (this->m_Enable && SceneFlag == MAIN_SCENE)
	{
		this->m_IsMove = IsMove;
	}
}

void CCamera3D::SetCursorX(LONG CursorX)
{
	if (this->m_Enable && SceneFlag == MAIN_SCENE)
	{
		this->m_CursorX = CursorX;
	}
}

void CCamera3D::SetCursorY(LONG CursorY)
{
	if (this->m_Enable && SceneFlag == MAIN_SCENE)
	{
		this->m_CursorY = CursorY;
	}
}

void CCamera3D::Zoom(MOUSEHOOKSTRUCTEX* lpMouse)
{
	if (!this->m_Enable || this->m_IsMove || SceneFlag != MAIN_SCENE)
	{
		return;
	}

	this->m_Zoom.MinLimit = (this->m_Default.Zoom / 100) * this->m_Zoom.MinPercent;

	this->m_Zoom.MaxLimit = (this->m_Default.Zoom / 100) * this->m_Zoom.MaxPercent;

	short wheelDelta = GET_WHEEL_DELTA_WPARAM(lpMouse->mouseData);

	if (wheelDelta < 0)
	{
		if ((*this->m_Address.Zoom) <= this->m_Zoom.MaxLimit)
		{
			SetFloat((DWORD)this->m_Address.Zoom, ((*this->m_Address.Zoom) + this->m_Zoom.Precision));
		}
	}

	if (wheelDelta > 0)
	{
		if ((*this->m_Address.Zoom) >= this->m_Zoom.MinLimit)
		{
			SetFloat((DWORD)this->m_Address.Zoom, ((*this->m_Address.Zoom) - this->m_Zoom.Precision));
		}
	}

	this->SetCurrentValue();
}

void CCamera3D::Move(MOUSEHOOKSTRUCTEX* lpMouse)
{
	if (!this->m_Enable || !this->m_IsMove || SceneFlag != MAIN_SCENE)
	{
		return;
	}

	if (this->m_CursorX < lpMouse->pt.x)
	{
		if ((*this->m_Address.RotX) > 309.0f)
		{
			SetFloat((DWORD)this->m_Address.RotX, -45.0f);
		}
		else
		{
			SetFloat((DWORD)this->m_Address.RotX, ((*this->m_Address.RotX) + 6.0f));
		}
	}

	if (this->m_CursorX > lpMouse->pt.x)
	{
		if ((*this->m_Address.RotX) < -417.0f)
		{
			SetFloat((DWORD)this->m_Address.RotX, -45.0f);
		}
		else
		{
			SetFloat((DWORD)this->m_Address.RotX, ((*this->m_Address.RotX) - 6.0f));
		}
	}

	if (this->m_CursorY < lpMouse->pt.y)
	{
		if ((*this->m_Address.RotY) > 22.5f)
		{
			SetFloat((DWORD)this->m_Address.RotY, ((*this->m_Address.RotY) - 2.42f));

			SetFloat((DWORD)this->m_Address.PosZ, ((*this->m_Address.PosZ) - 44.0f));
		}
	}

	if (this->m_CursorY > lpMouse->pt.y)
	{
		if ((*this->m_Address.RotY) < 90.0f)
		{
			SetFloat((DWORD)this->m_Address.RotY, ((*this->m_Address.RotY) + 2.42f));

			SetFloat((DWORD)this->m_Address.PosZ, ((*this->m_Address.PosZ) + 44.0f));
		}
	}

	this->m_CursorX = lpMouse->pt.x;

	this->m_CursorY = lpMouse->pt.y;

	this->SetCurrentValue();
}

void CCamera3D::SetCurrentValue()
{
	SetFloat((DWORD)this->m_Address.ClipX[0], (1272 + (abs(*this->m_Address.PosZ - 150) * 3) + 1000));

	SetFloat((DWORD)this->m_Address.ClipX[1], (1272 + (abs(*this->m_Address.PosZ - 150) * 3) + 1000));

	SetFloat((DWORD)this->m_Address.ClipY[0], (-672 - (abs(*this->m_Address.PosZ - 150) * 3) - 3000));

	SetFloat((DWORD)this->m_Address.ClipY[1], (-672 - (abs(*this->m_Address.PosZ - 150) * 3) - 3000));

	SetFloat((DWORD)this->m_Address.ClipZ, (1190 + (abs(*this->m_Address.PosZ - 150) * 3) + 3000));

	SetFloat((DWORD)this->m_Address.ClipGL, (2000 + (abs(*this->m_Address.PosZ - 150) * 3) + 1500));
}

void CCamera3D::SetDefaultValue()
{
	if (this->m_Default.IsLoad)
	{
		SetFloat((DWORD)this->m_Address.Zoom, this->m_Default.Zoom);

		SetFloat((DWORD)this->m_Address.RotX, this->m_Default.RotX);

		SetFloat((DWORD)this->m_Address.RotY, this->m_Default.RotY);

		SetFloat((DWORD)this->m_Address.PosZ, this->m_Default.PosZ);

		SetFloat((DWORD)this->m_Address.ClipX[0], this->m_Default.ClipX[0]);

		SetFloat((DWORD)this->m_Address.ClipX[1], this->m_Default.ClipX[1]);

		SetFloat((DWORD)this->m_Address.ClipY[0], this->m_Default.ClipY[0]);

		SetFloat((DWORD)this->m_Address.ClipY[1], this->m_Default.ClipY[1]);

		SetFloat((DWORD)this->m_Address.ClipZ, this->m_Default.ClipZ);

		SetFloat((DWORD)this->m_Address.ClipGL, this->m_Default.ClipGL);
	}

}