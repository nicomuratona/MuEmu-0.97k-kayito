#include "stdafx.h"
#include "Patchs.h"
#include "Protect.h"
#include "Protocol.h"

CPatchs gPatchs;

CPatchs::CPatchs()
{

}

CPatchs::~CPatchs()
{

}

void CPatchs::Init()
{
	SetByte(0x00558EA8, 0xA0); // Accent

	SetByte(0x00406F36, 0xEB); // Crack return

	SetByte(0x00406F5F, 0xEB); // Crack error messagebox

	SetByte(0x00406F9B, 0xEB); // Crack error messagebox

	SetByte(0x0041ECB5, 0xEB); // Crack mu.exe

	SetByte(0x0041ED25, 0xEB); // Crack config.ini

	SetByte(0x0041ED5E, 0xEB); // Crack config.ini

	SetByte(0x0041EFB5, 0xEB); // Crack gg init

	SetByte(0x0053D90B, 0xEB); // Crack gameguard

	SetByte(0x0055961C, (gProtect.m_MainInfo.ClientVersion[0] + 1)); // Version

	SetByte(0x0055961D, (gProtect.m_MainInfo.ClientVersion[2] + 2)); // Version

	SetByte(0x0055961E, (gProtect.m_MainInfo.ClientVersion[3] + 3)); // Version

	SetByte(0x0055961F, (gProtect.m_MainInfo.ClientVersion[5] + 4)); // Version

	SetByte(0x00559620, (gProtect.m_MainInfo.ClientVersion[6] + 5)); // Version

	SetWord(0x005615BC, (gProtect.m_MainInfo.IpAddressPort)); // IpAddressPort

	SetDword(0x005267B2, (DWORD)gProtect.m_MainInfo.ScreenShotPath); //Screenshot

	MemoryCpy(0x00558ED8, gProtect.m_MainInfo.IpAddress, sizeof(gProtect.m_MainInfo.IpAddress)); // IpAddress

	MemoryCpy(0x00559624, gProtect.m_MainInfo.ClientSerial, sizeof(gProtect.m_MainInfo.ClientSerial)); // ClientSerial

	MemorySet(0x004127B0, 0x90, 5); // Remove MuError.log

	SetCompleteHook(0xE9, 0x00524146, 0x00524231); // Remove Select Character Texts

	SetCompleteHook(0xE9, 0x0052292E, 0x00522953); // Ignore Character Level to Create MG
	SetCompleteHook(0xE9, 0x00524293, 0x005242B8); // Ignore Character Level to Create MG

	SetWord(0x00444B76, 0x19EB); //-> Uniria & Dinorant Reflect

	SetCompleteHook(0xE9, 0x00526A5A, &this->ReduceCPU);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)this->ReduceRam, 0, 0, 0);

	// Fix move cursor NPC
	SetByte(0x00430B9F, 0x90);
	MemorySet(0x00430BBD, 0x90, 0x7);

	// Fix move cursor TRADE
	SetByte(0x004335FC, 0x90);
	SetByte(0x00433621, 0x90);
	MemorySet(0x00433635, 0x90, 0x6);

	// Fix move cursor
	SetByte(0x00492EBD, 0x90);
	MemorySet(0x00492EDB, 0x90, 0x7);

	// Fix Shop NPC Closing send to server
	SetCompleteHook(0xE9, 0x004CBB15, &this->FixShopNpcClose);

	// Fix Drop Zen
	MemorySet(0x0042F2DF, 0x90, 0x7);

	// Fix Trade Zen Over 50000000
	SetByte(0x00515BF7, 0xEB);

	SetCompleteHook(0xE8, 0x0042B33D, &this->IgnoreRandomStuck);

	SetCompleteHook(0xE9, 0x00483AC5, &this->FixChasingAttackMovement);

	// Decrypt BMD
	SetCompleteHook(0xE9, 0x004424B4, &this->DecBMD);

	// EncTerrain%d.map
	SetByte(0x0050E5F9, 0xEB);
	SetDword(0x0050E623 + 1, (DWORD)"Data\\%s\\EncTerrain%d.map");
	// EncTerrain%d.att
	SetDword(0x0050E648 + 1, (DWORD)"Data\\%s\\EncTerrain%d.att");
	// EncTerrain%d.obj
	SetByte(0x0050E663, 0xEB);
	SetDword(0x0050E68D + 1, (DWORD)"Data\\%s\\EncTerrain%d.obj");

	// Decrypt MAP
	SetCompleteHook(0xE8, 0x0050E636, &this->OpenTerrainMapping);

	// Decrypt ATT
	SetCompleteHook(0xE8, 0x0050E658, &this->OpenTerrainAttribute);

	// Decrypt OBJ
	SetCompleteHook(0xE8, 0x0050E6A0, &this->OpenObjectsEnc);
}

