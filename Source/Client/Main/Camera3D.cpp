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

	// Zoom Limits
	this->m_Limits.ZoomMinLimit = 250.0f;
	this->m_Limits.ZoomMaxLimit = 2000.0f;
	this->m_Limits.ZoomMinPrecision = 10.0f;
	this->m_Limits.ZoomMaxPrecision = 100.0f;

	// Horizontal Rotation Limits
	this->m_Limits.HorizontalRotationLimit = 360.0f;
	this->m_Limits.HorizontalRotationMinPrecision = 0.1f;
	this->m_Limits.HorizontalRotationMaxPrecision = 5.0f;

	// Vertical Rotation Limits
	this->m_Limits.VerticalRotationUpperLimit = -1.0f;
	this->m_Limits.VerticalRotationLowerLimit = -85.0f;

	this->m_Limits.VerticalRotationMinPrecision = 0.1f;
	this->m_Limits.VerticalRotationMaxPrecision = 1.5f;

	// Velocity Values for Movements
	this->m_Velocity.Zoom = 30.0f;
	this->m_Velocity.Horizontal = 0.0f;
	this->m_Velocity.Vertical = 0.2f;
	this->m_Velocity.Reduction = 0.95f;

	// Default Camera Values
	this->m_Default.ZoomDistance = 300.0f;
	this->m_Default.HorizontalRotation = -45.0f;
	this->m_Default.VerticalRotation = -48.5f;

	// Initial Camera Values
	this->m_Camera.ZoomDistance = 300.0f;
	this->m_Camera.HorizontalRotation = -45.0f;
	this->m_Camera.VerticalRotation = -48.5f;

	this->Init();
}

CCamera3D::~CCamera3D()
{

}

void CCamera3D::Init()
{
	SetCompleteHook(0xE8, 0x00525ADE, &this->MoveMainCamera);

	SetCompleteHook(0xE9, 0x004F9050, &this->CreateFrustum);

	SetCompleteHook(0xE9, 0x005120C0, &this->MyRenderNumber);
}

bool CCamera3D::MoveMainCamera()
{
	CameraFOV = CalculateVFOV(90.0f, (WindowWidth) / ((float)WindowHeight));

	// Zoom
	{
		Camera3D.m_Camera.ZoomDistance += Camera3D.m_Velocity.Zoom;

		Camera3D.m_Camera.ZoomDistance = Clamp(Camera3D.m_Camera.ZoomDistance, Camera3D.m_Limits.ZoomMinLimit, Camera3D.m_Limits.ZoomMaxLimit);

		Camera3D.m_Velocity.Zoom *= Camera3D.m_Velocity.Reduction;

		if (fabs(Camera3D.m_Velocity.Zoom) < 0.01f)
		{
			Camera3D.m_Velocity.Zoom = 0.0f;
		}

		CameraDistance = Camera3D.m_Camera.ZoomDistance;
	}

	// Rotación Horizontal
	{
		Camera3D.m_Camera.HorizontalRotation += Camera3D.m_Velocity.Horizontal;

		Camera3D.m_Camera.HorizontalRotation = fmodf(Camera3D.m_Camera.HorizontalRotation, Camera3D.m_Limits.HorizontalRotationLimit);

		Camera3D.m_Velocity.Horizontal *= Camera3D.m_Velocity.Reduction;

		if (fabs(Camera3D.m_Velocity.Horizontal) < 0.01f)
		{
			Camera3D.m_Velocity.Horizontal = 0.0f;
		}

		CameraAngle[2] = Camera3D.m_Camera.HorizontalRotation;
	}

	// Rotación Vertical
	{
		Camera3D.m_Camera.VerticalRotation += Camera3D.m_Velocity.Vertical;

		Camera3D.m_Camera.VerticalRotation = Clamp(Camera3D.m_Camera.VerticalRotation, Camera3D.m_Limits.VerticalRotationLowerLimit, Camera3D.m_Limits.VerticalRotationUpperLimit);

		Camera3D.m_Velocity.Vertical *= Camera3D.m_Velocity.Reduction;

		if (fabs(Camera3D.m_Velocity.Vertical) < 0.01f)
		{
			Camera3D.m_Velocity.Vertical = 0.0f;
		}

		CameraAngle[0] = Camera3D.m_Camera.VerticalRotation;
	}

	// ViewFar
	{
		float normalizedPitch = 1.0f - ((CameraAngle[0] - Camera3D.m_Limits.VerticalRotationLowerLimit) / (Camera3D.m_Limits.VerticalRotationUpperLimit - Camera3D.m_Limits.VerticalRotationLowerLimit));

		CameraViewFar = 1000.0f + CameraDistance * 2.0f + normalizedPitch * 4000.0f;
	}

	// ViewNear
	{
		CameraViewNear = fmaxf(CameraDistance * 0.1f, 0.1f);
	}

	float Matrix[3][4];
	AngleMatrix(CameraAngle, Matrix);
	Matrix[1][2] *= -1.0f;

	vec3_t Position = { 0.0f, 0.0f, CameraDistance };

	vec3_t PositionTransform;
	VectorRotate(Position, Matrix, PositionTransform);

	vec3_t TargetPosition = { PlayerPosition[0], PlayerPosition[1], PlayerPosition[2] + 50.0f };

	VectorAdd(TargetPosition, PositionTransform, CameraPosition);

	CameraPosition[2] += (EarthQuake * 100.0f);

	if (gMapManager.GetMapMovement(World))
	{
		CameraPosition[0] += ((sinf(WorldTime * 0.0005f) * 2.0f) * 30.0f);

		CameraPosition[1] -= ((sinf(WorldTime * 0.0008f) * 2.5f) * 30.0f);
	}

	return false;
}

