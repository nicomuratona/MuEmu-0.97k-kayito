#include "stdafx.h"
#include "QueryManager.h"
#include "Util.h"

CQueryManager gQueryManager;

CQueryManager::CQueryManager()
{
	this->m_SQLEnvironment = SQL_NULL_HANDLE;

	this->m_SQLConnection = SQL_NULL_HANDLE;

	this->m_STMT = SQL_NULL_HANDLE;

	this->m_RowCount = -1;

	this->m_ColCount = -1;

	memset(this->m_SQLColName, 0, sizeof(this->m_SQLColName));

	memset(this->m_SQLData, 0, sizeof(this->m_SQLData));

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->m_SQLEnvironment);

	SQLSetEnvAttr(this->m_SQLEnvironment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
}

CQueryManager::~CQueryManager()
{
	this->Disconnect();
}

bool CQueryManager::Connect(char* odbc, char* user, char* pass)
{
	strcpy_s(this->m_odbc, odbc);

	strcpy_s(this->m_user, user);

	strcpy_s(this->m_pass, pass);

	if (SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, this->m_SQLEnvironment, &this->m_SQLConnection)) == 0)
	{
		return false;
	}

	if (SQL_SUCCEEDED(SQLConnect(this->m_SQLConnection, (SQLCHAR*)this->m_odbc, SQL_NTS, (SQLCHAR*)this->m_user, SQL_NTS, (SQLCHAR*)this->m_pass, SQL_NTS)) == 0)
	{
		return false;
	}

	if (SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, this->m_SQLConnection, &this->m_STMT)) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CQueryManager::Disconnect()
{
	if (this->m_STMT != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, this->m_STMT);

		this->m_STMT = SQL_NULL_HANDLE;
	}

	if (this->m_SQLConnection != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, this->m_SQLConnection);

		this->m_SQLConnection = SQL_NULL_HANDLE;
	}

	if (this->m_SQLEnvironment != SQL_NULL_HANDLE)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, this->m_SQLEnvironment);

		this->m_SQLEnvironment = SQL_NULL_HANDLE;
	}
}

void CQueryManager::Diagnostic(char* query)
{
	LogAdd(LOG_BLACK, "%s", query);

	SQLINTEGER NativeError;

	SQLSMALLINT RecNumber = 1, BufferLength;

	SQLCHAR SqlState[6], MessageText[SQL_MAX_MESSAGE_LENGTH];

	while (SQLGetDiagRec(SQL_HANDLE_STMT, this->m_STMT, (RecNumber++), SqlState, &NativeError, MessageText, sizeof(MessageText), &BufferLength) != SQL_NO_DATA)
	{
		LogAdd(LOG_RED, "[QueryManager] State (%s), Diagnostic: %s", SqlState, MessageText);
	}

	if (strcmp((char*)SqlState, "08S01") == 0)
	{
		this->Connect(this->m_odbc, this->m_user, this->m_pass);
	}
}

bool CQueryManager::ExecQuery(char* query, ...)
{
	char buff[4096];

	va_list arg;

	va_start(arg, query);

	vsprintf_s(buff, query, arg);

	va_end(arg);

	SQLRETURN result;

	if (SQL_SUCCEEDED((result = SQLExecDirect(this->m_STMT, (SQLCHAR*)buff, SQL_NTS))) == 0 && result != SQL_NO_DATA)
	{
		this->Diagnostic(buff);

		return false;
	}

	SQLRowCount(this->m_STMT, &this->m_RowCount);

	if (this->m_RowCount == 0)
	{
		return true;
	}

	SQLNumResultCols(this->m_STMT, &this->m_ColCount);

	if (this->m_ColCount == 0)
	{
		return true;
	}

	if (this->m_ColCount > MAX_COLUMNS)
	{
		return false;
	}

	memset(this->m_SQLColName, 0, sizeof(this->m_SQLColName));

	memset(this->m_SQLData, 0, sizeof(this->m_SQLData));

	for (int n = 0; n < this->m_ColCount; n++)
	{
		SQLDescribeCol(this->m_STMT, (n + 1), this->m_SQLColName[n], sizeof(this->m_SQLColName[n]), 0, 0, 0, 0, 0);

		SQLBindCol(this->m_STMT, (n + 1), SQL_C_CHAR, this->m_SQLData[n], sizeof(this->m_SQLData[n]), &this->m_SQLDataLen[n]);
	}

	return true;
}

void CQueryManager::Close()
{
	SQLCloseCursor(this->m_STMT);

	SQLFreeStmt(this->m_STMT, SQL_UNBIND);
}

