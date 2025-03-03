#include "stdafx.h"

CConsole gConsole;

CConsole::CConsole()
{
	SetConsoleOutputCP(1252);

	this->HideConsole = true;

	this->EnableOutput[CON_GENERAL] = true;
	this->EnableOutput[CON_PROTO_TCP_RECV] = false;
	this->EnableOutput[CON_PROTO_TCP_SEND] = false;
}

CConsole::~CConsole()
{

}

void CConsole::Output(int type, char* text, ...)
{
	if (this->HideConsole)
	{
		return;
	}

	if (this->EnableOutput[type] == false)
	{
		return;
	}

	SYSTEMTIME now;

	GetLocalTime(&now);

	char time[10];

	wsprintf(time, "%02d:%02d:%02d", now.wHour, now.wMinute, now.wSecond);

	char temp[1024];

	va_list arg;

	va_start(arg, text);

	vsprintf_s(temp, text, arg);

	va_end(arg);

	char log[1024];

	wsprintf(log, "[%s] %s", time, temp);

	std::cout << log << std::endl;
}