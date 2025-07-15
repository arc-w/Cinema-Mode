#include "header.h"

std::set<DWORD> pids;

BOOL CALLBACK EnumProcs(HWND hwnd, LPARAM lparam)
{
	DWORD pid = 0; // stores current process PID
	GetWindowThreadProcessId(hwnd, &pid); // gets current process PID
	// gets info from current process
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
		if (_tcscmp(processName, TEXT("wallpaper32.exe")) == 0 || _tcscmp(processName, TEXT("wallpaperservice32.exe")) == 0) // checks is current process belongs to WallPaper Engine (OPTIONAL), adds to PIDs list
		{
			pids.insert(pid);
		}
		if (_tcscmp(processName, TEXT("WindowsTerminal.exe")) == 0 || _tcscmp(processName, TEXT("VsDebugConsole.exe")) == 0) // checks is current process belongs to Terminal / Visual Studio(OPTIONAL), skips them
		{
			return TRUE;
		}
	}
	CloseHandle(hName);
	// filter all processes
	if (!IsWindowVisible(hwnd)) // checks if it has visible window, if false - skips
	{
		return TRUE;
	}
	if (GetWindowTextLength(hwnd) == 0) // checks if this window has name, if no - skips
	{
		return TRUE;
	}
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_TOOLWINDOW) // checks if this window is tool window (some critical for Windows(OS) processes has visible windows parameter true, but its not visible for user), if true skip
	{
		return TRUE;
	}

	LONG style = GetWindowLong(hwnd, GWL_STYLE); 
	if (style & WS_CHILD) // checks is this child window (some critical for Windows(OS) processes has this marked as true)
	{
		return TRUE;
	}
	if (pid != 0)
	{
		pids.insert(pid); // if this process passed all filters, add it to the list of processes to be closed
	}
	return TRUE;
}

void CloseProcs()
{
	pids.clear(); // clear pids in case smth was left here
	EnumWindows(EnumProcs, 0); // get list of all processes to be closed
	// first, try to SOFT close every listed process
	for (DWORD pid : pids)
	{
		// to ask process to close, we need to access it directly 
		EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL {
			DWORD wndPid = 0;
			GetWindowThreadProcessId(hwnd, &wndPid);
			if (wndPid == (DWORD)lparam)
			{
				LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
				LONG style = GetWindowLong(hwnd, GWL_STYLE);
				if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0 && exStyle & WS_EX_TOOLWINDOW && style & WS_CHILD) // filters processes same as EnumProcs
					PostMessage(hwnd, WM_CLOSE, 0, 0); // gives process message WM_CLOSE (asks process to close)
			}
			return TRUE;
			}, (LPARAM)pid);
	}

	std::chrono::seconds timespan(1);
	std::this_thread::sleep_for(timespan); // waits for 1 sec for processes to soft close

	for (DWORD for_pid : pids) // terminates all processes left
	{
			HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, for_pid);
			if (hProc)
			{
				TerminateProcess(hProc, 0);
				CloseHandle(hProc);
			}
	}
}