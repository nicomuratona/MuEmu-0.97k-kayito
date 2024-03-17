#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_VISTA

// System includes
#include <windows.h>
#include <dbghelp.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <sql.h>
#include <sqlext.h>
#include <mbstring.h>
#include <WS2tcpip.h>
#include <vector>
#include <fstream>

// General includes
#include "Console.h"

// Libraries
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"ws2_32.lib")