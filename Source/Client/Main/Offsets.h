#pragma once

// Window
#define m_Resolution *(int*)0x055C9E38
#define WindowWidth *(int*)0x0056156C
#define WindowHeight *(int*)0x00561570
#define g_fScreenRate_x *(float*)0x55C9B70
#define g_fScreenRate_y *(float*)0x55C9B74
#define m_hFontDC *(HDC*)0x055C9FEC
#define g_hWnd *(HWND*)0x055C9FFC
#define g_hDC *(HDC*)0x055CA004
#define g_hRC *(HGLRC*)0x055CA008
#define WndProc (WNDPROC)(0x4149D0)

// State
#define SceneFlag *(int*)0x005615C0
#define	World *(int*)0x0055A7AC
#define MouseX *(int*)0x083A427C
#define MouseY *(int*)0x083A4278
#define MouseLButton *(bool*)0x083A42C4
#define MouseRButton *(bool*)0x083A42AC
#define MouseLButtonPush *(bool*)0x083A4124
#define MouseRButtonPush *(bool*)0x083A42D0
#define MouseUpdateTime *(DWORD*)0x07E11D28
#define MouseUpdateTimeMax *(DWORD*)0x00559BEC
#define ItemKey *(int*)0x07E109C8
#define RepairEnable *(int*)0x07EAA138
#define KeyState 0x07E118EC
#define m_bAutoAttack *(bool*)0x00559C5C
#define m_bWhisperSound *(bool*)0x07E11D80

// Interface
#define pPickedItem (&*(ITEM*)0x07E91350)
#define OffsetTradeItems 0x07EA7B88
#define OffsetMixItems 0x07EA9848
#define OffsetWarehouseItems 0x07EA5B30
#define OffsetInventoryItems 0x07EA8410
#define GetInventoryItem(slot) (ITEM*)(OffsetInventoryItems + (sizeof(ITEM) * slot))
#define GetItemFromInventory(inventory, slot) (ITEM*)(inventory + (sizeof(ITEM) * slot))
#define CreateGuildMark ((void(__cdecl*)(int nMarkIndex, bool blend)) 0x004F0100)
#define RenderBoolean ((void(__cdecl*)(int x, int y, DWORD c)) 0x00480E00)
#define LoadImageJPG ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529740)
#define LoadImageTGA ((int(__cdecl*)(char *Folder, int ModelID, GLint a3, GLint a4, int a5, int a6)) 0x00529BD0)
#define MouseOnWindow *(bool*)0x07D78094
#define InventoryOpened *(BYTE*)0x07EAA117
#define GuildOpened *(BYTE*)0x07EAA114
#define CharacterOpened *(BYTE*)0x07EAA116
#define PartyOpened *(BYTE*)0x07EAA115
#define GoldenArcherOpened *(DWORD*)0x07EAA128
#define ErrorMessage *(BYTE*)0x083A7C24

// Drawing
#define RenderBitmap ((void(__cdecl*)(int Texture, float x, float y, float Width, float Height, float u, float v, float uWidth, float vHeight, bool Scale, bool StartScale)) 0x005125A0)
#define RenderNumber2D ((void(*)(float x, float y, int Num, float Width, float Height))0x005122F0)
#define RenderColor ((void(__cdecl*)(float x, float y, float width, float height)) 0x005124C0)
#define CreatePoint ((void(_cdecl*)(float Position[3], int Value, float Color[3], float scale, bool bMove))0x004792C0)
#define CreateEffect ((void(__cdecl*)(int Type, float* Position, float* Angle, float* Light, int SubType, DWORD Owner, int PKKey, int SkillIndex, int Skill)) 0x00460DC0)
#define DeleteEffect ((void(__cdecl*)(int Type, DWORD Owner, int iSubType)) 0x00460D20)
#define CreateJoint ((void(_cdecl*)(int Type, float* Position, float* TargetPosition, float* Angle, int SubType, DWORD Target, float Scale, short PKKey, WORD SkillIndex)) 0x0046D840)
#define DeleteJoint ((void(__cdecl*)(int Type, DWORD Target, int SubType)) 0x0046FE00)