__declspec(naked) void CPatchs::ReduceCPU()
{
	static DWORD JmpBack = 0x00526A60;

	__asm
	{
		Push 1;

		Call Dword Ptr Ds : [0x00552128] ; // Sleep

		Call Dword Ptr Ds : [0x00552198] ; // GetTickCount

		Jmp[JmpBack];
	}
}

void CPatchs::ReduceRam(LPVOID lpThreadParameter)
{
	HANDLE v1;

	HANDLE v2;

	while (TRUE)
	{
		Sleep(5000);

		v1 = GetCurrentProcess();

		SetProcessWorkingSetSize(v1, 0xFFFFFFFF, 0xFFFFFFFF);

		v2 = GetCurrentProcess();

		SetThreadPriority(v2, -2);
	}
}

__declspec(naked) void CPatchs::FixShopNpcClose()
{
	static DWORD jmpBack = 0x004CBB1A;
	static BYTE InterfaceID;

	_asm
	{
		Pushad;
		Mov Bl, Byte Ptr Ds : [Eax] ;
		Mov InterfaceID, Bl;
	}

	if (InterfaceID == 0xFE) // Case Shop
	{
		static PBMSG_HEAD pMsg;

		pMsg.set(0x31, sizeof(pMsg));

		gProtocol.DataSend((BYTE*)&pMsg, pMsg.size);
	}

	_asm
	{
		Popad;
		Mov Bl, [Eax + 1];
		Inc Bl;
		Jmp jmpBack;
	}
}

void CPatchs::IgnoreRandomStuck(DWORD c, DWORD Damage)
{
	if (*(WORD*)(c + 2) != 390)
	{
		SetPlayerShock(c, Damage);
	}
}

_declspec(naked) void CPatchs::FixChasingAttackMovement()
{
	static DWORD jmpBack = 0x00483ACC;
	static DWORD SendMove = 0x00491C40;

	_asm
	{
		Mov Byte Ptr[Ebp + 0x2ED], 5;
		Push Ebp;
		Push Ebp;
		Call[SendMove];
		Add Esp, 0x8;
		Jmp jmpBack;
	}
}

_declspec(naked) void CPatchs::DecBMD()
{
	static DWORD jmpBack = 0x004424BA;
	static int DataSize;
	static BYTE* Data;

	_asm
	{
		Mov DataSize, Edi;
		Mov Data, Ebx;
		Pushad;
	}

	if (Data[3] == 0xC)
	{
		Decrypt(&Data[4], &Data[8], *((long*)(Data + 4)));
	}

	_asm
	{
		Popad;
		Mov Cl, [Ebx + 3];
		Lea Esi, [Ebx + 4];
		Jmp jmpBack;
	}
}

int CPatchs::OpenTerrainMapping(char* FileName)
{
	InitTerrainMappingLayer();

	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		return -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, 1, EncBytes, fp);

	fclose(fp);

	BYTE* Data = new BYTE[EncBytes];

	Decrypt(Data, EncData, EncBytes);

	delete[] EncData;

	int DataPtr = 0;

	BYTE Version = Data[DataPtr];

	DataPtr += 1;

	int iMapNumber = Data[DataPtr];

	DataPtr += 1;

	memcpy(TerrainMappingLayer1, Data + DataPtr, 256 * 256);

	DataPtr += 256 * 256;

	memcpy(TerrainMappingLayer2, Data + DataPtr, 256 * 256);

	DataPtr += 256 * 256;

	for (int i = 0; i < 256 * 256; i++)
	{
		BYTE Alpha;

		Alpha = *((BYTE*)(Data + DataPtr));

		DataPtr += 1;

		TerrainMappingAlpha[i] = (float)Alpha / 255.0f;
	}

	delete[] Data;

	fclose(fp);

	return iMapNumber;
}

