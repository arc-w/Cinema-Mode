#include "header.h"

int main()
{
	std::vector<CString> devicesList = CoutDefaultAudioDevices();
	std::cout << "Choose device (0,1,2...): ";
	int chooseDevice;
	std::cin >> chooseDevice;
	InitDefaultAudioDevice(devicesList.at(chooseDevice));
	return 0;
}