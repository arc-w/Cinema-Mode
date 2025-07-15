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
	std::string temp_browser_exe = "";
	for (int i = 0; i < browserPath.size(); i++)
	{
		if (!(browserPath.at(i) == '/'))
		{
			temp_browser_exe += browserPath.at(i);
		}
		else
		{
			temp_browser_exe.clear();
		}
	}
	browser_exe = temp_browser_exe;
}

BOOL CALLBACK EnumBrowser(HWND hwnd, LPARAM lparam)
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
		if (_tcscmp(processName, to_wide(browser_exe).c_str()) == 0)
		{
			isBrowserOpen = true;
			return TRUE;
		}
	}
	CloseHandle(hName);
	return TRUE;
}