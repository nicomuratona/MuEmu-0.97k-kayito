#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

// System includes
#include <windows.h>
#include <dbghelp.h>
#include <winsock2.h>
#include <queue>
#include <map>
#include <iostream>
#include <string>

// General includes
#include "Console.h"

// Libraries
#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"ws2_32.lib")

extern long MaxIpConnection;