#include "header.h"

void CinemaMode(CString deviceName, std::string url, std::string browserPath)
{
	CloseProcs();
	for (int i = 0; i < browserPath.size(); i++)
	{
		if (browserPath.at(i) == '\\')
		{
			browserPath.at(i) = '/';
		}
	}
	get_browser_exe(browserPath);
	do
	{
		ShellExecuteA(nullptr, "open", browserPath.c_str(), url.c_str(), nullptr, SW_SHOWNORMAL);
		std::chrono::seconds timespan(1);
		std::this_thread::sleep_for(timespan);
		EnumWindows(EnumBrowser, 0);
	} while (!isBrowserOpen);
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = VK_F11;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
	InitDefaultAudioDevice(deviceName);
}