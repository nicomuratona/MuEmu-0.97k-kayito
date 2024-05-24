#pragma once

#ifndef MYSQL

#define MAX_COLUMNS 100

class CQueryManager
{
public:

	CQueryManager();

	~CQueryManager();

	bool Connect(char* odbc, char* user, char* pass);

	void Disconnect();

	void Diagnostic(char* query);

	bool ExecQuery(char* query, ...);

	void Close();

	SQLRETURN Fetch();

	int FindIndex(char* ColName);

	int GetResult(int index);

	int GetAsInteger(char* ColName);

	float GetAsFloat(char* ColName);

	__int64 GetAsInteger64(char* ColName);

	void GetAsString(char* ColName, char* OutBuffer, int OutBufferSize);

	void GetAsBinary(char* ColName, BYTE* OutBuffer, int OutBufferSize);

	void BindParameterAsString(int ParamNumber, void* InBuffer, int ColumnSize);

	void BindParameterAsBinary(int ParamNumber, void* InBuffer, int ColumnSize);

	void ConvertStringToBinary(char* InBuff, int InSize, BYTE* OutBuff, int OutSize);

	void ConvertBinaryToString(BYTE* InBuff, int InSize, char* OutBuff, int OutSize);

private:

	SQLHANDLE m_SQLEnvironment;

	SQLHANDLE m_SQLConnection;

	SQLHANDLE m_STMT;

	char m_odbc[32];

	char m_user[32];

	char m_pass[32];

	SQLINTEGER m_RowCount;

	SQLSMALLINT m_ColCount;

	SQLCHAR m_SQLColName[MAX_COLUMNS][30];

	char m_SQLData[MAX_COLUMNS][8192];

	SQLINTEGER m_SQLDataLen[MAX_COLUMNS];

	SQLINTEGER m_SQLBindValue[MAX_COLUMNS];
};

extern CQueryManager gQueryManager;

#else

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

class CQueryManager
{
	struct membuf : std::streambuf
	{
		membuf(char* begin, char* end)
		{
			this->setg(begin, begin, end);
		}
	};

public:

	CQueryManager();

	virtual ~CQueryManager();

	bool Init(std::string HostName, WORD HostPort, std::string Username, std::string Password, std::string SchemaName);

	bool Connect();

	void Disconnect();

	void PrepareQuery(std::string query, ...);

	bool ExecPreparedUpdateQuery();

	bool ExecPreparedResultQuery();

	bool ExecUpdateQuery(std::string query, ...);

	bool ExecResultQuery(std::string query, ...);

	bool ExecQuery(std::string query, ...);

	void Close();

	bool Fetch();

	void SetAsInteger(int position, int value);

	void SetAsFloat(int position, float value);

	void SetAsInteger64(int position, __int64 value);

	void SetAsString(int position, char* InBuffer, int InBufferSize);

	void SetAsBinary(int position, unsigned char* InBuffer, int InBufferSize);

	int GetAsInteger(std::string ColName);

	float GetAsFloat(std::string ColName);

	__int64 GetAsInteger64(std::string ColName);

	void GetAsString(std::string ColName, char* OutBuffer, int OutBufferSize);

	void GetAsBinary(std::string ColName, unsigned char* OutBuffer, int OutBufferSize);

private:

	void Diagnostic(sql::SQLException& e, char* Query = NULL);

	void ConvertStringToBinary(char* InBuff, int InSize, unsigned char* OutBuff, int OutSize);

	void ConvertBinaryToString(unsigned char* InBuff, int InSize, char* OutBuff, int OutSize);

private:

	sql::Driver* driver;

	sql::ConnectOptionsMap connection_properties;

	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* res;

	std::map<int, std::stringstream*> m_BinaryBuffs;
	std::map<int, std::string> m_StringBuffs;
};

extern CQueryManager gQueryManager;

#endif