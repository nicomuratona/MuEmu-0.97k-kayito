#include "stdafx.h"
#include "QueryManager.h"
#include "Log.h"
#include "Util.h"

#pragma comment(lib,"mysqlcppconn.lib")

CQueryManager gQueryManager;

CQueryManager::CQueryManager()
{
	this->driver = NULL;

	this->con = NULL;

	this->connection_properties.clear();

	this->stmt = NULL;

	this->pstmt = NULL;

	this->res = NULL;

	this->m_StringBuffs.clear();

	this->m_BinaryBuffs.clear();
}

CQueryManager::~CQueryManager()
{
	this->Disconnect();
}

bool CQueryManager::Init(std::string HostName, WORD HostPort, std::string Username, std::string Password, std::string SchemaName)
{
	this->connection_properties["hostName"] = HostName;

	this->connection_properties["port"] = HostPort;

	this->connection_properties["userName"] = Username;

	this->connection_properties["password"] = Password;

	this->connection_properties["schema"] = SchemaName;

	this->connection_properties["OPT_RECONNECT"] = true;

	return this->Connect();
}

bool CQueryManager::Connect()
{
	try
	{
		this->driver = get_driver_instance();

		this->con = this->driver->connect(this->connection_properties);
	}
	catch (sql::SQLException e)
	{
		LogAdd(LOG_RED, "[QueryManager] Connection to DataBase failed: %s", e.what());

		return false;
	}

	return true;
}

void CQueryManager::Disconnect()
{
	this->driver = NULL;

	if (this->con)
	{
		delete this->con;

		this->con = NULL;
	}

	this->connection_properties.clear();

	this->Close();
}

void CQueryManager::PrepareQuery(std::string query, ...)
{
	char buff[4096];

	va_list arg;

	va_start(arg, query);

	vsprintf_s(buff, query.c_str(), arg);

	va_end(arg);

	this->pstmt = this->con->prepareStatement(buff);

	gLog.Output(LOG_QUERY, "[PrepareQuery] Query prepared: %s", buff);
}

bool CQueryManager::ExecPreparedUpdateQuery()
{
	try
	{
		int Quant = this->pstmt->executeUpdate();

		gLog.Output(LOG_QUERY, "[ExecPreparedUpdateQuery] %d rows updated", Quant);
	}
	catch (sql::SQLException& e)
	{
		this->Diagnostic(e);

		return false;
	}
	catch (...)
	{
		LogAdd(LOG_RED, "[ExecPreparedUpdateQuery] Exception thrown");

		return false;
	}

	return true;
}

bool CQueryManager::ExecPreparedResultQuery()
{
	try
	{
		if (this->pstmt->execute())
		{
			this->res = this->pstmt->getResultSet();

			gLog.Output(LOG_QUERY, "[ExecPreparedResultQuery] Query with results.");
		}
		else
		{
			gLog.Output(LOG_QUERY, "[ExecPreparedResultQuery] No results available.");

			return false;
		}
	}
	catch (sql::SQLException& e)
	{
		this->Diagnostic(e);

		return false;
	}
	catch (...)
	{
		LogAdd(LOG_RED, "[ExecPreparedResultQuery] Exception thrown");

		return false;
	}

	return true;
}

bool CQueryManager::ExecUpdateQuery(std::string query, ...)
{
	char buff[4096];

	va_list arg;

	va_start(arg, query);

	vsprintf_s(buff, query.c_str(), arg);

	va_end(arg);

	try
	{
		this->stmt = this->con->createStatement();

		int Quant = this->stmt->executeUpdate(buff);

		gLog.Output(LOG_QUERY, "[ExecUpdateQuery] %d rows updated. Query: %s", Quant, buff);
	}
	catch (sql::SQLException& e)
	{
		this->Diagnostic(e, buff);

		return false;
	}
	catch (...)
	{
		LogAdd(LOG_RED, "[ExecUpdateQuery] Exception thrown");

		return false;
	}

	return true;
}

bool CQueryManager::ExecResultQuery(std::string query, ...)
{
	char buff[4096];

	va_list arg;

	va_start(arg, query);

	vsprintf_s(buff, query.c_str(), arg);

	va_end(arg);

	try
	{
		this->stmt = this->con->createStatement();

		if (this->stmt->execute(buff))
		{
			this->res = this->stmt->getResultSet();

			gLog.Output(LOG_QUERY, "[ExecResultQuery] Query with results. Query: %s", buff);
		}
		else
		{
			gLog.Output(LOG_QUERY, "[ExecResultQuery] No results available. Query: %s", buff);

			return false;
		}
	}
	catch (sql::SQLException& e)
	{
		this->Diagnostic(e, buff);

		return false;
	}
	catch (...)
	{
		LogAdd(LOG_RED, "[ExecResultQuery] Exception thrown");

		return false;
	}

	return true;
}

