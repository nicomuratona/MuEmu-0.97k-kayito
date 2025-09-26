#pragma once

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_VISTA

// System Include
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <MMSystem.h>
#include <math.h>
#include <vector>
#include <set>
#include <map>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <Shellapi.h>
#include <Rpc.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <dsound.h>
#include <mbstring.h>
#include <fstream>
#include <deque>
#include <chrono>

// General Includes
#include "Console.h"
#include "MathLib.h"
#include "Defines.h"
#include "Enums.h"
#include "Structs.h"
#include "Offsets.h"
#include "Util.h"
#include "OpenglUtil.h"
#include "stb.h"

// Libraries
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib,"version.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"Opengl32.lib")
#pragma comment(lib,"glu32.lib")