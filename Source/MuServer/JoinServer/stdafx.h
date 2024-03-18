#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_VISTA

// System includes
#include <windows.h>
#include <dbghelp.h>
#include <winsock2.h>
#include <queue>
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <sql.h>
#include <sqlext.h>
#include <assert.h>
#include <vector>
#include <fstream>
#include <WS2tcpip.h>

// General includes
#include "Console.h"

// Libraries
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"ws2_32.lib")

extern BOOL CaseSensitive;
extern BOOL MD5Encryption;
extern char GlobalPassword[11];