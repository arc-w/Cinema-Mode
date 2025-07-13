#include "header.h"

std::set<DWORD> pids;

BOOL CALLBACK EnumProcs(HWND hwnd, LPARAM lparam)
{
	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hName = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
	if (hName)
	{
		TCHAR processName[MAX_PATH] = TEXT("unluck");
		HMODULE hMod;
		DWORD cbNeeded;
		if (EnumProcessModules(hName, &hMod, sizeof(hMod), &cbNeeded))
		{
			GetModuleBaseName(hName, hMod, processName, sizeof(processName) / sizeof(TCHAR));
		}
		if (_tcscmp(processName, TEXT("wallpaper32.exe")) == 0 || _tcscmp(processName, TEXT("wallpaperservice32.exe")) == 0) // optional
		{
			pids.insert(pid);
		}
		if (_tcscmp(processName, TEXT("explorer.exe")) == 0 || _tcscmp(processName, TEXT("explorer.EXE")) == 0)
		{
			return TRUE;
		}
		if (_tcscmp(processName, TEXT("WindowsTerminal.exe")) == 0 || _tcscmp(processName, TEXT("VsDebugConsole.exe")) == 0) // terminal
		{
			return TRUE;
		}
	}
	pid = 0;
	CloseHandle(hName);
	if (!IsWindowVisible(hwnd))
	{
		return TRUE;
	}
	if (GetWindowTextLength(hwnd) == 0)
	{
		return TRUE;
	}
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_TOOLWINDOW)
		return TRUE;

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	if (style & WS_CHILD)
		return TRUE;

	GetWindowThreadProcessId(hwnd, &pid);
	if (pid != 0)
	{
		pids.insert(pid);
	}
	return TRUE;
}

void CloseProcs()
{
	pids.clear();
	EnumWindows(EnumProcs, 0);
	for (DWORD for_pid : pids)
	{
		if (for_pid != 17976) // visual studio
		{
			HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, for_pid);
			if (hProc)
			{
				TerminateProcess(hProc, 0);
				CloseHandle(hProc);
			}
		}
	}
}