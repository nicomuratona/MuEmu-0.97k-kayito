#include "stdafx.h"
#include "OpenglUtil.h"

void MyUpdateMousePosition()
{
	vec3_t vPos;

	glLoadIdentity();

	glTranslatef(-CameraPosition[0], -CameraPosition[1], -CameraPosition[2]);

	GetOpenGLMatrix(CameraMatrix);

	Vector(-CameraMatrix[0][3], -CameraMatrix[1][3], -CameraMatrix[2][3], vPos);

	VectorIRotate(vPos, CameraMatrix, MousePosition);
}

float DegToRad(float degrees)
{
	return degrees * (Q_PI / 180.0f);
}

float RadToDeg(float radians)
{
	return radians * (180.0f / Q_PI);
}

float CalculateVFOV(float hFOV, float aspectRatio)
{
	float hFOV_radians = DegToRad(hFOV);

	float vFOV_radians = 2.0f * atan(tan(hFOV_radians / 2.0f) / aspectRatio);

	return RadToDeg(vFOV_radians);
}

void MyRenderBitmapRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight)
{
	x = ConvertX(x);

	y = ConvertY(y);

	Width = ConvertX(Width);

	Height = ConvertY(Height);

	BindTexture(Texture);

	vec3_t p[4], p2[4];

	x += (Width * 0.5f);

	y += (Height * 0.5f);

	y = WindowHeight - y;

	Vector(-Width * 0.5f, Height * 0.5f, 0.0f, p[0]);

	Vector(-Width * 0.5f, -Height * 0.5f, 0.0f, p[1]);

	Vector(Width * 0.5f, -Height * 0.5f, 0.0f, p[2]);

	Vector(Width * 0.5f, Height * 0.5f, 0.0f, p[3]);

	vec3_t Angle;

	Vector(0.0f, 0.0f, Rotate, Angle);

	float Matrix[3][4];

	AngleMatrix(Angle, Matrix);

	float c[4][2];

	c[0][0] = u;
	c[0][1] = v;

	c[3][0] = u + uWidth;
	c[3][1] = v;

	c[2][0] = u + uWidth;
	c[2][1] = v + vHeight;

	c[1][0] = u;
	c[1][1] = v + vHeight;

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 4; i++)
	{
		glTexCoord2f(c[i][0], c[i][1]);

		VectorRotate(p[i], Matrix, p2[i]);

		glVertex2f(p2[i][0] + x, p2[i][1] + y);
	}

	glEnd();
}

void MyRenderBitRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight)
{
	x = ConvertX(x);
	y = ConvertY(y);

	Width = ConvertX(Width);
	Height = ConvertY(Height);

	y = Height - y;

	BindTexture(Texture);

	float cx = (Width / 2.0f) - (Width - x);
	float cy = (Height / 2.0f) - (Height - y);

	float ax = (-Width * 0.5f) + cx;
	float bx = (Width * 0.5f) + cx;

	float ay = (-Height * 0.5f) + cy;
	float by = (Height * 0.5f) + cy;

	vec3_t p[4];

	Vector(ax, by, 0.0f, p[0]);
	Vector(ax, ay, 0.0f, p[1]);
	Vector(bx, ay, 0.0f, p[2]);
	Vector(bx, by, 0.0f, p[3]);

	vec3_t Angle;

	Vector(0.0f, 0.0f, Rotate, Angle);

	float Matrix[3][4];

	AngleMatrix(Angle, Matrix);

	float c[4][2];

	c[0][0] = u;
	c[0][1] = v;

	c[3][0] = u + uWidth;
	c[3][1] = v;

	c[2][0] = u + uWidth;
	c[2][1] = v + vHeight;

	c[1][0] = u;
	c[1][1] = v + vHeight;

	vec3_t p2[4];

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 4; i++)
	{
		glTexCoord2f(c[i][0], c[i][1]);

		VectorRotate(p[i], Matrix, p2[i]);

		glVertex2f(p2[i][0] + (WindowWidth / 2.0f), p2[i][1] + (WindowHeight / 2.0f));
	}

	glEnd();
}

