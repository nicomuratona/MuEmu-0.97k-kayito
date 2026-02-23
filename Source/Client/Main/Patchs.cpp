#include "stdafx.h"
#include "Patchs.h"
#include "LoadModels.h"
#include "PrintPlayer.h"
#include "Protect.h"
#include "Protocol.h"

CPatchs gPatchs;

CPatchs::CPatchs()
{
	GetPrivateProfileString("User", "Username", "", m_ID, 10, ".\\Config.ini");
}

CPatchs::~CPatchs()
{
	WritePrivateProfileString("User", "Username", m_ID, ".\\Config.ini");
}

void CPatchs::Init()
{
	SetByte(0x00406F36, 0xEB); // Crack npGameGuard::init

	SetByte(0x00406F5F, 0xEB); // Crack error messagebox

	SetByte(0x00406F9B, 0xEB); // Crack error messagebox

	SetByte(0x0041ECB5, 0xEB); // Crack mu.exe

	SetByte(0x0041ED25, 0xEB); // Crack OpenMainExe

	SetByte(0x0041ED5E, 0xEB); // Crack config.ini

	SetByte(0x0041EFB5, 0xEB); // Crack gg init

	SetByte(0x0053D90B, 0xEB); // Crack gameguard

	SetByte(0x0055961C, (gProtect.m_MainInfo.ClientVersion[0] + 1)); // Version

	SetByte(0x0055961D, (gProtect.m_MainInfo.ClientVersion[2] + 2)); // Version

	SetByte(0x0055961E, (gProtect.m_MainInfo.ClientVersion[3] + 3)); // Version

	SetByte(0x0055961F, (gProtect.m_MainInfo.ClientVersion[5] + 4)); // Version

	SetByte(0x00559620, (gProtect.m_MainInfo.ClientVersion[6] + 5)); // Version

	SetWord(0x005615BC, (gProtect.m_MainInfo.IpAddressPort)); // IpAddressPort

	MemoryCpy(0x00558ED8, gProtect.m_MainInfo.IpAddress, sizeof(gProtect.m_MainInfo.IpAddress)); // IpAddress

	MemoryCpy(0x00559624, gProtect.m_MainInfo.ClientSerial, sizeof(gProtect.m_MainInfo.ClientSerial)); // ClientSerial

	MemorySet(0x004127B0, 0x90, 5); // Remove MuError.log

	SetDword(0x0047FB96, 0x190); // Expand global message size before newline jump

	MemoryCpy(0x005616D0, "data\\music\\mutheme.mp3", 24); // Correct login music

	SetCompleteHook(0xE9, 0x00524146, 0x00524231); // Remove Select Character Texts

	SetCompleteHook(0xE9, 0x0052292E, 0x00522953); // Ignore Character Level to Create MG
	SetCompleteHook(0xE9, 0x00524293, 0x005242B8); // Ignore Character Level to Create MG

	SetWord(0x00444B76, 0x19EB); //-> Uniria & Dinorant Reflect

	SetCompleteHook(0xE8, 0x00526A5A, &this->ReduceCPU);
	SetByte(0x00526A5A + 5, 0x90);

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

	SetCompleteHook(0xE9, 0x004CBB15, &this->FixShopNpcClose); // Fix Shop NPC Closing send to server

	MemorySet(0x0042F2DF, 0x90, 0x7); // Fix Drop Zen

	SetDword(0x004EB8C7, 0x0A); // Expand max digits in Zen trade
	SetByte(0x0051667C, 0xEB); // Fix Trade Max Amount

	SetDword(0x00515BF3, MAX_CHARACTER_ZEN); // Block trade/warehouse to max Zen
	SetByte(0x00515BFB, 0x24); // Fix messagebox to max Zen

	SetDword(0x004EB6BD + 6, 0x0A); // Expand max digits in Warehouse deposit
	SetDword(0x004EB6F8 + 6, 0x0A); // Expand max digits in Warehouse extraction

	SetByte(0x004032A8, 0x0); // Fix Quest Item Preview

	SetCompleteHook(0xE9, 0x004028EA, &this->FixQuestProceedButton);

	// Remove JPG size limit
	MemorySet(0x005299E7, 0x90, 11);
	MemorySet(0x005299F8, 0x90, 12);
	SetByte(0x00529A18, 0xEB); // Width
	SetByte(0x00529A31, 0xEB); // Height
	// Remove TGA size limit
	MemorySet(0x00529E37, 0x90, 22);
	SetByte(0x00529E69, 0xEB); // Width
	SetByte(0x00529E8B, 0xEB); // Height

	SetByte(0x0042C417, 0xE8); // Fix autobuff view to others

	// Remove Twisting Sparks (Fix Joints overflow)
	MemorySet(0x00467222, 0x90, 5);
	SetByte(0x00467254, 1);

	// Fix Atlans and Icarus Goldens Overflow
	MemorySet(0x00501292, 0x90, 0x0A);

	SetCompleteHook(0xE9, 0x00443AAF, 0x00443D76); // Fix Dinorant Movement

	SetCompleteHook(0xE8, 0x0042B33D, &this->IgnoreRandomStuck);

	SetCompleteHook(0xE9, 0x00483AC5, &this->FixChasingAttackMovement);

	SetCompleteHook(0xE9, 0x004292BC, &this->FixWeaponGlow);

	SetCompleteHook(0xE9, 0x004424B4, &this->DecBMD); // Decrypt BMD

	// EncTerrain%d.map
	SetByte(0x0050E5F9, 0xEB);
	SetDword(0x0050E624, (DWORD)"Data\\%s\\EncTerrain%d.map");
	// EncTerrain%d.att
	SetDword(0x0050E649, (DWORD)"Data\\%s\\EncTerrain%d.att");
	// EncTerrain%d.obj
	SetByte(0x0050E663, 0xEB);
	SetDword(0x0050E68E, (DWORD)"Data\\%s\\EncTerrain%d.obj");

	SetCompleteHook(0xE8, 0x0050E636, &this->OpenTerrainMapping); // Decrypt MAP
	SetCompleteHook(0xE8, 0x0050E658, &this->OpenTerrainAttribute); // Decrypt ATT
	SetCompleteHook(0xE8, 0x0050E6A0, &this->OpenObjectsEnc); // Decrypt OBJ

	SetCompleteHook(0xE8, 0x0041ED4C, &this->ReadMainVersion);

	SetCompleteHook(0xE9, 0x0050F750, &this->MyOpenMacro);

	SetCompleteHook(0xE9, 0x0050F700, &this->MySaveMacro);

	SetCompleteHook(0xE8, 0x005269EC, &this->MySaveScreen);

	SetCompleteHook(0xE9, 0x005123C0, &this->MyBeginBitmap);

	SetCompleteHook(0xE8, 0x004BFE63, &MyGluPerspective2); // Fix Items when changing zoom

	SetCompleteHook(0xE8, 0x004BC0B3, &this->RenderNumArrow);

	SetCompleteHook(0xE8, 0x004BC0C1, &this->RenderEquipedHelperLife);

	SetCompleteHook(0xE8, 0x004BC0C7, &this->RenderBrokenItem);

	SetCompleteHook(0xE9, 0x00406B30, &this->CheckSpecialText);

	SetCompleteHook(0xE9, 0x00522597, &this->SetCharacterDeleteMaxLevel);

	SetDword(0x00484D15 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x00487D81 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x00489085 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x0048A8A4 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x0048B006 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x0049E80D + 1, 0x0A); // Skip Delay Skills
	SetDword(0x0049F285 + 1, 0x0A); // Skip Delay Skills
	SetDword(0x004A373A + 1, 0x0A); // Skip Delay Skills
	SetDword(0x004A5D8D + 1, 0x0A); // Skip Delay Skills
	SetDword(0x004A6539 + 1, 0x0A); // Skip Delay Skills

	SetCompleteHook(0xE9, 0x00486136, &this->SendContinueDeathStab);
	SetCompleteHook(0xE9, 0x00489769, &this->SendContinueBlast);
	SetCompleteHook(0xE9, 0x0048C6F9, &this->SendContinueTripleShot);
	SetCompleteHook(0xE9, 0x0049DBB1, &this->SendContinuePenetration);
	SetCompleteHook(0xE9, 0x004A0153, &this->SendContinueTwistingSlash);
	SetCompleteHook(0xE9, 0x004A132A, &this->SendContinueFireSlash);
	SetCompleteHook(0xE9, 0x004A25C6, &this->SendContinuePowerSlash);
	SetCompleteHook(0xE9, 0x004A45D4, &this->SendContinueRagefulBlow);
	SetCompleteHook(0xE9, 0x004A6D45, &this->SendContinueHellFire);
	SetCompleteHook(0xE9, 0x004A78BC, &this->SendContinueInferno);
	SetCompleteHook(0xE9, 0x004A886B, &this->SendContinueTwisterAndEvilSpirit);
	SetCompleteHook(0xE9, 0x004A9425, &this->SendContinueAquaBeam);
	SetCompleteHook(0xE9, 0x004AA038, &this->SendContinueFlame);

	SetCompleteHook(0xE9, 0x0048AF8E, &this->SendElfBuff);

	SetCompleteHook(0xE9, 0x00448930, &this->AttackStage);

	SetCompleteHook(0xE9, 0x004AF967, &this->FixMoveWhileAttacking);

	SetCompleteHook(0xE9, 0x0043FD70, &this->CalcFPS);

	SetCompleteHook(0xE9, 0x00501C26, &this->FixPigeons);
}

