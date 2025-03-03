#include "stdafx.h"
#include "MapFog.h"

CMapFog gMapFog;

CMapFog::CMapFog()
{
        for (int i = 0; i < MAX_MAPS; i++)
        {
                this->m_MapFog[i].MapNumber = -1;
        }

        this->Init();
}

CMapFog::~CMapFog()
{

}

void CMapFog::Init(MAP_FOG_INFO* info)
{
        for (int i = 0; i < MAX_MAPS; i++)
        {
                this->SetInfo(info[i]);
        }
}

void CMapFog::SetInfo(MAP_FOG_INFO info)
{
        if (info.MapNumber < 0 || info.MapNumber >= MAX_MAPS)
        {
                return;
        }

        this->m_MapFog[info.MapNumber] = info;
}

void CMapFog::Init()
{
        SetByte(0x00525ADD, 1); // FogEnable = true;

        SetCompleteHook(0xE9, 0x00511B66, &this->glEnableFog);

        SetDword(0x0055233C, (DWORD)&this->glClearColorFog);
}

MAP_FOG_INFO* CMapFog::GetInfoByIndex(int MapNumber)
{
        if (!MAP_RANGE(MapNumber))
        {
                return NULL;
        }

        if (this->m_MapFog[MapNumber].MapNumber != MapNumber)
        {
                return NULL;
        }

        return &this->m_MapFog[MapNumber];
}

bool CMapFog::GetFogRGB(int MapNumber, float* DayFogRGB, float* NightFogRGB)
{
        MAP_FOG_INFO* lpInfo = this->GetInfoByIndex(MapNumber);

        if (lpInfo == NULL)
        {
                return false;
        }

        DayFogRGB[0] = (lpInfo->DayRGB[0] / 255.0f);
        DayFogRGB[1] = (lpInfo->DayRGB[1] / 255.0f);
        DayFogRGB[2] = (lpInfo->DayRGB[2] / 255.0f);

        NightFogRGB[0] = (lpInfo->NightRGB[0] / 255.0f);
        NightFogRGB[1] = (lpInfo->NightRGB[1] / 255.0f);
        NightFogRGB[2] = (lpInfo->NightRGB[2] / 255.0f);

        return true;
}

_declspec(naked) void CMapFog::glEnableFog()
{
        static DWORD jmpBack = 0x00511B98;

        _asm
        {
                Pushad;
        }

        glFogi(GL_FOG_MODE, GL_LINEAR);

        glFogf(GL_FOG_START, CameraDistance);

        glFogf(GL_FOG_END, CameraViewFar * 0.57f);

        glFogfv(GL_FOG_COLOR, FogColor);

        _asm
        {
                Popad;
                Jmp[jmpBack];
        }
}

void WINAPI CMapFog::glClearColorFog(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
        if (SceneFlag == MAIN_SCENE && FogEnable)
        {
                DWORD currentTick = GetTickCount();

                // Update when change map or after 10 seconds
                if (World != gMapFog.m_LastWorld || currentTick - gMapFog.m_LastTickCount >= 10000)
                {
                        gMapFog.ChangeFog();

                        gMapFog.m_LastWorld = World;

                        gMapFog.m_LastTickCount = currentTick;
                }

                return glClearColor(FogColor[0], FogColor[1], FogColor[2], FogColor[3]);
        }

        return glClearColor(red, green, blue, alpha);
}

void CMapFog::ChangeFog()
{
        SYSTEMTIME sm;

        GetLocalTime(&sm);

        float minutestate = (float)sm.wMinute / 60.0f;

        float DayRGB[3];
        float NightRGB[3];

        if (!this->GetFogRGB(World, DayRGB, NightRGB))
        {
                DayRGB[0] = this->m_DefaultDayRGB[0] / 255.0f;
                DayRGB[1] = this->m_DefaultDayRGB[1] / 255.0f;
                DayRGB[2] = this->m_DefaultDayRGB[2] / 255.0f;

                NightRGB[0] = this->m_DefaultNightRGB[0] / 255.0f;
                NightRGB[1] = this->m_DefaultNightRGB[1] / 255.0f;
                NightRGB[2] = this->m_DefaultNightRGB[2] / 255.0f;
        }

        float t = 0.0f;

        if (sm.wHour == 7)
        {
                t = minutestate;
        }
        else if (sm.wHour > 7 && sm.wHour < 20)
        {
                t = 1.0f;
        }
        else if (sm.wHour == 20)
        {
                t = 1.0f - minutestate;
        }
        else
        {
                t = 0.0f;
        }

        for (int i = 0; i < 3; i++)
        {
                FogColor[i] = (1.0f - t) * NightRGB[i] + t * DayRGB[i];
        }
}