// Font
#define FontHeight *(int*)0x07D78080
#define g_hFont *(HFONT*)0x055CA00C
#define g_hFontBold *(HFONT*)0x055CA010

// Text
#define SetTextColor *(DWORD*)0x00559C78
#define SetBackgroundTextColor *(DWORD*)0x00559C80
#define RenderText ((int(__cdecl*)(int iPos_x, int iPos_y, char *pszText, int iBoxWidth, int iSort, SIZE *lpTextSize)) 0x0047F650)
#define GetTextLine(x) (char*)(0x07D29D24 + (300 * x))
#define UIChatLogWindow_AddText ((void(_cdecl*)(const char* strID, const char* strText, int MsgType)) 0x00480620)
#define CreateNotice ((void(__cdecl*)(char *Text, int Color)) 0x0047FAE0)

// Input
#define InputEnable *(bool*)0x00559C84

// Character
#define MAIN_CHARACTER_STRUCT 0x07CF1FF4
#define Hero *(DWORD*)0x07ABF5D8
#define PartyNumber *(int*)0x07EAA0E0
#define CharactersClient *(DWORD*)0x07ABF5D0
#define Projection ((void(__cdecl*)(float *Position, int *sx, int *sy)) 0x005113F0)
#define Action ((void(__cdecl*)(DWORD c, DWORD o)) 0x0048D640)

// Item
#define SendRequestEquipmentItem ((bool(_cdecl*)(int SrcFlag, int SrcIndex, int DstFlag, int DstIndex)) 0x0043C250)

// Connection
#define MAIN_PACKET_SERIAL 0x05826CEB
#define MAIN_STRUCT_SERIAL 0x07CF1FFC
#define MAIN_HASH_CLASS 0x055C9BC8
#define MAIN_CONNECTION_STATUS 0x05826CF0
#define PACKET_DECRYPT ((void(__thiscall*)(void*,int))0x00422DF0)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)
#define PACKET_ENCRYPT ((void(__thiscall*)(void*,int))0x00404040)((void*)0x055C9BC8,MAIN_PACKET_SERIAL)
#define STRUCT_DECRYPT ((void(__thiscall*)(void*,void*))0x00423040)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)
#define STRUCT_ENCRYPT ((void(__thiscall*)(void*,void*))0x0043D1D0)((void*)MAIN_HASH_CLASS,*(void**)MAIN_STRUCT_SERIAL)
#define pSocket	*(SOCKET*)(0x055CA168)
#define ProtocolCore ((BOOL(*)(BYTE*))0x004389A0)
#define SendChat ((void(__cdecl*)(const char* Text)) 0x004C1B90)

// OpenGL
#define EnableAlphaTest ((void(__cdecl*)(BYTE)) 0x00511680)
#define EnableAlphaBlend ((void(__cdecl*)()) 0x00511710)
#define DisableAlphaBlend ((void(__cdecl*)()) 0x00511600)

// Sound
#define PlayBuffer ((int(__cdecl*)(int Buffer, DWORD Object, BOOL bLooped))0x00404BC0)
#define CreateStaticBuffer ((HRESULT(__cdecl*)(int Buffer, const char* strFileName, int MaxChannel, bool Enable))0x00404650)
#define FillBuffer ((HRESULT(__cdecl*)(int Buffer, int MaxChannel, bool Enable))0x00404A00)
#define LoadWaveFile ((HRESULT(__cdecl*)(int Buffer, TCHAR* strFileName, int MaxChannel, bool Enable))0x00404A10)
#define g_EnableSound *(bool*)0x00590AC8
#define g_Enable3DSound *(bool*)0x00590AC9
#define SoundLoadCount *(int*)0x00590ACC
#define BufferChannel (&*(int*)0x00583DAC)// int BufferChannel[420];
#define MaxBufferChannel (&*(int*)0x0058E1C4) // int MaxBufferChannel[420];
#define Enable3DSound (&*(bool*)0x00590924) // bool Enable3DSound[420];
#define BufferName 0x00585E7C // char BufferName[420][64];
#define g_lpDSBuffer (&*(LPDIRECTSOUNDBUFFER*)0x0058C780) // LPDIRECTSOUNDBUFFER g_lpDSBuffer[420][4]

// Models
#define ItemAttribute *(DWORD*)0x07D78068