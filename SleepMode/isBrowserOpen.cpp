#include "header.h"

bool isBrowserOpen = false;
std::string browser_exe = "";

std::wstring to_wide(const std::string& multi) {
	std::wstring wide; wchar_t w; mbstate_t mb{};
	size_t n = 0, len = multi.length() + 1;
	while (auto res = mbrtowc(&w, multi.c_str() + n, len - n, &mb)) {
		if (res == size_t(-1) || res == size_t(-2))
			throw "invalid encoding";

		n += res;
		wide += w;
	}
	return wide;
}

void get_browser_exe(std::string browserPath)
{
	std::string temp_browser_exe = ""; // stores temporary text but at the end gets a browser process name
	for (int i = 0; i < browserPath.size(); i++)
	{
		if (!(browserPath.at(i) == '/'))
		{
			temp_browser_exe += browserPath.at(i); // if current symbol is not slash - save letter
		}
		else
		{
			temp_browser_exe.clear(); // if it is - clear temp_browser_exe
		}
	}
	browser_exe = temp_browser_exe; // gives a global variable browser_exe browser`s process name
}

BOOL CALLBACK EnumBrowser(HWND hwnd, LPARAM lparam)
{
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hName = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hName)
	{
		TCHAR processName[MAX_PATH] = TEXT("unknown");
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hName, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hName, hMod, processName, sizeof(processName) / sizeof(TCHAR)); // gets name of process
		}
		if (_tcscmp(processName, to_wide(browser_exe).c_str()) == 0) // converts browser process name (string) to wchar_t, to compare it to current process name
		{
			isBrowserOpen = true; // if process name is same as stored globally, marks that browser has been successfuly opened
			return TRUE;
		}
	}
	CloseHandle(hName);
	return TRUE;
}