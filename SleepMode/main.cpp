#include "header.h"


int main()
{
	CloseProcs();
	do
	{
		ShellExecute(nullptr, L"open", L"C:/Program Files/Google/Chrome/Application/chrome.exe", L"https://youtube.com", nullptr, SW_SHOWNORMAL);
		std::chrono::seconds timespan(2);
		std::this_thread::sleep_for(timespan);
		EnumWindows(EnumChrome, 0);
	} while (!isChromeOpen);
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
	return 0;
}