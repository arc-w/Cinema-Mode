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
#include "PolicyConfig.h" // gives an ability to change default audio devices
#include "Propidl.h"
#include "Functiondiscoverykeys_devpkey.h"

extern std::set<DWORD> pids; // stores opened visible processes PIDs (those are to be closed)
extern bool isBrowserOpen;
extern std::string browser_exe; // stores browser`s process name
BOOL CALLBACK EnumProcs(HWND hwnd, LPARAM lparam); // finds all processes, that have visible for user windows
BOOL CALLBACK EnumBrowser(HWND hwnd, LPARAM lparam); // checks did browser successfuly open
HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID); // sets Default Audio Playback Device (raw, used in main device setter InitDefaultAudioDevice)
std::vector<CString> CoutDefaultAudioDevices(); // prints all available Audio Playback Devices
void CloseProcs(); // closes all processes, firstly via WM_CLOSE (soft close), then terminates left processes (force close)
void enumDevices(); // finds all available Audio Playback Devices
void InitDefaultAudioDevice(CString device); // sets Default Audio Playback Device (adapted for this code)
void CinemaMode(CString deviceName, std::string url, std::string browserPath); // main function, closes all processes, opens browser and website, sets fullscreen and changes default output audio device
void get_browser_exe(std::string browserPath); // gets browser process name from path to browser
std::wstring to_wide(const std::string& multi); // converts string to wide string