DWORD CPatchs::ReduceCPU()
{
	Sleep(1);

	return GetTickCount();
}

void CPatchs::ReduceRam(LPVOID lpThreadParameter)
{
	HANDLE CurrentProcess;

	while (TRUE)
	{
		Sleep(5000);

		CurrentProcess = GetCurrentProcess();

		SetProcessWorkingSetSize(CurrentProcess, 0xFFFFFFFF, 0xFFFFFFFF);

		SetThreadPriority(CurrentProcess, -2);
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

__declspec(naked) void CPatchs::FixQuestProceedButton()
{
	static DWORD jmpOnOk = 0x004028F2;
	static DWORD jmpOnNot = 0x00402F18;

	_asm
	{
		Pushad;
	}

	if (*(BYTE*)(g_csQuest + 0x1C882) == eQuestState::QUEST_CANCEL)
	{
		MouseLButtonPush = 0;

		MouseUpdateTime = 0;

		MouseUpdateTimeMax = 6;

		goto EXIT;
	}

	_asm
	{
		Popad;
		Mov Dword Ptr ds : [Esp + 0x28] , 0x00552460;
		Jmp jmpOnOk;
	}

EXIT:

	_asm
	{
		Popad;
		Jmp jmpOnNot;
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

_declspec(naked) void CPatchs::FixWeaponGlow()
{
	static DWORD jmpBack = 0x00429338;
	static DWORD LevelConvert = 0x0045C850;
	static DWORD SetCharacterScale = 0x0045C050;

	_asm
	{
		Mov Ecx, Dword Ptr[Esp + 0x14];
		Add Eax, 0x190;
		Push Ecx;
		Mov[Esi + 0x270], Ax;
		Call[LevelConvert];
		Add Esp, 4;
		Mov[Esi + 0x272], Al;
		Mov[Esi + 0x273], Bl;
		Push Esi;
		Call[SetCharacterScale];
		Add Esp, 4;
		Pop Edi;
		Pop Esi;
		Pop Ebp;
		Pop Ebx;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::DecBMD()
{
	static DWORD jmpBack = 0x004424BA;
	static BYTE* Data;

	_asm
	{
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

		return -1;
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

bool MyGetFileNameOfFilePath(char* lpszFile, char* lpszPath)
{
	int iFind = (int)'\\';

	char* lpFound = lpszPath;

	char* lpOld = lpFound;

	while (lpFound)
	{
		lpOld = lpFound;

		lpFound = strchr(lpFound + 1, iFind);
	}

	// copy name
	if (strchr(lpszPath, iFind))
	{
		strcpy_s(lpszFile, MAX_PATH, lpOld + 1);
	}
	else
	{
		strcpy_s(lpszFile, MAX_PATH, lpOld);
	}

	// get rid of options appended
	bool bCheck = true;

	for (char* lpTemp = lpszFile; bCheck; ++lpTemp)
	{
		switch (*lpTemp)
		{
			case '\"':
			case '\\':
			case '/':
			case ' ':
			{
				*lpTemp = '\0';
			}

			case '\0':
			{
				bCheck = false;

				break;
			}
		}
	}

	return true;
}

bool MyGetFileVersion(char* lpszFileName, WORD* pwVersion)
{
	DWORD dwHandle;

	DWORD dwLen = GetFileVersionInfoSize(lpszFileName, &dwHandle); // get size

	if (dwLen <= 0)
	{
		return false;
	}

	BYTE* pbyData = new BYTE[dwLen];

	if (!GetFileVersionInfo(lpszFileName, dwHandle, dwLen, pbyData)) // get actual information
	{
		delete[] pbyData;

		return false;
	}

	VS_FIXEDFILEINFO* pffi;

	UINT uLen;

	if (!VerQueryValue(pbyData, "\\", (LPVOID*)&pffi, &uLen)) // get version value
	{
		delete[] pbyData;

		return false;
	}

	pwVersion[0] = HIWORD(pffi->dwFileVersionMS);

	pwVersion[1] = LOWORD(pffi->dwFileVersionMS);

	pwVersion[2] = HIWORD(pffi->dwFileVersionLS);

	pwVersion[3] = LOWORD(pffi->dwFileVersionLS);

	delete[] pbyData;

	return true;
}

BOOL CPatchs::ReadMainVersion()
{
	memcpy(m_Version, "0.97.11", 11);

	char* lpszCommandLine = GetCommandLine();

	char lpszFile[MAX_PATH];

	if (MyGetFileNameOfFilePath(lpszFile, lpszCommandLine))
	{
		WORD wVersion[4];

		if (MyGetFileVersion(lpszFile, wVersion))
		{
			sprintf_s(m_ExeVersion, 11, "%d.%02d", wVersion[0], wVersion[1]);

			if (wVersion[2] > 0)
			{
				char lpszMinorVersion[3] = "a";

				if (wVersion[2] > 26)
				{
					lpszMinorVersion[0] = 'A';

					lpszMinorVersion[0] += (wVersion[2] - 27);

					lpszMinorVersion[1] = '+';
				}
				else
				{
					lpszMinorVersion[0] += (wVersion[2] - 1);
				}

				strcat_s(m_ExeVersion, 11, lpszMinorVersion);
			}
		}
		else
		{
			strcpy_s(m_ExeVersion, 11, m_Version);
		}
	}
	else
	{
		strcpy_s(m_ExeVersion, 11, m_Version);
	}

	return TRUE;
}

void CPatchs::MyOpenMacro(char* FileName)
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, FileName, "rt") != 0 || !fp)
	{
		return;
	}

	memset(MacroText, 0, sizeof(char) * 10 * 256);

	for (int i = 0; i < 10; ++i)
	{
		if (!fgets(MacroText[i], 256, fp))
		{
			break;
		}

		MacroText[i][strcspn(MacroText[i], "\r\n")] = '\0';
	}

	fclose(fp);
}

void CPatchs::MySaveMacro(char* FileName)
{
	FILE* fp = nullptr;

	if (fopen_s(&fp, FileName, "wt") != 0 || !fp)
	{
		return;
	}

	for (int i = 0; i < 10; ++i)
	{
		fprintf(fp, "%s\n", MacroText[i]);
	}

	fclose(fp);
}

void CPatchs::MySaveScreen()
{
	CreateDirectory(gProtect.m_MainInfo.ScreenShotPath, 0);

	SYSTEMTIME st;

	GetLocalTime(&st);

	sprintf_s(GrabFileName, MAX_PATH, "%s\\[%04d-%02d-%02d]%02d-%02d-%02d.jpg", gProtect.m_MainInfo.ScreenShotPath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	unsigned char* Buffer = new unsigned char[WindowWidth * WindowHeight * 3];

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadBuffer(GL_BACK);

	glReadPixels(0, 0, WindowWidth, WindowHeight, GL_RGB, GL_UNSIGNED_BYTE, Buffer);

	WriteJpeg(GrabFileName, WindowWidth, WindowHeight, Buffer, 100);

	delete[] Buffer;
}

void CPatchs::MyBeginBitmap()
{
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();

	glViewport(0, 0, WindowWidth, WindowHeight);

	gluPerspective(CameraFOV, (WindowWidth) / ((float)WindowHeight), CameraViewNear, CameraViewFar);

	glLoadIdentity();

	gluOrtho2D(0, WindowWidth, 0, WindowHeight);

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();

	glLoadIdentity();

	DisableDepthTest();
}

bool CPatchs::RenderNumArrow()
{
	STRUCT_DECRYPT;

	ITEM* PlayerRightHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_RIGHT)));

	ITEM* PlayerLeftHand = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_WEAPON_LEFT)));

	if (PlayerRightHand->Type == GET_ITEM(4, 15)) // Arrows
	{
		int screenWidth = GetScreenWidth();

		int MaxQuant = ((int(__cdecl*)()) 0x00482850)();

		char text[128] = { '\0' };

		sprintf_s(text, GlobalText[351], PlayerRightHand->Durability, MaxQuant);

		SelectObject(m_hFontDC, g_hFont);

		int PosX = screenWidth - GetTextWidth(text) - 10;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		EnableAlphaTest(true);

		DWORD backupBgTextColor = SetBackgroundTextColor;

		DWORD backupTextColor = SetTextColor;

		SetBackgroundTextColor = Color4b(0, 0, 0, 128);

		SetTextColor = Color4b(255, 255, 255, 255);

		RenderText(PosX, 4, text, 0, RT3_SORT_LEFT, NULL);

		SetBackgroundTextColor = backupBgTextColor;

		SetTextColor = backupTextColor;

		STRUCT_ENCRYPT;

		return true;
	}

	if (PlayerLeftHand->Type == GET_ITEM(4, 7)) // Bolts
	{
		int screenWidth = GetScreenWidth();

		int MaxQuant = ((int(__cdecl*)()) 0x00482850)();

		char text[128] = { '\0' };

		sprintf_s(text, GlobalText[352], PlayerLeftHand->Durability, MaxQuant);

		SelectObject(m_hFontDC, g_hFont);

		int PosX = screenWidth - GetTextWidth(text) - 6;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		EnableAlphaTest(true);

		DWORD backupBgTextColor = SetBackgroundTextColor;

		DWORD backupTextColor = SetTextColor;

		SetBackgroundTextColor = Color4b(0, 0, 0, 128);

		SetTextColor = Color4b(255, 255, 255, 255);

		RenderText(PosX, 4, text, 0, RT3_SORT_LEFT, NULL);

		SetBackgroundTextColor = backupBgTextColor;

		SetTextColor = backupTextColor;

		STRUCT_ENCRYPT;

		return true;
	}

	STRUCT_ENCRYPT;

	return false;
}

