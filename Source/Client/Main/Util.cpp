#include "stdafx.h"
#include "Util.h"

void SetByte(DWORD offset, BYTE value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 1, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset, 1, OldProtect, &OldProtect);
}

void SetWord(DWORD offset, WORD value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 2, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(WORD*)(offset) = value;

	VirtualProtect((void*)offset, 2, OldProtect, &OldProtect);
}

void SetDword(DWORD offset, DWORD value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(DWORD*)(offset) = value;

	VirtualProtect((void*)offset, 4, OldProtect, &OldProtect);
}

void SetFloat(DWORD offset, float value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 4, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(float*)(offset) = value;

	VirtualProtect((void*)offset, 4, OldProtect, &OldProtect);
}

void SetDouble(DWORD offset, double value)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 8, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(double*)(offset) = value;

	VirtualProtect((void*)offset, 8, OldProtect, &OldProtect);
}

void SetCompleteHook(BYTE head, DWORD offset, ...)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, 5, PAGE_EXECUTE_READWRITE, &OldProtect);

	if (head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset + 1;

	*(DWORD*)(offset + 1) = (*function) - (offset + 5);

	VirtualProtect((void*)offset, 5, OldProtect, &OldProtect);
}

void MemoryCpy(DWORD offset, void* value, DWORD size)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	memcpy((void*)offset, value, size);

	VirtualProtect((void*)offset, size, OldProtect, &OldProtect);
}

void MemorySet(DWORD offset, DWORD value, DWORD size)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	memset((void*)offset, value, size);

	VirtualProtect((void*)offset, size, OldProtect, &OldProtect);
}

void VirtualizeOffset(DWORD offset, DWORD size)
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, size, PAGE_EXECUTE_READWRITE, &OldProtect);

	DWORD HookAddr = (DWORD)malloc(size + 5);

	memcpy((void*)HookAddr, (void*)offset, size);

	*(BYTE*)(HookAddr + size) = 0xE9;

	*(DWORD*)(HookAddr + size + 1) = (offset + size) - ((HookAddr + size) + 5);

	*(BYTE*)(offset) = 0xE9;

	*(DWORD*)(offset + 1) = HookAddr - (offset + 5);

	memset((void*)(offset + 5), 0x90, (size - 5));

	VirtualProtect((void*)offset, size, OldProtect, &OldProtect);
}

void PacketArgumentEncrypt(BYTE* out_buff, BYTE* in_buff, int size)
{
	BYTE XorTable[3] = { 0xFC, 0xCF, 0xAB };

	for (int n = 0; n < size; n++)
	{
		out_buff[n] = in_buff[n] ^ XorTable[n % 3];
	}
}

char* ConvertModuleFileName(char* name)
{
	static char buff[MAX_PATH] = { 0 };

	for (int n = strlen(name); n > 0; n--)
	{
		if (name[n] == '\\')
		{
			strcpy_s(buff, sizeof(buff), &name[(n + 1)]);

			break;
		}
	}

	return buff;
}

bool FileExists(char* name)
{
	if (CreateFile(name, 0, 0, 0, OPEN_EXISTING, 0, 0) == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	return true;
};

int CenterTextPosX(char* buff, int PosX)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (PosX - (640 * sz.cx / WindowWidth >> 1));
}

int CenterTextPosY(char* buff, int PosY)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (PosY - (480 * sz.cy / WindowHeight >> 1));
}

float ImgCenterScreenPosX(float Size)
{
	return (320.0f - (Size / 2.0f));
}

float ImgCenterScreenPosY(float Size)
{
	return (216.0f - (Size / 2.0f));
}

bool IsWorkZone(int X, int Y, int Width, int Height)
{
	if ((MouseX >= X && MouseX <= (X + Width)) && (MouseY >= Y && MouseY <= (Y + Height)))
	{
		return true;
	}

	return false;
}

void Decrypt(BYTE* OutBuff, BYTE* InBuff, int size)
{
	BYTE byMapXorKey[16] = { 0xD1, 0x73, 0x52, 0xF6, 0xD2, 0x9A, 0xCB, 0x27, 0x3E, 0xAF, 0x59, 0x31, 0x37, 0xB3, 0xE7, 0xA2 };

	WORD wMapKey = 0x5E;

	for (int i = 0; i < size; ++i)
	{
		OutBuff[i] = (InBuff[i] ^ byMapXorKey[i % 16]) - (BYTE)wMapKey;

		wMapKey = InBuff[i] + 0x3D;

		wMapKey = wMapKey & 0xFF;
	}
}

void Encrypt(BYTE* OutBuff, BYTE* InBuff, int size)
{
	BYTE byMapXorKey[16] = { 0xD1, 0x73, 0x52, 0xF6, 0xD2, 0x9A, 0xCB, 0x27, 0x3E, 0xAF, 0x59, 0x31, 0x37, 0xB3, 0xE7, 0xA2 };

	WORD wMapKey = 0x5E;

	for (int i = 0; i < size; ++i)
	{
		OutBuff[i] = (InBuff[i] + (BYTE)wMapKey) ^ byMapXorKey[i % 16];

		wMapKey = InBuff[i] + 0x3D;

		wMapKey = wMapKey & 0xFF;
	}
}

void MyRenderBitmapRotate(int Texture, float x, float y, float Width, float Height, float Rotate, float u, float v, float uWidth, float vHeight)
{
	x = ConvertX(x);

	y = ConvertY(y);

	Width = ConvertX(Width);

	Height = ConvertY(Height);

	BindTexture(Texture);

	vec3_t p[4], p2[4];

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