#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <set>
#include <Psapi.h>
#include <tchar.h>
#include <chrono>
#include <thread>

extern std::set<DWORD> pids;
extern bool isChromeOpen;
BOOL CALLBACK EnumProcs(HWND hwnd, LPARAM lparam);
BOOL CALLBACK EnumChrome(HWND hwnd, LPARAM lparam);
void CloseProcs();