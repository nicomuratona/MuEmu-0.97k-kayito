#pragma once

#define READ_SCRIPT_ALLOC_ERROR "[ReadScript] Could not alloc memory. File: '%s'.\n"
#define READ_SCRIPT_FILE_ERROR "[ReadScript] Could not open file '%s'.\n"
#define READ_SCRIPT_LINE_ERROR "[ReadScript] There is an error in file '%s' in line %d.\n"
#define READ_SCRIPT_UNKNOWN_ERROR "[ReadScript] Unknow error in file '%s'.\n"

enum eTokenResult
{
	TOKEN_NUMBER = 0,
	TOKEN_STRING = 1,
	TOKEN_END = 2,
	TOKEN_END_LINE = 3,
	TOKEN_END_SECTION = 4,
	TOKEN_ERROR = 5,
};

class CReadScript
{
public:

	CReadScript();

	virtual ~CReadScript();

	bool Load(char* path);

	eTokenResult GetToken(bool wReturn = false);

	int GetNumber();

	int GetAsNumber();

	float GetFloatNumber();

	float GetAsFloatNumber();

	const char* GetString();

	const char* GetAsString();

	char* GetError();

private:

	FILE* m_file;

	char m_path[MAX_PATH];

	float m_number;

	char m_string[100];

	DWORD m_line;

	eTokenResult m_lastToken;

	char m_error[256];
};