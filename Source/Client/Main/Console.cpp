#include "stdafx.h"

Console gConsole;

Console::Console()
{
	this->inUse = false;
}

Console::~Console()
{
	this->EndConsole();
}

void Console::StartConsole()
{
	if (this->inUse)
	{
		return;
	}

	this->inUse = true;

	AllocConsole();

	SetConsoleTitle("Console Debugger");

	this->Write("Console Started!");
}

void Console::EndConsole()
{
	if (!this->inUse)
	{
		return;
	}

	FreeConsole();
}

void Console::Write(char* message, ...)
{
	if (!this->inUse)
	{
		return;
	}

	tm today;

	time_t ltime;

	time(&ltime);

	if (localtime_s(&today, &ltime) != 0)
	{
		return;
	}

	char time[32];

	if (asctime_s(time, sizeof(time), &today) != 0)
	{
		return;
	}

	char temp[1024];

	va_list arg;

	va_start(arg, message);

	vsprintf_s(temp, message, arg);

	va_end(arg);

	char log[1024];

	wsprintf(log, "[%.8s] %s\n", &time[11], temp);

	DWORD bytewrite;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);

	WriteFile(Handle, log, (DWORD)strlen(log), &bytewrite, 0);
}