int CPatchs::RenderEquipedHelperLife(bool RenderedArrow)
{
	int PosY = 4;

	if (RenderedArrow)
	{
		PosY += 12;
	}

	int screenWidth = GetScreenWidth();

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	STRUCT_DECRYPT;

	ITEM* PlayerHelper = &*(ITEM*)(CharacterMachine + (536 + (68 * EQUIPMENT_HELPER)));

	if (PlayerHelper->Type >= GET_ITEM(13, 0) && PlayerHelper->Type <= GET_ITEM(13, 3))
	{
		ITEM_ATTRIBUTE* ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + PlayerHelper->Type * sizeof(ITEM_ATTRIBUTE));

		char text[128] = { '\0' };

		sprintf_s(text, "%s", ItemInfo->Name);

		SelectObject(m_hFontDC, g_hFont);

		int PosX = screenWidth - GetTextWidth(text) - 6;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		EnableAlphaTest(true);

		SetBackgroundTextColor = Color4b(0, 0, 0, 128);

		SetTextColor = Color4b(255, 255, 255, 255);

		RenderText(PosX, PosY, text, 0, RT3_SORT_LEFT, NULL);

		PosY += 12;

		float fDur = PlayerHelper->Durability / 255.0f * 50.0f;

		PosX = screenWidth - 60;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		RenderBar((float)PosX, (float)PosY, 50.0f, 2.0f, fDur, false, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		PosY += 12;
	}

	STRUCT_ENCRYPT;

	if (SummonLife)
	{
		char text[128] = { '\0' };

		sprintf_s(text, "%s", GlobalText[356]);

		SelectObject(m_hFontDC, g_hFont);

		int PosX = screenWidth - GetTextWidth(text) - 6;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		EnableAlphaTest(true);

		SetBackgroundTextColor = Color4b(0, 0, 0, 128);

		SetTextColor = Color4b(255, 255, 255, 255);

		RenderText(PosX, PosY, text, 0, RT3_SORT_LEFT, NULL);

		PosY += 12;

		float fDur = SummonLife * 0.5f;

		PosX = screenWidth - 60;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		RenderBar((float)PosX, (float)PosY, 50.0f, 2.0f, fDur, false, true);

		glColor3f(1.0f, 1.0f, 1.0f);

		PosY += 12;
	}

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;

	return PosY;
}

