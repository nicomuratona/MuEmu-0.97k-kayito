#include "stdafx.h"
#include "Resolution.h"

CResolution gResolution;

void CResolution::Init()
{
	SetCompleteHook(0xE9, 0x0041E36E, &this->ResolutionSwitch);

	SetCompleteHook(0xE9, 0x0041F012, &this->ResolutionSwitchFont);
}

__declspec(naked) void CResolution::ResolutionSwitch()
{
	static DWORD ResolutionSwitchAddress1 = 0x0041E3EA;

	_asm
	{
		Mov Eax, Dword Ptr Ds : [0x055C9E38] ; //MAIN_RESOLUTION
		Cmp Eax, 0x00;
		Jnz NEXT1;
		Mov Dword Ptr Ds : [0x0056156C] , 640; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 480; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT1:
		Cmp Eax, 0x01;
		Jnz NEXT2;
		Mov Dword Ptr Ds : [0x0056156C] , 800; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 600; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT2:
		Cmp Eax, 0x02;
		Jnz NEXT3;
		Mov Dword Ptr Ds : [0x0056156C] , 1024; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 768; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT3:
		Cmp Eax, 0x03;
		Jnz NEXT4;
		Mov Dword Ptr Ds : [0x0056156C] , 1280; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 1024; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT4:
		Cmp Eax, 0x04;
		Jnz NEXT5;
		Mov Dword Ptr Ds : [0x0056156C] , 1360; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 768; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT5:
		Cmp Eax, 0x05;
		Jnz NEXT6;
		Mov Dword Ptr Ds : [0x0056156C] , 1440; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 900; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT6:
		Cmp Eax, 0x06;
		Jnz NEXT7;
		Mov Dword Ptr Ds : [0x0056156C] , 1600; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 900; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT7:
		Cmp Eax, 0x07;
		Jnz NEXT8;
		Mov Dword Ptr Ds : [0x0056156C] , 1680; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 1050; //MAIN_RESOLUTION_Y
		Jmp EXIT;
	NEXT8:
		Cmp Eax, 0x08;
		Jnz EXIT;
		Mov Dword Ptr Ds : [0x0056156C] , 1920; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ds : [0x00561570] , 1080; //MAIN_RESOLUTION_Y
	EXIT:
		Jmp[ResolutionSwitchAddress1];
	}
}

__declspec(naked) void CResolution::ResolutionSwitchFont()
{
	static DWORD ResolutionSwitchFontAddress1 = 0x0041F08C;

	_asm
	{
		Mov Edx, Dword Ptr Ds : [0x0056156C] ; //MAIN_RESOLUTION_X
		Mov Dword Ptr Ss : [Ebp - 0x1620] , Edx;
		Cmp Dword Ptr Ss : [Ebp - 0x1620] , 640;
		Jnz NEXT1;
		Mov Dword Ptr Ds : [0x07D78080] , 0x0C; //MAIN_FONT_SIZE
		Jmp EXIT;
	NEXT1:
		Cmp Dword Ptr Ss : [Ebp - 0x1620] , 800;
		Jnz NEXT2;
		Mov Dword Ptr Ds : [0x07D78080] , 0x0D; //MAIN_FONT_SIZE
		Jmp EXIT;
	NEXT2:
		Cmp Dword Ptr Ss : [Ebp - 0x1620] , 1024;
		Jnz NEXT3;
		Mov Dword Ptr Ds : [0x07D78080] , 0x0E; //MAIN_FONT_SIZE
		Jmp EXIT;
	NEXT3:
		Cmp Dword Ptr Ss : [Ebp - 0x1620] , 1280;
		Jnz NEXT4;
		Mov Dword Ptr Ds : [0x07D78080] , 0x0F; //MAIN_FONT_SIZE
		Jmp EXIT;
	NEXT4:
		Mov Dword Ptr Ds : [0x07D78080] , 0x0F; //MAIN_FONT_SIZE
	EXIT:
		Jmp[ResolutionSwitchFontAddress1];
	}
}