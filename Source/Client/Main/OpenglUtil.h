#pragma once

void MyUpdateMousePosition();

float DegToRad(float degrees);

float RadToDeg(float radians);

float CalculateVFOV(float hFOV, float aspectRatio);

void MyRenderBitmapRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight);

void MyRenderBitRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight);

void MyRenderPointRotate(int Texture, float ix, float iy, float iWidth, float iHeight, float x, float y, float Width, float Height, float Rotate, float Rotate_Loc, float u, float v, float uWidth, float vHeight, char* Tooltip);

void MyCreateScreenVector(int sx, int sy, vec3_t Target);

void MyGluPerspective2(float Fov, float Aspect, float ZNear, float ZFar);

void RenderTriangleColor(float x, float y, float Width, float Height);