void MyRenderPointRotate(int Texture, float ix, float iy, float iWidth, float iHeight, float x, float y, float Width, float Height, float Rotate, float Rotate_Loc, float u, float v, float uWidth, float vHeight, char* Tooltip)
{
	ix = ConvertX(ix);
	iy = ConvertY(iy);

	x = ConvertX(x);
	y = ConvertY(y);

	Width = ConvertX(Width);
	Height = ConvertY(Height);

	y = Height - y;

	iy = Height - iy;

	BindTexture(Texture);

	vec3_t p;

	Vector((ix - (Width * 0.5f)) + ((Width / 2.0f) - (Width - x)), (iy - (Height * 0.5f)) + ((Height / 2.0f) - (Height - y)), 0.0f, p);

	vec3_t Angle;

	Vector(0.0f, 0.0f, Rotate, Angle);

	float Matrix[3][4];

	AngleMatrix(Angle, Matrix);

	vec3_t p3;

	VectorRotate(p, Matrix, p3);

	vec3_t p2[4];

	Vector(-(iWidth * 0.5f), (iHeight * 0.5f), 0.0f, p2[0]);
	Vector(-(iWidth * 0.5f), -(iHeight * 0.5f), 0.0f, p2[1]);
	Vector((iWidth * 0.5f), -(iHeight * 0.5f), 0.0f, p2[2]);
	Vector((iWidth * 0.5f), (iHeight * 0.5f), 0.0f, p2[3]);

	Vector(0.0f, 0.0f, Rotate_Loc, Angle);

	AngleMatrix(Angle, Matrix);

	float c[4][2];

	c[0][0] = u;
	c[0][1] = v;

	c[3][0] = u + uWidth;
	c[3][1] = v;

	c[2][0] = u + uWidth;
	c[2][1] = v + vHeight;

	c[1][0] = u;
	c[1][1] = v + vHeight;

	vec3_t p4[4];

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < 4; i++)
	{
		glTexCoord2f(c[i][0], c[i][1]);

		Matrix[0][3] = p3[0];

		Matrix[1][3] = p3[1];

		VectorTransform(p2[i], Matrix, p4[i]);

		glVertex2f(p4[i][0] + (WindowWidth / 2.0f), p4[i][1] + (WindowHeight / 2.0f));
	}

	glEnd();

	if (Tooltip)
	{
		float dx = ((p3[0] + (WindowWidth / 2.0f)) * (float)(640.0f / WindowWidth)) - (iWidth / 4.0f);
		float dy = ((p3[1] + (WindowHeight / 2.0f)) * (float)(480.0f / WindowHeight)) + (iHeight / 4.0f);

		if (IsWorkZone((int)(dx), (int)(480.0f - dy), (int)(iWidth / 2.0f), (int)(iHeight / 2.0f)))
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			SetBackgroundTextColor = Color4b(0, 0, 0, 255);
			SetTextColor = Color4b(0, 255, 255, 255);

			RenderTipText
			(
				CenterTextPosX(Tooltip, (int)(dx + (iWidth / 2.0f))),
				(int)(480.0f - dy - 5.0f),
				Tooltip
			);
		}

		glColor3f(1.0f, 1.0f, 1.0f);
	}
}

void MyCreateScreenVector(int sx, int sy, vec3_t Target)
{
	sx = sx * WindowWidth / 640;

	sy = sy * WindowHeight / 480;

	vec3_t p1, p2;

	float fixedDepth = 2000.0f;

	p1[0] = (float)(sx - ScreenCenterX) * fixedDepth * PerspectiveX;

	p1[1] = -(float)(sy - ScreenCenterY) * fixedDepth * PerspectiveY;

	p1[2] = -fixedDepth;

	p2[0] = -CameraMatrix[0][3];

	p2[1] = -CameraMatrix[1][3];

	p2[2] = -CameraMatrix[2][3];

	VectorIRotate(p2, CameraMatrix, MousePosition);

	VectorIRotate(p1, CameraMatrix, p2);

	VectorAdd(MousePosition, p2, Target);
}

void MyGluPerspective2(float Fov, float Aspect, float ZNear, float ZFar)
{
	gluPerspective(Fov, Aspect, 20.0f, 2000.0f);

	ScreenCenterX = OpenglWindowX + OpenglWindowWidth / 2;

	ScreenCenterY = OpenglWindowY + OpenglWindowHeight / 2;

	ScreenCenterYFlip = WindowWidth - ScreenCenterY;

	float AspectY = (float)(WindowHeight) / (float)(OpenglWindowHeight);

	PerspectiveX = tanf(Fov * 0.5f * 3.141592f / 180.0f) / (float)(OpenglWindowWidth / 2) * Aspect;

	PerspectiveY = tanf(Fov * 0.5f * 3.141592f / 180.0f) / (float)(OpenglWindowHeight / 2) * AspectY;
}

void RenderTriangleColor(float x, float y, float Width, float Height)
{
	DisableTexture(false);

	x = ConvertX(x);

	y = ConvertY(y);

	Width = ConvertX(Width);

	Height = ConvertY(Height);

	float p[3][2];

	y = WindowHeight - y;

	p[0][0] = x;

	p[0][1] = y;

	p[0][2] = x;

	p[1][1] = y - Height;

	p[2][0] = x + Width;

	p[2][1] = y - (Height * 0.5f);

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < 3; i++)
	{
		glVertex2f(p[i][0], p[i][1]);
	}

	glEnd();
}