int CPatchs::OpenTerrainAttribute(char* FileName)
{
	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		char Text[256];

		sprintf_s(Text, "%s file not found.", FileName);

		MessageBox(g_hWnd, Text, NULL, MB_OK);

		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

		return  -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, EncBytes, 1, fp);

	BYTE* byBuffer = new BYTE[EncBytes];

	Decrypt(byBuffer, EncData, EncBytes);

	delete[] EncData;

	if (EncBytes != 131076 && EncBytes != 65540)
	{
		delete[] byBuffer;

		return -1;
	}

	bool extAtt = (EncBytes == 131076);

	PacketArgumentEncrypt(byBuffer, byBuffer, EncBytes);

	BYTE Version = byBuffer[0];

	int iMap = byBuffer[1];

	BYTE Width = byBuffer[2];

	BYTE Height = byBuffer[3];

	if (extAtt == false)
	{
		memcpy(TerrainWall, &byBuffer[4], 256 * 256);
	}
	else
	{
		BYTE TWall[256 * 256 * sizeof(WORD)];

		memcpy(TWall, &byBuffer[4], 256 * 256 * sizeof(WORD));

		for (int i = 0; i < 256 * 256; ++i)
		{
			TerrainWall[i] = TWall[i * 2];
		}
	}

	delete[] byBuffer;

	bool Error = false;

	if (Version != 0 || Width != 255 || Height != 255)
	{
		Error = true;
	}

	switch (World)
	{
		case MAP_LORENCIA:
		{
			if (TerrainWall[123 * 256 + 135] != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_DUNGEON:
		{
			if (TerrainWall[120 * 256 + 227] != 4)
			{
				Error = true;
			}

			break;
		}

		case MAP_DEVIAS:
		{
			if (TerrainWall[55 * 256 + 208] != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_NORIA:
		{
			if (TerrainWall[119 * 256 + 186] != 5)
			{
				Error = true;
			}

			break;
		}

		case MAP_LOST_TOWER:
		{
			if (TerrainWall[75 * 256 + 193] != 5)
			{
				Error = true;
			}

			break;
		}
	}

	for (int i = 0; i < 256 * 256; i++)
	{
		if (TerrainWall[i] >= 128)
		{
			Error = true;
		}
	}

	if (Error)
	{
		ExitProgram();

		return -1;
	}

	fclose(fp);

	return iMap;
}

int CPatchs::OpenObjectsEnc(char* FileName)
{
	FILE* fp;

	if (fopen_s(&fp, FileName, "rb") != 0)
	{
		char Text[256];

		sprintf_s(Text, "%s file not found.", FileName);

		MessageBox(g_hWnd, Text, NULL, MB_OK);

		SendMessage(g_hWnd, WM_DESTROY, 0, 0);

		return -1;
	}

	fseek(fp, 0, SEEK_END);

	int EncBytes = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	BYTE* EncData = new BYTE[EncBytes];

	fread(EncData, 1, EncBytes, fp);

	fclose(fp);

	BYTE* Data = new BYTE[EncBytes];

	Decrypt(Data, EncData, EncBytes);

	delete[] EncData;

	int DataPtr = 0;

	BYTE Version = Data[DataPtr];

	DataPtr += 1;

	int iMapNumber = Data[DataPtr];

	DataPtr += 1;

	short Count = *((short*)(Data + DataPtr));

	DataPtr += 2;

	for (int i = 0; i < Count; i++)
	{
		float Position[3];

		float Angle[3];

		short Type = *((short*)(Data + DataPtr));

		DataPtr += 2;

		memcpy(Position, Data + DataPtr, sizeof(float) * 3);

		DataPtr += sizeof(float) * 3;

		memcpy(Angle, Data + DataPtr, sizeof(float) * 3);

		DataPtr += sizeof(float) * 3;

		float Scale = *((float*)(Data + DataPtr));

		DataPtr += 4;

		CreateObject(Type, Position, Angle, Scale);
	}

	delete[] Data;

	fclose(fp);

	return iMapNumber;
}