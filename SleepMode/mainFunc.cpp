#include "header.h"

void CinemaMode(CString deviceName, std::string url, std::string browserPath)
{
	CloseProcs(); // closes processes
	// (double-)checks if browser path contains any backslashes (we cant use them)
	for (int i = 0; i < browserPath.size(); i++)
	{
		if (browserPath.at(i) == '\\')
		{
			browserPath.at(i) = '/';
		}
	}
	get_browser_exe(browserPath); // gets browser process name from given browser path
	// tries to open browser and website, waits 1 second and checks if browser was opened
	do
	{
		ShellExecuteA(nullptr, "open", browserPath.c_str(), url.c_str(), nullptr, SW_SHOWNORMAL); // open browser and website
		std::chrono::seconds timespan(1);
		std::this_thread::sleep_for(timespan); // waits for 1 second
		EnumWindows(EnumBrowser, 0); // checks all processes name, and if any has browser`s process name - continue
	} while (!isBrowserOpen); // if browser didnt open - try again
	// enters fullscreen mode (via pressing F11) (when any app opens, system sets focus to it, so pressing F11 will put it in fullscreen mode)
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	ip.ki.wVk = VK_F11;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT)); // press F11
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT)); // release F11
	InitDefaultAudioDevice(deviceName); // sets default audio playback device to, chosen, device
}