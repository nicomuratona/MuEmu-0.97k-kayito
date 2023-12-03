#pragma once

#include "ServerDisplayer.h"
#include "User.h"

int SafeGetItem(int index);

float GetRoundValue(float value);

BYTE GetExceOptionCount(BYTE ExceOption);

int LevelSmallConvert(int level);

bool CheckSpecialText(char* Text);

bool CheckSpaceCharacter(char* Text);

BYTE GetPathPacketDirPos(int px, int py);

void PacketArgumentDecrypt(char* out_buff, char* in_buff, int size);

void ErrorMessageBox(char* message, ...);

void LogAdd(eLogColor color, char* text, ...);

bool DataSend(int aIndex, BYTE* lpMsg, DWORD size);

void DataSendAll(BYTE* lpMsg, int size);

bool DataSendSocket(SOCKET socket, BYTE* lpMsg, DWORD size);

void MsgSendV2(LPOBJ lpObj, BYTE* lpMsg, int size);

void CloseClient(int aIndex);

void PostMessageGold(char* name, char* serverName, int message, char* text);

void PostMessageBlue(char* name, char* serverName, int message, char* text);

void PostMessageGreen(char* name, char* serverName, int message, char* text);

void PostMessageLightGreen(char* name, char* serverName, int message, char* text);

void SetLargeRand();

long GetLargeRand();

extern short RoadPathTable[MAX_ROAD_PATH_TABLE];