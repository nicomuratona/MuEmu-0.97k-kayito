#include "stdafx.h"
#include "MemScript.h"

CMemScript::CMemScript()
{
	this->m_buffer.clear();

	this->m_position = 0;

	memset(this->m_path, 0, sizeof(this->m_path));

	this->SetLastError(4);
}

CMemScript::~CMemScript()
{
	this->m_buffer.clear();

	this->m_position = 0;
}

bool CMemScript::SetBuffer(char* path)
{
	strcpy_s(this->m_path, path);

	std::ifstream file(this->m_path, std::ios::binary | std::ios::ate);

	if (!file.is_open())
	{
		SetLastError(0);

		return false;
	}

	size_t fileSize = (size_t)file.tellg();

	file.seekg(0, std::ios::beg);

	this->m_buffer.resize(fileSize);

	if (!file.read(&this->m_buffer[0], fileSize))
	{
		SetLastError(2);

		return false;
	}

	file.close();

	if (this->m_buffer.size() >= 3 && this->m_buffer[0] == char(0xEF) && this->m_buffer[1] == char(0xBB) && this->m_buffer[2] == char(0xBF))
	{
		this->m_buffer.erase(this->m_buffer.begin(), this->m_buffer.begin() + 3);
	}
	else if (IsEncodingAnsi())
	{
		std::string utf8Content(this->m_buffer.begin(), this->m_buffer.end());

		std::wstring utf16Content;

		utf16Content.resize(MultiByteToWideChar(CP_ACP, 0, utf8Content.c_str(), utf8Content.size(), 0, 0));

		MultiByteToWideChar(CP_ACP, 0, utf8Content.c_str(), utf8Content.size(), &utf16Content[0], utf16Content.size());

		int utf8Size = WideCharToMultiByte(CP_UTF8, 0, utf16Content.c_str(), utf16Content.size(), 0, 0, 0, 0);

		std::string utf8Buffer(utf8Size, '\0');

		WideCharToMultiByte(CP_UTF8, 0, utf16Content.c_str(), utf16Content.size(), &utf8Buffer[0], utf8Buffer.size(), 0, 0);

		this->m_buffer.resize(utf8Buffer.size());

		std::copy(utf8Buffer.begin(), utf8Buffer.end(), this->m_buffer.begin());
	}

	this->m_position = 0;

	this->m_tick = GetTickCount();

	this->m_line = 1;

	return true;
}

bool CMemScript::IsEncodingAnsi()
{
	for (size_t n = 0; n < this->m_buffer.size(); n++)
	{
		if ((this->m_buffer[n] & 0x80) == 0x00)
		{
			continue;
		}
		else if ((this->m_buffer[n] & 0xE0) == 0xC0)
		{
			if ((n + 1) >= this->m_buffer.size() || (this->m_buffer[n + 1] & 0xC0) != 0x80)
			{
				return true;
			}

			n += 1;
		}
		else if ((this->m_buffer[n] & 0xF0) == 0xE0)
		{
			if ((n + 2) >= this->m_buffer.size() || (this->m_buffer[n + 1] & 0xC0) != 0x80 || (this->m_buffer[n + 2] & 0xC0) != 0x80)
			{
				return true;
			}

			n += 2;
		}
		else
		{
			return true;
		}
	}

	return false;
}

eTokenResult CMemScript::GetToken(bool wReturn)
{
	if ((GetTickCount() - this->m_tick) > 2000)
	{
		this->SetLastError(4);

		throw 1;
	}

	this->m_number = 0;

	memset(this->m_string, 0, sizeof(this->m_string));

	char ch;

	while (true)
	{
		if ((ch = this->GetChar()) == -1)
		{
			this->m_lastToken = TOKEN_END;

			return this->m_lastToken;
		}

		if (CheckSpace(ch))
		{
			continue;
		}

		if ((ch = this->CheckComment(ch)) == -1)
		{
			this->m_lastToken = TOKEN_END;

			return this->m_lastToken;
		}
		else if (ch == '\n')
		{
			this->m_line++;

			if (wReturn)
			{
				this->m_lastToken = TOKEN_END_LINE;

				return this->m_lastToken;
			}
		}
		else
		{
			break;
		}
	}

	if (ch == '-' || ch == '.' || ch == '*' || isdigit(ch) != 0)
	{
		return this->GetTokenNumber(ch);
	}

	if (ch == '"')
	{
		return this->GetTokenString(ch);
	}

	return this->GetTokenCommon(ch);
}

char CMemScript::GetChar()
{
	if (this->m_position >= this->m_buffer.size())
	{
		return -1;
	}

	return this->m_buffer[this->m_position++];
}

void CMemScript::UnGetChar(char ch)
{
	if (this->m_position == 0)
	{
		return;
	}

	this->m_buffer[--this->m_position] = ch;
}

