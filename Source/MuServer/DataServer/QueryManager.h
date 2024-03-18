#pragma once

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

	bool Connect(std::string Server, std::string Username, std::string Password, std::string SchemaName);

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

	std::string Server;
	std::string Username;
	std::string Password;
	std::string SchemaName;

	sql::Driver* driver;

	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* pstmt;
	sql::ResultSet* res;

	std::map<int, std::stringstream*> m_BinaryBuffs;
	std::map<int, std::string> m_StringBuffs;
};

extern CQueryManager gQueryManager;