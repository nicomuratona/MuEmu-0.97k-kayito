#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_VISTA

#define GAMESERVER_EXTRA 1

#define MAX_LANGUAGE 3

#define ENCRYPT_STATE 1

// System includes
#include <windows.h>
#include <dbghelp.h>
#include <winsock2.h>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <random>
#include <time.h>
#include <atltime.h>
#include <WS2tcpip.h>
#include <vector>
#include <fstream>

// General includes
#include "Console.h"

// Libraries
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"dbghelp.lib")

typedef unsigned __int64 QWORD;