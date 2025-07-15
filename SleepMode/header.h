#pragma once
#pragma comment(lib, "Winmm.lib")
#include <iostream>
#include <windows.h>
#include <vector>
#include <set>
#include <Psapi.h>
#include <tchar.h>
#include <chrono>
#include <thread>
#include <mmsystem.h>
#include <atlstr.h>
#include <string>
#include <fstream>
#include "Mmdeviceapi.h"
#include "PolicyConfig.h"
#include "Propidl.h"
#include "Functiondiscoverykeys_devpkey.h"

extern std::set<DWORD> pids;
extern bool isBrowserOpen;
extern std::string browser_exe;
BOOL CALLBACK EnumProcs(HWND hwnd, LPARAM lparam);
BOOL CALLBACK EnumBrowser(HWND hwnd, LPARAM lparam);
HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID);
std::vector<CString> CoutDefaultAudioDevices();
void CloseProcs();
void enumDevices();
void InitDefaultAudioDevice(CString device);
void CinemaMode(CString deviceName, std::string url, std::string browserPath);
bool IsExplorerProcess(DWORD pid);
void get_browser_exe(std::string browserPath);
std::wstring to_wide(const std::string& multi);