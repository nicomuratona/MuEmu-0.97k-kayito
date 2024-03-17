#pragma once

enum eOutputType
{
	CON_GENERAL = 0,
	CON_PROTO_TCP_RECV = 1,
	CON_PROTO_TCP_SEND = 2,
	MAX_CONSOLE_OUTPUT
};

class Console
{
public:

	Console(); // Constructor

	virtual ~Console(); // Destructor

	void StartConsole(); // Starts the console

	void EndConsole(); // Ends the console

	void Write(char* message, ...); // Write a message in the console

	void Clear();

	const char* GetOutputString(int type);

private:

	bool inUse; // Current state of the console

	std::map<int, std::string> m_OutputType;
};

extern Console gConsole;