bool CQueryManager::ExecQuery(std::string query, ...)
{
	char buff[4096];

	va_list arg;

	va_start(arg, query);

	vsprintf_s(buff, query.c_str(), arg);

	va_end(arg);

	try
	{
		this->stmt = this->con->createStatement();

		if (this->stmt->execute(buff))
		{
			this->res = this->stmt->getResultSet();

			gLog.Output(LOG_QUERY, "[ExecQuery] Query with results. Query: %s", buff);
		}
		else
		{
			__int64 Quant = this->stmt->getUpdateCount();

			gLog.Output(LOG_QUERY, "[ExecQuery] %d rows updated. Query: %s", Quant, buff);

			return (Quant > 0);
		}
	}
	catch (sql::SQLException& e)
	{
		this->Diagnostic(e, buff);

		return false;
	}
	catch (...)
	{
		LogAdd(LOG_RED, "[ExecQuery] Exception thrown");

		return false;
	}

	return true;
}

void CQueryManager::Diagnostic(sql::SQLException& e, char* Query)
{
	if (Query)
	{
		LogAdd(LOG_BLUE, "%s", Query);
	}

	std::string SqlState = e.getSQLState();

	LogAdd(LOG_RED, "[QueryManager] State (%s), Diagnostic: %s", SqlState.c_str(), e.what());

	if (SqlState.compare("HY000") == 0)
	{
		this->Connect();
	}
}

void CQueryManager::Close()
{
	if (this->res)
	{
		delete this->res;

		this->res = NULL;
	}

	if (this->stmt)
	{
		while (this->stmt->getMoreResults());

		delete this->stmt;

		this->stmt = NULL;
	}

	if (this->pstmt)
	{
		while (this->pstmt->getMoreResults());

		delete this->pstmt;

		this->pstmt = NULL;
	}

	this->m_StringBuffs.clear();

	for (std::map<int, std::stringstream*>::iterator itr = this->m_BinaryBuffs.begin(); itr != this->m_BinaryBuffs.end(); itr++)
	{
		delete itr->second;
	}

	this->m_BinaryBuffs.clear();
}

bool CQueryManager::Fetch()
{
	return this->res->next();
}

void CQueryManager::SetAsInteger(int position, int value)
{
	this->pstmt->setInt(position, value);
}

void CQueryManager::SetAsFloat(int position, float value)
{
	this->pstmt->setDouble(position, (double)value);
}

void CQueryManager::SetAsInteger64(int position, __int64 value)
{
	this->pstmt->setInt64(position, value);
}

void CQueryManager::SetAsString(int position, char* InBuffer, int InBufferSize)
{
	this->m_StringBuffs[position] = InBuffer;

	if (this->m_StringBuffs[position].length() > (unsigned int)InBufferSize)
	{
		this->m_StringBuffs[position].resize(InBufferSize);
	}

	this->pstmt->setString(position, this->m_StringBuffs[position]);
}

void CQueryManager::SetAsBinary(int position, unsigned char* InBuffer, int InBufferSize)
{
	std::stringstream* buff = new std::stringstream;

	buff->write((const char*)InBuffer, InBufferSize);

	this->m_BinaryBuffs[position] = buff;

	this->pstmt->setBlob(position, this->m_BinaryBuffs[position]);
}

int CQueryManager::GetAsInteger(std::string ColName)
{
	return this->res->getInt(ColName);
}

float CQueryManager::GetAsFloat(std::string ColName)
{
	return (float)this->res->getDouble(ColName);
}

__int64 CQueryManager::GetAsInteger64(std::string ColName)
{
	return this->res->getInt64(ColName);
}

void CQueryManager::GetAsString(std::string ColName, char* OutBuffer, int OutBufferSize)
{
	strncpy_s(OutBuffer, OutBufferSize, this->res->getString(ColName).c_str(), (OutBufferSize - 1));
}

void CQueryManager::GetAsBinary(std::string ColName, unsigned char* OutBuffer, int OutBufferSize)
{
	char* result = new char[OutBufferSize];

	this->res->getBlob(ColName)->read(result, OutBufferSize);

	memcpy(OutBuffer, result, OutBufferSize);

	delete result;
}

void CQueryManager::ConvertStringToBinary(char* InBuff, int InSize, unsigned char* OutBuff, int OutSize)
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

void CQueryManager::ConvertBinaryToString(unsigned char* InBuff, int InSize, char* OutBuff, int OutSize)
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