#pragma once

enum eOutputType
{
	CON_GENERAL = 0,
	CON_PROTO_TCP_RECV = 1,
	CON_PROTO_TCP_SEND = 2,
	MAX_CONSOLE_OUTPUT
};

class CConsole
{
public:

	CConsole();

	~CConsole();

	void Output(int type, char* text, ...);

public:

	bool HideConsole;

	bool EnableOutput[5];
};

extern CConsole gConsole;