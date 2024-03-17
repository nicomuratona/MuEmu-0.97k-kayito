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

void ConsoleProtocolLog(int type, BYTE* lpMsg, int size)
{
	BYTE head, subhead;

	BYTE header = lpMsg[0];

	if (header == 0xC1 || header == 0xC3)
	{
		head = lpMsg[2];
	}
	else if (header == 0xC2 || header == 0xC4)
	{
		head = lpMsg[3];
	}

	subhead = ((header == 0xC1) ? lpMsg[3] : lpMsg[4]);

	gConsole.Write("[%s] Header: 0x%02X, Head: 0x%02X, SubHead: 0x%02X, Size: %d", gConsole.GetOutputString(type), header, head, subhead, size);
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
}

int GetTextWidth(char* buff)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (640 * sz.cx / WindowWidth);
}

int GetTextHeight(char* buff)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (480 * sz.cy / WindowHeight);
}

int CenterTextPosX(char* buff, int PosX)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (PosX - (((640 * sz.cx) / WindowWidth) >> 1));
}

int CenterTextPosY(char* buff, int PosY)
{
	SIZE sz;

	GetTextExtentPoint(m_hFontDC, buff, strlen(buff), &sz);

	return (PosY - (((480 * sz.cy) / WindowHeight) >> 1));
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

void ConvertGold(double dGold, char* szText, int iDecimals)
{
	char szTemp[256];

	int iCipherCnt = 0;

	DWORD dwValueTemp = (DWORD)dGold;

	// integer digits
	while (dwValueTemp / 1000 > 0)
	{
		iCipherCnt = iCipherCnt + 3;

		dwValueTemp = dwValueTemp / 1000;
	}

	sprintf_s(szTemp, "%d", dwValueTemp);

	std::string sText = szTemp;

	while (iCipherCnt > 0)
	{
		dwValueTemp = (DWORD)dGold;

		dwValueTemp = (dwValueTemp % (int)pow(10.0f, (float)iCipherCnt)) / (int)pow(10.0f, (float)(iCipherCnt - 3));

		sprintf_s(szTemp, ",%03d", dwValueTemp);

		sText += szTemp;

		iCipherCnt = iCipherCnt - 3;
	}

	// place after decimal point
	if (iDecimals > 0)
	{
		dwValueTemp = (int)(dGold * pow(10.0f, (float)iDecimals)) % (int)pow(10.0f, (float)iDecimals);

		sprintf_s(szTemp, ".%d", dwValueTemp);

		sText += szTemp;
	}

	wsprintf(szText, sText.c_str());
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

BYTE GetDestValue(int xPos, int yPos, int xDst, int yDst)
{
	int DestX = xDst - xPos;

	int DestY = yDst - yPos;

	if (DestX < -8)
	{
		DestX = -8;
	}

	if (DestX > 7)
	{
		DestX = 7;
	}

	if (DestY < -8)
	{
		DestY = -8;
	}

	if (DestY > 7)
	{
		DestY = 7;
	}

	BYTE byValue1 = ((BYTE)(DestX + 8)) << 4;

	BYTE byValue2 = ((BYTE)(DestY + 8)) & 0xF;

	return (byValue1 | byValue2);
}

void GetNearRandomPos(vec3_t Pos, int iRange, vec3_t NewPos)
{
	VectorCopy(Pos, NewPos);

	NewPos[0] += (float)(rand() % (iRange * 2 + 1) - iRange);

	NewPos[1] += (float)(rand() % (iRange * 2 + 1) - iRange);

	NewPos[2] += (float)(rand() % (iRange * 2 + 1) - iRange);
}