SQLRETURN CQueryManager::Fetch()
{
	memset(this->m_SQLData, 0, sizeof(this->m_SQLData));

	return SQLFetch(this->m_STMT);
}

int CQueryManager::FindIndex(char* ColName)
{
	for (int n = 0; n < this->m_ColCount; n++)
	{
		if (_stricmp(ColName, (char*)this->m_SQLColName[n]) == 0)
		{
			return n;
		}
	}

	return -1;
}

int CQueryManager::GetResult(int index)
{
	return atoi(this->m_SQLData[index]);
}

int CQueryManager::GetAsInteger(char* ColName)
{
	int index = this->FindIndex(ColName);

	if (index == -1)
	{
		return index;
	}
	else
	{
		return atoi(this->m_SQLData[index]);
	}
}

float CQueryManager::GetAsFloat(char* ColName)
{
	int index = this->FindIndex(ColName);

	if (index == -1)
	{
		return (float)index;
	}
	else
	{
		return (float)atof(this->m_SQLData[index]);
	}
}

__int64 CQueryManager::GetAsInteger64(char* ColName)
{
	int index = this->FindIndex(ColName);

	if (index == -1)
	{
		return index;
	}
	else
	{
		return _atoi64(this->m_SQLData[index]);
	}
}

void CQueryManager::GetAsString(char* ColName, char* OutBuffer, int OutBufferSize)
{
	int index = this->FindIndex(ColName);

	if (index == -1)
	{
		memset(OutBuffer, 0, OutBufferSize);
	}
	else
	{
		strncpy_s(OutBuffer, OutBufferSize, this->m_SQLData[index], (OutBufferSize - 1));
	}
}

void CQueryManager::GetAsBinary(char* ColName, BYTE* OutBuffer, int OutBufferSize)
{
	int index = this->FindIndex(ColName);

	if (index == -1)
	{
		memset(OutBuffer, 0, OutBufferSize);
	}
	else
	{
		this->ConvertStringToBinary(this->m_SQLData[index], sizeof(this->m_SQLData[index]), OutBuffer, OutBufferSize);
	}
}

void CQueryManager::BindParameterAsString(int ParamNumber, void* InBuffer, int ColumnSize)
{
	this->m_SQLBindValue[(ParamNumber - 1)] = SQL_NTS;

	SQLBindParameter(this->m_STMT, ParamNumber, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, ColumnSize, 0, InBuffer, 0, &this->m_SQLBindValue[(ParamNumber - 1)]);
}

void CQueryManager::BindParameterAsBinary(int ParamNumber, void* InBuffer, int ColumnSize)
{
	this->m_SQLBindValue[(ParamNumber - 1)] = ColumnSize;

	SQLBindParameter(this->m_STMT, ParamNumber, SQL_PARAM_INPUT, SQL_C_BINARY, SQL_VARBINARY, ColumnSize, 0, InBuffer, 0, &this->m_SQLBindValue[(ParamNumber - 1)]);
}

void CQueryManager::ConvertStringToBinary(char* InBuff, int InSize, BYTE* OutBuff, int OutSize)
{
	int size = 0;

	memset(OutBuff, 0, OutSize);

	for (int n = 0; n < InSize, size < OutSize; n++)
	{
		if (InBuff[n] == 0)
		{
			break;
		}

		if ((n % 2) == 0)
		{
			OutBuff[size] = ((InBuff[n] >= 'A') ? ((InBuff[n] - 'A') + 10) : (InBuff[n] - '0')) * 16;

			size = size + 0;
		}
		else
		{
			OutBuff[size] = OutBuff[size] | ((InBuff[n] >= 'A') ? ((InBuff[n] - 'A') + 10) : (InBuff[n] - '0'));

			size = size + 1;
		}
	}
}

void CQueryManager::ConvertBinaryToString(BYTE* InBuff, int InSize, char* OutBuff, int OutSize)
{
	int size = 0;

	memset(OutBuff, 0, OutSize);

	for (int n = 0; n < OutSize, size < InSize; n++)
	{
		if ((n % 2) == 0)
		{
			OutBuff[n] = (((InBuff[size] / 16) >= 10) ? ('A' + ((InBuff[size] / 16) - 10)) : ('0' + (InBuff[size] / 16)));

			size = size + 0;
		}
		else
		{
			OutBuff[n] = (((InBuff[size] % 16) >= 10) ? ('A' + ((InBuff[size] % 16) - 10)) : ('0' + (InBuff[size] % 16)));

			size = size + 1;
		}
	}
}