void CPatchs::RenderBrokenItem(int PosY)
{
	int screenWidth = GetScreenWidth();

	DWORD backupBgTextColor = SetBackgroundTextColor;

	DWORD backupTextColor = SetTextColor;

	ITEM* PlayerItem;

	ITEM_ATTRIBUTE* ItemInfo;

	int PosX;

	WORD MaxDur;

	char text[128] = { '\0' };

	STRUCT_DECRYPT;

	for (int i = EQUIPMENT_WEAPON_RIGHT; i <= EQUIPMENT_RING_LEFT; i++)
	{
		PlayerItem = &*(ITEM*)(CharacterMachine + (536 + (68 * i)));

		if (PlayerItem->Type == -1)
		{
			continue;
		}

		if (PlayerItem->Type == GET_ITEM(4, 7)
		    || PlayerItem->Type == GET_ITEM(4, 15)
		    || (PlayerItem->Type >= GET_ITEM(13, 0) && PlayerItem->Type <= GET_ITEM(13, 3)))
		{
			continue;
		}

		ItemInfo = (ITEM_ATTRIBUTE*)(ItemAttribute + PlayerItem->Type * sizeof(ITEM_ATTRIBUTE));

		MaxDur = CalcMaxDurability(PlayerItem, ItemInfo, (PlayerItem->Level >> 3) & 0xF);

		if (PlayerItem->Durability > (MaxDur * 0.5f))
		{
			continue;
		}

		if (PlayerItem->Durability == 0)
		{
			SetTextColor = Color4b(255, 255, 255, 255);

			SetBackgroundTextColor = Color4b(255, 10, 10, 128);

			PlayerItem->Color = 8;
		}
		else if (PlayerItem->Durability <= (MaxDur * 0.2f))
		{
			SetTextColor = Color4b(255, 10, 10, 255);

			SetBackgroundTextColor = Color4b(0, 0, 0, 128);

			PlayerItem->Color = 7;
		}
		else if (PlayerItem->Durability <= (MaxDur * 0.3f))
		{
			SetTextColor = Color4b(255, 156, 0, 255);

			SetBackgroundTextColor = Color4b(0, 0, 0, 128);

			PlayerItem->Color = 6;
		}
		else if (PlayerItem->Durability <= (MaxDur * 0.5f))
		{
			SetTextColor = Color4b(255, 228, 0, 255);

			SetBackgroundTextColor = Color4b(0, 0, 0, 128);

			PlayerItem->Color = 5;
		}

		sprintf_s(text, "%s (%d/%d)", ItemInfo->Name, PlayerItem->Durability, MaxDur);

		SelectObject(m_hFontDC, g_hFont);

		PosX = screenWidth - GetTextWidth(text) - 6;

		if (PartyNumber > 0)
		{
			PosX -= 60;
		}

		EnableAlphaTest(true);

		RenderText(PosX, PosY, text, 0, RT3_SORT_LEFT, NULL);

		PosY += 12;
	}

	STRUCT_ENCRYPT;

	SetBackgroundTextColor = backupBgTextColor;

	SetTextColor = backupTextColor;
}

