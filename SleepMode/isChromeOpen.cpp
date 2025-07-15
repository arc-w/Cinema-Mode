#include "header.h"

bool isChromeOpen = false;

BOOL CALLBACK EnumChrome(HWND hwnd, LPARAM lparam)
{
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hName = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hName)
	{
		TCHAR processName[MAX_PATH] = TEXT("unkown");
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hName, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hName, hMod, processName, sizeof(processName) / sizeof(TCHAR));
		}
		if (_tcscmp(processName, TEXT("chrome.exe")) == 0)
		{
			isChromeOpen = true;
			return TRUE;
		}
	}
	CloseHandle(hName);
	return TRUE;
}