#pragma once

class Console
{
public:

	Console(); // Constructor

	virtual ~Console(); // Destructor

	void StartConsole(); // Starts the console

	void EndConsole(); // Ends the console

	void Write(char* message, ...); // Write a message in the console

private:

	bool inUse; // Current state of the console
};

extern Console gConsole;