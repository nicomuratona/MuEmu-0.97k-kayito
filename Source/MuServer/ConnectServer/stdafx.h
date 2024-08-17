#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_VISTA

// System includes
#include <windows.h>
#include <dbghelp.h>
#include <winsock2.h>
#include <queue>
#include <map>
#include <time.h>
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#include <fstream>

// General includes
#include "Console.h"

// Libraries
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"ws2_32.lib")

extern long MaxIpConnection;