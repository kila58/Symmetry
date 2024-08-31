/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     * 
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

// Definitions
#define WIN32_LEAN_AND_MEAN

// Headers
#if defined(__cplusplus)
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <regex>
#include <sstream>
#include <fstream>
#include <iomanip>

// Namespaces
using namespace std;
#endif

#include <Windows.h>

#include <TlHelp32.h>
#include <winsock2.h>
#include <WS2tcpip.h>

// CRT
#if defined(__cplusplus) 
extern "C" {
#endif
	BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
#if defined(__cplusplus) 
}
#endif