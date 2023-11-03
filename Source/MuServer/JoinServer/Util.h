#pragma once

#include "ServerDisplayer.h"
#include "ServerManager.h"

void ErrorMessageBox(char* message, ...);

void LogAdd(eLogColor color, char* text, ...);

bool CheckTextSyntax(char* text, int size);

LONG CheckAccountCaseSensitive(int value);

int GetFreeServerIndex();

int SearchFreeServerIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime);

CServerManager* FindServerByCode(WORD ServerCode);

DWORD MakeAccountKey(char* account);

extern int gServerCount;