void CCamera3D::CreateFrustum(float Aspect, vec3_t Position)
{
	float Distance = CameraViewFar * 0.9f;

	Aspect = (float)WindowWidth / 640.0f;

	float Width = tanf(DegToRad(CameraFOV * 0.5f)) * Distance * Aspect + 100.0f;

	float Height = Width * 3.0f / 4.0f;

	vec3_t Temp[5];

	Vector(0.0f, 0.0f, 0.0f, Temp[0]);

	Vector(-Width, Height, -Distance, Temp[1]);

	Vector(Width, Height, -Distance, Temp[2]);

	Vector(Width, -Height, -Distance, Temp[3]);

	Vector(-Width, -Height, -Distance, Temp[4]);

	float FrustrumMinX = (float)TERRAIN_SIZE * TERRAIN_SCALE;

	float FrustrumMinY = (float)TERRAIN_SIZE * TERRAIN_SCALE;

	float FrustrumMaxX = 0.0f;

	float FrustrumMaxY = 0.0f;

	float Matrix[3][4];

	GetOpenGLMatrix(Matrix);

	for (int i = 0; i < 5; i++)
	{
		vec3_t t;

		VectorIRotate(Temp[i], Matrix, t);

		VectorAdd(t, CameraPosition, FrustrumVertex[i]);

		if (FrustrumMinX > FrustrumVertex[i][0])
		{
			FrustrumMinX = FrustrumVertex[i][0];
		}

		if (FrustrumMinY > FrustrumVertex[i][1])
		{
			FrustrumMinY = FrustrumVertex[i][1];
		}

		if (FrustrumMaxX < FrustrumVertex[i][0])
		{
			FrustrumMaxX = FrustrumVertex[i][0];
		}

		if (FrustrumMaxY < FrustrumVertex[i][1])
		{
			FrustrumMaxY = FrustrumVertex[i][1];
		}
	}

	int tileWidth = 4;

	FrustrumBoundMinX_1 = (int)(FrustrumMinX / TERRAIN_SCALE) / tileWidth * tileWidth - tileWidth;
	FrustrumBoundMinY_1 = (int)(FrustrumMinY / TERRAIN_SCALE) / tileWidth * tileWidth - tileWidth;
	FrustrumBoundMaxX_1 = (int)(FrustrumMaxX / TERRAIN_SCALE) / tileWidth * tileWidth + tileWidth;
	FrustrumBoundMaxY_1 = (int)(FrustrumMaxY / TERRAIN_SCALE) / tileWidth * tileWidth + tileWidth;

	if (FrustrumBoundMinX_1 < 0)
	{
		FrustrumBoundMinX_1 = 0;
	}
	if (FrustrumBoundMinX_1 > TERRAIN_SIZE_MASK - tileWidth)
	{
		FrustrumBoundMinX_1 = TERRAIN_SIZE_MASK - tileWidth;
	}

	if (FrustrumBoundMinY_1 < 0)
	{
		FrustrumBoundMinY_1 = 0;
	}
	if (FrustrumBoundMinY_1 > TERRAIN_SIZE_MASK - tileWidth)
	{
		FrustrumBoundMinY_1 = TERRAIN_SIZE_MASK - tileWidth;
	}

	if (FrustrumBoundMaxX_1 < 0)
	{
		FrustrumBoundMaxX_1 = 0;
	}
	if (FrustrumBoundMaxX_1 > TERRAIN_SIZE_MASK - tileWidth)
	{
		FrustrumBoundMaxX_1 = TERRAIN_SIZE_MASK - tileWidth;
	}

	if (FrustrumBoundMaxY_1 < 0)
	{
		FrustrumBoundMaxY_1 = 0;
	}
	if (FrustrumBoundMaxY_1 > TERRAIN_SIZE_MASK - tileWidth)
	{
		FrustrumBoundMaxY_1 = TERRAIN_SIZE_MASK - tileWidth;
	}

	FaceNormalize(FrustrumVertex[0], FrustrumVertex[1], FrustrumVertex[2], FrustrumFaceNormal[0]);

	FaceNormalize(FrustrumVertex[0], FrustrumVertex[2], FrustrumVertex[3], FrustrumFaceNormal[1]);

	FaceNormalize(FrustrumVertex[0], FrustrumVertex[3], FrustrumVertex[4], FrustrumFaceNormal[2]);

	FaceNormalize(FrustrumVertex[0], FrustrumVertex[4], FrustrumVertex[1], FrustrumFaceNormal[3]);

	FaceNormalize(FrustrumVertex[3], FrustrumVertex[2], FrustrumVertex[1], FrustrumFaceNormal[4]);

	FrustrumFaceD[0] = -DotProduct(FrustrumVertex[0], FrustrumFaceNormal[0]);

	FrustrumFaceD[1] = -DotProduct(FrustrumVertex[0], FrustrumFaceNormal[1]);

	FrustrumFaceD[2] = -DotProduct(FrustrumVertex[0], FrustrumFaceNormal[2]);

	FrustrumFaceD[3] = -DotProduct(FrustrumVertex[0], FrustrumFaceNormal[3]);

	FrustrumFaceD[4] = -DotProduct(FrustrumVertex[1], FrustrumFaceNormal[4]);

	Camera3D.CreateFrustum2D(Position);
}

