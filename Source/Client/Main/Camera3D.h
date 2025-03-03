#pragma once

struct CAMERA_LIMITS
{
	float ZoomMinLimit; // Zoom In Max
	float ZoomMaxLimit; // Zoom Out Max
	float ZoomMinPrecision; // Velocity per mouse scroll
	float ZoomMaxPrecision; // Max accumulated scroll velocity

	float HorizontalRotationLimit; // Horizontal rotation (360 view)
	float HorizontalRotationMinPrecision; // Velocity per mouse horizontal movement
	float HorizontalRotationMaxPrecision; // Max accumulated horizontal velocity

	float VerticalRotationUpperLimit; // Limit to camera upper view rotation
	float VerticalRotationLowerLimit; // Limit to camera lower view rotation
	float VerticalRotationMinPrecision; // Velocity per mouse vertical movement
	float VerticalRotationMaxPrecision; // Max accumulated vertical velocity
};

struct CURRENT_VELOCITY
{
	float Zoom;
	float Horizontal;
	float Vertical;
	float Reduction;
};

struct CAMERA_INFO
{
	float ZoomDistance;
	float HorizontalRotation;
	float VerticalRotation;
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

        static bool MoveMainCamera();

	static void CreateFrustum(float Aspect, vec3_t Position);

	void CreateFrustum2D(vec3_t Position);

	static void MyRenderNumber(vec3_t Position, int Num, vec3_t Color, float Alpha, float Scale);

private:

	bool m_Enable;

	bool m_IsMove;

	LONG m_CursorX;

	LONG m_CursorY;

	CAMERA_LIMITS m_Limits;

	CURRENT_VELOCITY m_Velocity;

	CAMERA_INFO m_Default;

	CAMERA_INFO m_Camera;
};

extern CCamera3D Camera3D;