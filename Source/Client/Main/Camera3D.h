#pragma once

struct CAMERA_ZOOM
{
	float MinPercent;
	float MaxPercent;
	float MinLimit;
	float MaxLimit;
	float Precision;
};

struct CAMERA_ADDR
{
	float* Zoom;
	float* RotX;
	float* RotY;
	float* PosZ;
	float* ClipX[2];
	float* ClipY[2];
	float* ClipZ;
	float* ClipGL;
};

struct CAMERA_INFO
{
	int IsLoad;
	float Zoom;
	float RotX;
	float RotY;
	float PosZ;
	float ClipX[2];
	float ClipY[2];
	float ClipZ;
	float ClipGL;
};


class CCamera3D
{
public:

        CCamera3D();

        virtual ~CCamera3D();

	void Toggle();

	void Restore();

	void SetIsMove(bool IsMove);

	void SetCursorX(LONG CursorX);

	void SetCursorY(LONG CursorY);

	void Zoom(MOUSEHOOKSTRUCTEX* lpMouse);

	void Move(MOUSEHOOKSTRUCTEX* lpMouse);

	void SetDefaultValue();

private:

        void Init();

	void SetCurrentValue();

	static void MyRenderNumber(vec3_t Position, int Num, vec3_t Color, float Alpha, float Scale);

private:

	bool m_Enable;

	bool m_IsMove;

	LONG m_CursorX;

	LONG m_CursorY;

	CAMERA_ZOOM m_Zoom;

	CAMERA_ADDR m_Address;

	CAMERA_INFO m_Default;
};

extern CCamera3D Camera3D;