bool CPatchs::CheckSpecialText(char* Text)
{
	if (gProtect.m_MainInfo.EnableSpecialCharacters != 0)
	{
		return false;
	}

	for (unsigned char* lpszCheck = (unsigned char*)Text; *lpszCheck; ++lpszCheck)
	{
		if (_mbclen(lpszCheck) == 1) // One byte
		{
			if (*lpszCheck < 48 || (58 <= *lpszCheck && *lpszCheck < 65) || (91 <= *lpszCheck && *lpszCheck < 97) || *lpszCheck > 122)
			{
				return true;
			}
		}
		else // Two bytes
		{
			unsigned char* lpszTrail = lpszCheck + 1;

			if (0x81 <= *lpszCheck && *lpszCheck <= 0xC8) // Korean
			{
				if ((0x41 <= *lpszTrail && *lpszTrail <= 0x5A)
				    || (0x61 <= *lpszTrail && *lpszTrail <= 0x7A)
				    || (0x81 <= *lpszTrail && *lpszTrail <= 0xFE))
				{ // Excluding transparent characters
					// Areas of special characters that are not allowed
					if (0xA1 <= *lpszCheck && *lpszCheck <= 0xAF && 0xA1 <= *lpszTrail)
					{
						return true;
					}
					else if (*lpszCheck == 0xC6 && 0x53 <= *lpszTrail && *lpszTrail <= 0xA0)
					{
						return true;
					}
					else if (0xC7 <= *lpszCheck && *lpszCheck <= 0xC8 && *lpszTrail <= 0xA0)
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
			else
			{
				return true;
			}

			++lpszCheck;
		}
	}

	return false;
}

_declspec(naked) void CPatchs::SetCharacterDeleteMaxLevel()
{
	static DWORD jmpOnOk = 0x005225C6;
	static DWORD jmpOnNot = 0x005225A5;
	static WORD PlayerLevel;

	_asm
	{
		Mov Ax, Word Ptr Ds : [Edx + Ecx * 4 + 0x1BE] ;
		Mov PlayerLevel, Ax;
		Lea Eax, Dword Ptr Ds : [Edx + Ecx * 4] ;
		Pushad;
	}

	if (PlayerLevel >= gPrintPlayer.MaxCharacterDeleteLevel)
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

_declspec(naked) void CPatchs::SendContinueDeathStab() // Death Stab
{
	static DWORD jmpBack = 0x00486B85;
	static DWORD c;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x0DD0 + 4] ;
		Mov c, Eax;
		Mov Dword Ptr Ds : [0x05826D10] , 0x2B;
		Pushad;
	}

	index = *(short*)(CharactersClient + (MovementSkillTarget * 916) + 0x1DC);

	gPatchs.SendRequestMagic(CurrentSkill, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueBlast() // Blast
{
	static DWORD jmpBack = 0x0048A11D;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x0D64] ;
		Mov c, Eax;
		Mov Dword Ptr Ds : [0x05826D10] , Edx;
		Pushad;
	}

	x = (int)(*(float*)(c + 788) / 100.0f);

	y = (int)(*(float*)(c + 792) / 100.0f);

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueTripleShot() // Triple Shot
{
	static DWORD jmpBack = 0x0048D305;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int dest;
	static int angle;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x1B64] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x1B68] ;
		Mov Ecx, Dword Ptr Ds : [Esp + 0x18] ;
		Xor Edx, Edx;
		Mov Dl, Byte Ptr Ds : [Eax + Ecx + 0x25] ;
		Mov Dword Ptr Ds : [0x05826D10] , Edx;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	dest = GetDestValue(x, y, TargetX, TargetY);

	angle = (int)((*(float*)(c + 0x24) + 180.0f) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, dest, angle, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinuePenetration() // Penetration
{
	static DWORD jmpBack = 0x0049E5FD;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Edi, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Edi;
		Mov Dword Ptr Ds : [0x05826D10] , 0x34;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueTwistingSlash() // Twisting Slash
{
	static DWORD jmpBack = 0x004A0AE2;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueFireSlash() // Fire Slash
{
	static DWORD jmpBack = 0x004A1D5B;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int des;
	static int index;

	_asm
	{
		Mov Edx, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Edx;
		Mov Edx, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Edx;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	des = GetDestValue(x, y, TargetX, TargetY);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, des, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinuePowerSlash() // Power Slash
{
	static DWORD jmpBack = 0x004A2FF9;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int des;
	static int pos;
	static int index;

	_asm
	{
		Mov Edx, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Edx;
		Mov Edx, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Edx;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	des = GetDestValue(x, y, TargetX, TargetY);

	pos = (BYTE)((*(float*)(c + 0x24) + 180.0f) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, des, pos, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueRagefulBlow() // Rageful Blow
{
	static DWORD jmpBack = 0x004A508E;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueHellFire() // Hell Fire
{
	static DWORD jmpBack = 0x004A7813;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueInferno() // Inferno
{
	static DWORD jmpBack = 0x004A83CC;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueTwisterAndEvilSpirit() // Twister And Evil Spirit
{
	static DWORD jmpBack = 0x004A9383;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int dest;
	static int angle;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueAquaBeam() // Aqua Beam
{
	static DWORD jmpBack = 0x004A9F91;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Eax;
		Mov Eax, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Eax;
		Pushad;
	}

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 904));

	x = (BYTE)(*(DWORD*)(c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 904));

	((void(__thiscall*)(DWORD, DWORD))0x0043D3E0)(0x055C9BC8, (c + 908));

	y = (BYTE)(*(DWORD*)(c + 908));

	((void(__thiscall*)(DWORD, DWORD))0x004233E0)(0x055C9BC8, (c + 908));

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendContinueFlame() // Flame
{
	static DWORD jmpBack = 0x004AAADC;
	static DWORD c;
	static int x;
	static int y;
	static int dir;
	static int index;

	_asm
	{
		Mov Edx, Dword Ptr Ds : [Esp + 0x139F4] ;
		Mov c, Edx;
		Mov Edx, Dword Ptr Ds : [Esp + 0x40] ;
		Mov Dword Ptr Ds : [0x05826D10] , Edx;
		Pushad;
	}

	x = (int)(*(float*)(c + 788) / 100.0f);

	y = (int)(*(float*)(c + 792) / 100.0f);

	dir = (int)(*(float*)(c + 0x24) / 360.0f * 256.0f);

	if (CheckAttack())
	{
		index = *(short*)(CharactersClient + (SelectedCharacter * 916) + 0x1DC);
	}
	else
	{
		index = -1;
	}

	gPatchs.SendRequestMagicContinue(CurrentSkill, x, y, dir, 0, 0, index);

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}

_declspec(naked) void CPatchs::SendElfBuff()
{
	static DWORD jmpBack = 0x0048B656;
	static DWORD jmpReturn = 0x0048B666;
	static DWORD c;
	static int index;
	static int iSkill;

	_asm
	{
		Pushad;
	}

	if (PartyNumber <= 0)
	{
		goto EXIT;
	}

	if (!IsPartyMember(SelectedCharacter))
	{
		goto EXIT;
	}

	if ((GetTickCount() - g_dwLatestMagicTick) <= 266)
	{
		goto SKIP;
	}

	g_dwLatestMagicTick = GetTickCount();

	STRUCT_DECRYPT;

	iSkill = *(BYTE*)(CharacterAttribute + 0x57 + *(DWORD*)0x07D7809C);

	STRUCT_ENCRYPT;

	index = *(short*)(CharactersClient + (MovementSkillTarget * 916) + 0x1DC);

	gPatchs.SendRequestMagic(iSkill, index);

SKIP:

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpReturn];
	}
}

void CPatchs::SendRequestMagicContinue(int skill, int x, int y, int dir, int dis, int angle, int target)
{
	PMSG_DURATION_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x1E, sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.x = x;

	pMsg.y = y;

	pMsg.dir = dir;

	pMsg.dis = dis;

	pMsg.angle = angle;

	pMsg.index[0] = HIBYTE(target);

	pMsg.index[1] = LOBYTE(target);

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

void CPatchs::SendRequestMagic(int skill, int target)
{
	PMSG_SKILL_ATTACK_SEND pMsg;

	pMsg.header.setE(0x19, sizeof(pMsg));

	pMsg.skill = skill;

	pMsg.index[0] = HIBYTE(target);

	pMsg.index[1] = LOBYTE(target);

	gProtocol.DataSend((BYTE*)&pMsg, sizeof(pMsg));
}

bool CPatchs::AttackStage(DWORD c, DWORD o)
{
	// Get weapon position
	int Hand = ((int(_cdecl*)(DWORD o)) 0x00448900)(o); // GetHandOfWeapon(o);

	int iSkill = ((BYTE(__thiscall*)(DWORD This, DWORD Skills)) 0x0045FAE0)(MAIN_HASH_CLASS, (c + 0x302));

	g_iLimitAttackTime = 15;

	int i;

	BYTE AttackTime = *(BYTE*)(c + 0x2F5);

	switch (iSkill)
	{
		case SKILL_DEATH_STAB:
		{
			DWORD b = gLoadModels.GetModels() + (0xBC * *(short*)(o + 2)); // BMD* b = &Models[o->Type];

			if (AttackTime == 8)
			{
				PlayBuffer(83, NULL, FALSE);
			}

			if (AttackTime <= 8)
			{
				// Location of gathering place
				vec3_t Position2 = { 0.0f, 0.0f, 0.0f };

				int LinkBone = *(BYTE*)(c + 24 * Hand + 0x274);

				float* pBoneTransform = (float*)(*(DWORD*)(o + 276) + 48 * LinkBone);

				TransformPosition(b, pBoneTransform, Position2, (float*)(o + 392), true);

				float fDistance = 300.0f;

				*(float*)(o + 392) += fDistance * sinf(*(float*)(o + 36) * Q_PI / 180.0f);

				*(float*)(o + 396) += -fDistance * cosf(*(float*)(o + 36) * Q_PI / 180.0f);

				// Screen random position lightnings
				if (AttackTime >= 1 && AttackTime <= 8)
				{
					// collect energy
					for (i = 0; i < 3; i++)
					{
						vec3_t CurPos;

						VectorCopy((float*)(o + 16), CurPos);

						CurPos[2] += 120.0f;

						vec3_t TempPos;

						GetNearRandomPos(CurPos, 300, TempPos);

						fDistance = 1400.0f;

						TempPos[0] += -fDistance * sinf(*(float*)(o + 36) * Q_PI / 180.0f);

						TempPos[1] += fDistance * cosf(*(float*)(o + 36) * Q_PI / 180.0f);

						CreateJoint(266, TempPos, TempPos, (float*)(o + 28), 2, o, 40.0f, -1, 0);
					}
				}
			}
			
			// Electric sparks on the enemy
			if (AttackTime >= 6 && AttackTime <= 12)
			{
				// Make the tail
				vec3_t Position;

				memcpy(Position, (float*)(o + 16), sizeof(vec3_t));

				Position[2] += 120.0f;

				float fDistance = 100.0f + ((AttackTime - 8) * 10.0f);

				Position[0] += fDistance * sinf(*(float*)(o + 36) * Q_PI / 180.0f);

				Position[1] += -fDistance * cosf(*(float*)(o + 36) * Q_PI / 180.0f);

				vec3_t Light = { 1.0f, 1.0f, 1.0f };

				CreateEffect(496, Position, (float*)(o + 28), Light, 1, o, -1, 0, 0);

				CreateEffect(496, Position, (float*)(o + 28), Light, 1, o, -1, 0, 0);

				if (*(short*)(c + 784) != -1) // if (c->TargetCharacter != -1)
				{
					DWORD tc = CharactersClient + 916 * *(short*)(c + 784);

					if (AttackTime >= 10 && *(BYTE*)(tc + 0)) // if (c->AttackTime >= 10 && to->Live)
					{
						*(BYTE*)(tc + 404) = 35; // to->m_byHurtByOneToOne = 35;
					}
				}
			}

			break;
		}

		case SKILL_IMPALE:
		{
			DWORD b = gLoadModels.GetModels() + (0xBC * *(short*)(o + 2)); // BMD* b = &Models[o->Type];

			vec3_t p;

			if (AttackTime == 10)
			{
				PlayBuffer(95, NULL, FALSE);
			}
			else if (AttackTime == 2) // Lighting Ball near the hand
			{
				// Preparation action
				vec3_t Light = { 1.0f, 1.0f, 0.5f };

				vec3_t Position2 = { 0.0f, 0.0f, 0.0f };

				int LinkBone = *(BYTE*)(c + 24 * Hand + 0x274);

				float* pBoneTransform = (float*)(*(DWORD*)(o + 276) + 48 * LinkBone);

				TransformPosition(b, pBoneTransform, Position2, p, true);

				CreateEffect(497, p, (float*)(o + 28), Light, *(short*)(c + 8 * (3 * Hand + 78)), o, -1, 0, 0);
			}
			else if (AttackTime == 1) // Yellow double spiral to the enemy
			{
				// Make the tail
				vec3_t Position;

				memcpy(Position, (float*)(o + 16), sizeof(vec3_t));

				Position[0] += 50.0f * sinf(*(float*)(o + 36) * Q_PI / 180.0f);

				Position[1] += -50.0f * cosf(*(float*)(o + 36) * Q_PI / 180.0f);

				Position[2] += 110.0f;

				vec3_t Light = { 1.0f, 1.0f, 1.0f };

				CreateEffect(496, Position, (float*)(o + 28), Light, 0, o, -1, 0, 0);

				CreateEffect(496, Position, (float*)(o + 28), Light, 0, o, -1, 0, 0);
			}
			else if (AttackTime >= 13 && AttackTime <= 14) // 3 Random smoke stakes to the enemy
			{
				// Brilliant spearmanship
				for (i = 0; i < 3; ++i)
				{
					vec3_t Position;

					memcpy(Position, (float*)(o + 16), sizeof(vec3_t));

					Position[0] += 145.0f * sinf(*(float*)(o + 36) * Q_PI / 180.0f);

					Position[1] += -145.0f * cosf(*(float*)(o + 36) * Q_PI / 180.0f);

					Position[2] += 110.0f;

					vec3_t Light = { 0.3f, 0.3f, 0.3f };

					if (AttackTime != 11)
					{
						Position[0] += (rand() % 60 - 30);

						Position[1] += (rand() % 60 - 30);

						Position[2] += (rand() % 60 - 30);
					}

					CreateEffect(266, Position, (float*)(o + 28), Light, *(short*)(c + 8 * (3 * Hand + 78)), o, -1, 0, 0);
				}
			}
			

			break;
		}

		case SKILL_GREATER_LIFE:
		{
			// if (c->AttackTime > 9 && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_SKILL_VITALITY)
			if (AttackTime > 9 && *(short*)(o + 2) == 390 && *(BYTE*)(o + 261) == 63)
			{
				*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
			}

			break;
		}

		case SKILL_FIRE_BREATH:
		{
			// if (o->AnimationFrame >= 5.0f && o->Type == MODEL_PLAYER && (o->CurrentAction == PLAYER_SKILL_RIDER || o->CurrentAction == PLAYER_SKILL_RIDER_FLY))
			if (*(float*)(o + 264) >= 5.0f && *(short*)(o + 2) == 390 && (*(BYTE*)(o + 261) == 64 || *(BYTE*)(o + 261) == 65))
			{
				*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
			}

			break;
		}

		case SKILL_PENETRATION:
		{
			// if (o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL)
			if (*(short*)(o + 2) == 390 && *(BYTE*)(o + 261) >= 34 && *(BYTE*)(o + 261) <= 91)
			{
				if (*(float*)(o + 264) >= 5.0f) // if (o->AnimationFrame >= 5.0f)
				{
					*(float*)(o + 268) = 4.0f; // o->PriorAnimationFrame = 4.0f;

					*(float*)(o + 264) = 5.0f; // o->AnimationFrame = 5.0f;
				}

				*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
			}

			if (AttackTime == 3) // Collect energy.
			{
				CreateEffect(1267, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), 0, o, -1, 0, 0);

				PlayBuffer(100, o, FALSE);
			}

			break;
		}

		case SKILL_FIRE_SLASH:
		{
			if (*(short*)(o + 2) == 390) // if (o->Type == MODEL_PLAYER)
			{
				// Hand lightning effect
				if (AttackTime >= 1 && AttackTime <= 2)
				{
					vec3_t Angle;

					Vector(1.0f, 0.0f, 0.0f, Angle);

					CreateEffect(1267, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), 1, o, -1, 0, 0);
				}

				// Red and yellow crescent-shaped flare
				if (*(float*)(o + 264) >= 3.0f) // if (o->AnimationFrame >= 3.0f)
				{
					PlayBuffer(84, NULL, FALSE);

					int HotKey = ((int(_cdecl*)(int Skill)) 0x004B1170)(iSkill);

					CreateEffect(1168, (float*)(o + 16), (float*)(o + 28), (float*)(o + 232), 0, o, *(WORD*)(o + 134), HotKey, 0);

					*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
				}
			}

			break;
		}

		case SKILL_POWER_SLASH:
		{
			// if (o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_TWO_HAND_SWORD_TWO)
			if (*(short*)(o + 2) == 390 && *(BYTE*)(o + 261) == 81)
			{
				vec3_t Angle;

				VectorCopy((float*)(o + 28), Angle);

				Angle[2] -= 40.0f;

				CreateEffect(203, (float*)(o + 16), Angle, (float*)(o + 232), 2, o, -1, 0, 0);

				Angle[2] += 20.0f;

				CreateEffect(203, (float*)(o + 16), Angle, (float*)(o + 232), 2, o, -1, 0, 0);

				Angle[2] += 20.0f;

				CreateEffect(203, (float*)(o + 16), Angle, (float*)(o + 232), 2, o, -1, 0, 0);

				Angle[2] += 20.0f;

				CreateEffect(203, (float*)(o + 16), Angle, (float*)(o + 232), 2, o, -1, 0, 0);

				Angle[2] += 20.0f;

				CreateEffect(203, (float*)(o + 16), Angle, (float*)(o + 232), 2, o, -1, 0, 0);

				PlayBuffer(84, NULL, FALSE);

				*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
			}

			break;
		}

		case SKILL_POISON:
		case SKILL_METEORITE:
		case SKILL_LIGHTNING:
		case SKILL_FIRE_BALL:
		case SKILL_FLAME:
		case SKILL_ICE:
		case SKILL_TWISTER:
		case SKILL_EVIL_SPIRIT:
		case SKILL_POWER_WAVE:
		case SKILL_AQUA_BEAM:
		case SKILL_BLAST:
		case SKILL_INFERNO:
		case SKILL_ENERGY_BALL:
		{
			*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;

			break;
		}

		default:
		{
			// if (o->AnimationFrame >= 1.0f && o->Type == MODEL_PLAYER && o->CurrentAction == PLAYER_ATTACK_SKILL_FURY_STRIKE)
			// if (o->AnimationFrame >= 5.0f && ((o->Type == MODEL_PLAYER && o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_RIDE_SKILL)
			// if ((o->Type >= MODEL_MONSTER01 && o->Type < MODEL_MONSTER_END) && o->CurrentAction >= MONSTER01_ATTACK1 && o->CurrentAction <= MONSTER01_ATTACK2)
			if ((*(float*)(o + 264) >= 1.0f && *(short*)(o + 2) == 390 && *(BYTE*)(o + 261) == 62)
			    || (*(float*)(o + 264) >= 5.0 && *(short*)(o + 2) == 390 && *(BYTE*)(o + 261) >= 34 && *(BYTE*)(o + 261) <= 91)
			    || (*(short*)(o + 2) >= 270 && *(short*)(o + 2) < 335 && *(BYTE*)(o + 261) >= 3u && *(BYTE*)(o + 261) <= 4u))
			{
				*(BYTE*)(c + 0x2F5) = 15; // c->AttackTime = 15;
			}

			break;
		}
	}

	return true;
}

_declspec(naked) void CPatchs::FixMoveWhileAttacking()
{
	static DWORD jmpOnOk = 0x004AF98B;
	static DWORD jmpOnNot = 0x004AF99E;

	static DWORD c;
	static DWORD PosX;
	static DWORD PosY;

	_asm
	{
		Mov c, Ebp;
		Mov PosY, Ebx;
		Mov PosX, Edi;
		Pushad;
	}

	if ((*(BYTE*)(c + 0x105) >= 34 && *(BYTE*)(c + 0x105) <= 67) // (o->CurrentAction >= PLAYER_ATTACK_FIST && o->CurrentAction <= PLAYER_ATTACK_ONETOONE)
	    || (*(BYTE*)(c + 0x105) >= 81 && *(BYTE*)(c + 0x105) <= 92)) // (o->CurrentAction >= PLAYER_ATTACK_TWO_HAND_SWORD_TWO && o->CurrentAction <= PLAYER_DEFENSE1)
	{
		goto EXIT;
	}

	if (!PathFinding2(PosX, PosY, TargetX, TargetY, (c + 0x354), 0.0f))
	{
		goto EXIT;
	}

	_asm
	{
		Popad;
		Jmp[jmpOnOk];
	}

EXIT:

	_asm
	{
		Popad;
		Jmp[jmpOnNot];
	}
}

void CPatchs::CalcFPS()
{
	float dif = (float)clock() - WorldTime;

	DeltaT = ((dif) ? dif : 0.1f) / CLOCKS_PER_SEC;

	FPS = CLOCKS_PER_SEC / ((dif) ? dif : 0.1f);

	WorldTime = (float)clock();
}

_declspec(naked) void CPatchs::FixPigeons()
{
	static DWORD jmpBack = 0x00501C66;
	static DWORD o;
	static float Height;

	_asm
	{
		Mov o, Esi;
		Pushad;
	}

	*(float*)(o + 0xC8) = -20.0f; // o->Direction[2] = -20.0f;

	Height = RequestTerrainHeight(*(float*)(o + 0x10), *(float*)(o + 0x14)); // RequestTerrainHeight(o->Position[0], o->Position[1]);

	if (*(float*)(o + 0x18) < Height) // if (o->Position[2] < Height)
	{
		*(float*)(o + 0x18) = (float)Height; // o->Position[2] = Height;

		*(BYTE*)(o + 0x104) = 2; // o->AI = BOID_GROUND;

		*(float*)(o + 0x0CC) = (float)(rand() % 4 + 6) * 0.05f; // o->Velocity = (float)(rand() % 4 + 6) * 0.05f;

		*(BYTE*)(o + 0x105) = 1; // o->CurrentAction = 1;
	}

	_asm
	{
		Popad;
		Jmp[jmpBack];
	}
}