#pragma once

#define MEM_SCRIPT_ALLOC_ERROR "[%s] Could not alloc memory for MemScript.\n"
#define MEM_SCRIPT_ERROR_CODE0 "[%s] Could not open file.\n"
#define MEM_SCRIPT_ERROR_CODE1 "[%s] Could not alloc file buffer.\n"
#define MEM_SCRIPT_ERROR_CODE2 "[%s] Could not read file.\n"
#define MEM_SCRIPT_ERROR_CODE3 "[%s] Could not get file buffer.\n"
#define MEM_SCRIPT_ERROR_CODE4 "[%s] The file is taking too long to read.\n"
#define MEM_SCRIPT_ERROR_CODE5 "[%s] There is an error in line %d.\n"
#define MEM_SCRIPT_ERROR_CODEX "[%s] Unknow error code: %d.\n"

enum eTokenResult
{
	TOKEN_NUMBER = 0,
	TOKEN_STRING = 1,
	TOKEN_END = 2,
	TOKEN_END_LINE = 3,
	TOKEN_END_SECTION = 4,
	TOKEN_ERROR = 5,
};

class CMemScript
{
public:

	CMemScript();

	virtual ~CMemScript();

	bool SetBuffer(char* path);

	eTokenResult GetToken(bool wReturn = false);

	char* GetLastError();

	int GetNumber();

	int GetAsNumber();

	float GetFloatNumber();

	float GetAsFloatNumber();

	char* GetString();

	char* GetAsString();

private:

	bool IsEncodingAnsi();

	char GetChar();

	void UnGetChar(char ch);

	char CheckComment(char ch);

	bool CheckSpace(char ch);

	eTokenResult GetTokenNumber(char ch);

	eTokenResult GetTokenString(char ch);

	eTokenResult GetTokenCommon(char ch);

	void SetLastError(int error);

private:

	std::vector<char> m_buffer;

	size_t m_position;

	char m_path[256];

	float m_number;

	char m_string[256];

	DWORD m_line;

	eTokenResult m_lastToken;

	DWORD m_tick;

	char m_LastError[256];
};