void CCamera3D::CreateFrustum2D(vec3_t Position)
{
	float divisor = (this->m_Limits.VerticalRotationLowerLimit - this->m_Limits.VerticalRotationUpperLimit) * 0.5f;

	float pitch = (CameraAngle[0] - this->m_Limits.VerticalRotationUpperLimit - divisor);

	float normalizedPitch = fabs(pitch / divisor);

	float fWidthNear = (2.0f * CameraViewNear * tanf(DegToRad(CameraFOV * 0.5f))) + normalizedPitch * 2000.0f;

	float fWidthFar = (2.0f * CameraViewFar * tanf(DegToRad(CameraFOV * 0.5f)));

	float fCameraViewTarget = 500.0f + CameraDistance;

	vec3_t p[4];
	Vector(-fWidthFar, CameraViewFar - fCameraViewTarget, 0.0f, p[0]);
	Vector(fWidthFar, CameraViewFar - fCameraViewTarget, 0.0f, p[1]);
	Vector(fWidthNear, CameraViewNear - fCameraViewTarget, 0.0f, p[2]);
	Vector(-fWidthNear, CameraViewNear - fCameraViewTarget, 0.0f, p[3]);

	vec3_t Angle;
	Vector(0.0f, 0.0f, -CameraAngle[2], Angle);

	float Matrix[3][4];
	AngleMatrix(Angle, Matrix);

	vec3_t Frustum[4];

	for (int i = 0; i < 4; i++)
	{
		VectorRotate(p[i], Matrix, Frustum[i]);

		VectorAdd(Frustum[i], CameraPosition, Frustum[i]);

		FrustrumX[i] = Frustum[i][0] * 0.01f;

		FrustrumY[i] = Frustum[i][1] * 0.01f;
	}
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
	float spacing = Scale * (float)sqrt(2) * 0.5f;

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

	short wheelDelta = GET_WHEEL_DELTA_WPARAM(lpMouse->mouseData);

	if (wheelDelta < 0)
	{
		this->m_Velocity.Zoom += this->m_Limits.ZoomMinPrecision;

		if (this->m_Velocity.Zoom > this->m_Limits.ZoomMaxPrecision)
		{
			this->m_Velocity.Zoom = this->m_Limits.ZoomMaxPrecision;
		}
	}

	if (wheelDelta > 0)
	{
		this->m_Velocity.Zoom -= this->m_Limits.ZoomMinPrecision;

		if (this->m_Velocity.Zoom < -this->m_Limits.ZoomMaxPrecision)
		{
			this->m_Velocity.Zoom = -this->m_Limits.ZoomMaxPrecision;
		}
	}
}