char CMemScript::CheckComment(char ch)
{
	if (ch != '/' || (ch = this->GetChar()) != '/')
	{
		return ch;
	}

	while (true)
	{
		if ((ch = this->GetChar()) == -1)
		{
			return ch;
		}

		if (ch == '\n')
		{
			return ch;
		}
	}

	return ch;
}

bool CMemScript::CheckSpace(char ch)
{
	if (ch == ' ' // space
	    || ch == '\t' // horizontal tab
	    || ch == '\v' // vertical tab
	    || ch == '\f' // form feed
	    || ch == '\r') // Carraige return
	{
		return true;
	}

	return false;
}

eTokenResult CMemScript::GetTokenNumber(char ch)
{
	int count = 0;

	this->UnGetChar(ch);

	while ((ch = this->GetChar()) != -1 && (ch == '-' || ch == '.' || ch == '*' || isdigit(ch) != 0))
	{
		this->m_string[count++] = ch;
	}

	if (strcmp(this->m_string, "*") == 0)
	{
		this->m_number = -1;
	}
	else
	{
		this->m_number = (float)atof(this->m_string);
	}

	this->m_string[count] = 0;

	this->m_lastToken = TOKEN_NUMBER;

	return this->m_lastToken;
}

eTokenResult CMemScript::GetTokenString(char ch)
{
	int count = 0;

	while ((ch = this->GetChar()) != -1 && ch != '"' && ch != '\n')
	{
		this->m_string[count++] = ch;
	}

	if (ch != '"')
	{
		this->UnGetChar(ch);
	}

	this->m_string[count] = 0;

	if (ch == -1)
	{
		this->m_lastToken = TOKEN_END;
	}
	else if (ch == '\n')
	{
		this->m_lastToken = TOKEN_END_LINE;
	}
	else
	{
		this->m_lastToken = TOKEN_STRING;
	}

	return this->m_lastToken;
}

eTokenResult CMemScript::GetTokenCommon(char ch)
{
	if (isalpha(ch) == 0)
	{
		this->m_lastToken = TOKEN_ERROR;

		return this->m_lastToken;
	}

	int count = 0;

	this->m_string[count++] = ch;

	while ((ch = this->GetChar()) != -1 && (ch == '.' || ch == '_' || isalnum(ch) != 0))
	{
		this->m_string[count++] = ch;
	}

	this->UnGetChar(ch);

	this->m_string[count] = 0;

	if (strcmp("end", this->m_string) == 0)
	{
		this->m_lastToken = TOKEN_END_SECTION;
	}
	else if (ch == -1)
	{
		this->m_lastToken = TOKEN_END;
	}
	else
	{
		this->m_lastToken = TOKEN_STRING;
	}

	return this->m_lastToken;
}

void CMemScript::SetLastError(int error)
{
	switch (error)
	{
		case 0:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE0, this->m_path);

			break;
		}

		case 1:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE1, this->m_path);

			break;
		}

		case 2:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE2, this->m_path);

			break;
		}

		case 3:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE3, this->m_path);

			break;
		}

		case 4:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE4, this->m_path);

			break;
		}

		case 5:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODE5, this->m_path, this->m_line);

			break;
		}

		default:
		{
			wsprintf(this->m_LastError, MEM_SCRIPT_ERROR_CODEX, this->m_path, error);

			break;
		}
	}
}

char* CMemScript::GetLastError()
{
	return this->m_LastError;
}

int CMemScript::GetNumber()
{
	if (this->m_lastToken != TOKEN_NUMBER)
	{
		this->SetLastError(5);

		throw 1;
	}

	return (int)this->m_number;
}

int CMemScript::GetAsNumber()
{
	eTokenResult token = this->GetToken(true);

	if (token == TOKEN_END_LINE || token != TOKEN_NUMBER)
	{
		this->SetLastError(5);

		throw 1;
	}

	return (int)this->m_number;
}

float CMemScript::GetFloatNumber()
{
	if (this->m_lastToken != TOKEN_NUMBER)
	{
		this->SetLastError(5);

		throw 1;
	}

	return this->m_number;
}

float CMemScript::GetAsFloatNumber()
{
	eTokenResult token = this->GetToken(true);

	if (token == TOKEN_END_LINE || token != TOKEN_NUMBER)
	{
		this->SetLastError(5);

		throw 1;
	}

	return this->m_number;
}

char* CMemScript::GetString()
{
	if (this->m_lastToken != TOKEN_STRING)
	{
		this->SetLastError(5);

		throw 1;
	}

	return this->m_string;
}

char* CMemScript::GetAsString()
{
	eTokenResult token = this->GetToken(true);

	if (token == TOKEN_END_LINE || token != TOKEN_STRING)
	{
		this->SetLastError(5);

		throw 1;
	}

	return this->m_string;
}