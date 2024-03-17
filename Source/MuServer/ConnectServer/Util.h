#pragma once

#include "ServerDisplayer.h"

void ErrorMessageBox(char* message, ...);

void LogAdd(eLogColor color, char* text, ...);

void ConsoleProtocolLog(int type, BYTE* lpMsg, int size);

void ConnectServerTimeoutProc();

int GetFreeClientIndex();

int SearchFreeClientIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime);

extern int gClientCount;