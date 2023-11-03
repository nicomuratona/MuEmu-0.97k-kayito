#pragma once

#define WIN32_LEAN_AND_MEAN

// System Include
#include <windows.h>
#include <winsock2.h>
#include <MMSystem.h>
#include <math.h>
#include <vector>
#include <map>
#include <cstdio>
#include <iostream>
#include <time.h>
#include <Shellapi.h>
#include <Rpc.h>
#include <gl\GL.h>
#include <dsound.h>

// General Includes
#include "Console.h"
#include "Offsets.h"
#include "Defines.h"
#include "Enums.h"
#include "Structs.h"
#include "MathLib.h"
#include "Util.h"
#include "stb.h"

// Libraries
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"Opengl32.lib")