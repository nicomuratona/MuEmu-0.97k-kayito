#pragma once

void SetByte(DWORD offset, BYTE value);

void SetWord(DWORD offset, WORD value);

void SetDword(DWORD offset, DWORD value);

void SetFloat(DWORD offset, float value);

void SetDouble(DWORD offset, double value);

void SetCompleteHook(BYTE head, DWORD offset, ...);

void MemoryCpy(DWORD offset, void* value, DWORD size);

void MemorySet(DWORD offset, DWORD value, DWORD size);

void VirtualizeOffset(DWORD offset, DWORD size);

void PacketArgumentEncrypt(BYTE* out_buff, BYTE* in_buff, int size);

char* ConvertModuleFileName(char* name);

bool FileExists(char* name);

int GetTextWidth(char* buff);

int GetTextHeight(char* buff);

int CenterTextPosX(char* buff, int PosX);

int CenterTextPosY(char* buff, int PosY);

float ImgCenterScreenPosX(float Size);

float ImgCenterScreenPosY(float Size);

bool IsWorkZone(int X, int Y, int Width, int Height);

void Decrypt(BYTE* OutBuff, BYTE* InBuff, int size);

void Encrypt(BYTE* OutBuff, BYTE* InBuff, int size);

void MyRenderBitmapRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight);

void RenderTriangleColor(float x, float y, float Width, float Height);