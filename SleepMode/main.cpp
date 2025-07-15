#include "header.h"

int main()
{
	int chooseDevice;
	CString deviceName;
	std::string url;
	std::string browserPath;
	std::ifstream read;
	char auto_close;
	read.open("CinemaMode_arc-w.txt");
	if (read.is_open())
	{
		std::string settingsFromFile;
		getline(read, settingsFromFile);
		if (settingsFromFile == "1")
		{
			std::cout << "Cinema mode activate" << std::endl;
			for (int i = 0; i < 3; i++)
			{
				settingsFromFile.clear();
				getline(read, settingsFromFile);
				switch (i)
				{
				case 0:
					deviceName = settingsFromFile.c_str();
					break;
				case 1:
					url = settingsFromFile;
					break;
				case 2:
					browserPath = settingsFromFile;
					break;
				default:
					break;
				}
			}
			CinemaMode(deviceName, url, browserPath);
			return 0;
		}
		read.close();
	}
	std::cout << "Welcome to Cinema Mode. Enter your preferences." << std::endl << std::endl;
	std::vector<CString> devicesList = CoutDefaultAudioDevices();
	do
	{
		std::cout << std::endl << "Choose which output device will be automatically set as default (0,1,2...): ";
		std::cin >> chooseDevice;
	} while (chooseDevice < 0 || chooseDevice >= devicesList.size());
	CT2CA pszConvertedAnsiString(devicesList.at(chooseDevice));
	std::string deviceNameStr(pszConvertedAnsiString);
	std::cout << "Enter website url: ";
	std::cin.clear();
	std::cin.ignore();
	getline(std::cin, url);
	std::cout << "Enter browser path: ";
	std::cin.clear();
	getline(std::cin, browserPath);
	std::cout << std::endl << "Output Device: " << deviceNameStr << std::endl;
	std::cout << "Website URL: " << url << std::endl;
	std::cout << "Browser path: " << browserPath << std::endl << std::endl;
	std::cout << "Everything right? (To setup again type 'n' and restart app or check README.md) (y/n): ";
	std::cin >> auto_close;
	if (auto_close == 'y' || auto_close == 'Y')
	{
		std::ofstream settings;
		settings.open("CinemaMode_arc-w.txt", std::ios_base::trunc);
		for (int i = 0; i < browserPath.size(); i++)
		{
			if (browserPath.at(i) == '\\') 
			{
				browserPath.at(i) = '/';
			}
		}
		settings << "1" << std::endl << deviceNameStr << std::endl << url << std::endl << browserPath;
		settings.close();
	}
	return 0;
}