void CCamera3D::Move(MOUSEHOOKSTRUCTEX* lpMouse)
{
	if (!this->m_Enable || !this->m_IsMove || SceneFlag != MAIN_SCENE)
	{
		return;
	}

	float deltaX = (float)(lpMouse->pt.x - this->m_CursorX);

	float deltaY = (float)(lpMouse->pt.y - this->m_CursorY);

	this->m_Velocity.Horizontal += deltaX * this->m_Limits.HorizontalRotationMinPrecision;

	this->m_Velocity.Vertical += deltaY * this->m_Limits.VerticalRotationMinPrecision;

	if (this->m_Velocity.Horizontal > this->m_Limits.HorizontalRotationMaxPrecision)
	{
		this->m_Velocity.Horizontal = this->m_Limits.HorizontalRotationMaxPrecision;
	}
	if (this->m_Velocity.Horizontal < -this->m_Limits.HorizontalRotationMaxPrecision)
	{
		this->m_Velocity.Horizontal = -this->m_Limits.HorizontalRotationMaxPrecision;
	}

	if (this->m_Velocity.Vertical > this->m_Limits.VerticalRotationMaxPrecision)
	{
		this->m_Velocity.Vertical = this->m_Limits.VerticalRotationMaxPrecision;
	}
	if (this->m_Velocity.Vertical < -this->m_Limits.VerticalRotationMaxPrecision)
	{
		this->m_Velocity.Vertical = -this->m_Limits.VerticalRotationMaxPrecision;
	}

	this->m_CursorX = lpMouse->pt.x;

	this->m_CursorY = lpMouse->pt.y;
}

void CCamera3D::SetDefaultValue()
{
	this->m_Camera = this->m_Default;

	this->m_Velocity.Zoom = 30.0f;

	this->m_Velocity.Horizontal = 0.0f;

	this->m_Velocity.Vertical = 0.2f;
}