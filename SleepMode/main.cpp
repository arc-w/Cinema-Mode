#include "header.h"

int main()
{
	// creates needed variables
	int chooseDevice;
	CString deviceName;
	std::string url;
	std::string browserPath;
	std::ifstream read;
	char auto_close;
	// checks if settings file exists
	read.open("CinemaMode_arc-w.txt"); // try to open CinemaMode_arc-w.txt (settings file)
	if (read.is_open())
	{
		std::string settingsFromFile;
		getline(read, settingsFromFile);
		if (settingsFromFile == "1") // if first line of settings file is "1" - start reading parameters from it, call CinemaMode function and pass it gathered info
		{
			// start reading lines and store them 
			std::cout << "Cinema mode activate" << std::endl;
			for (int i = 0; i < 3; i++)
			{
				settingsFromFile.clear();
				getline(read, settingsFromFile);
				switch (i)
				{
				case 0:
					deviceName = settingsFromFile.c_str(); // 2nd line = sound output device
					break;
				case 1:
					url = settingsFromFile; // 3rd line = website url
					break;
				case 2:
					browserPath = settingsFromFile; // 4th line = path to the browser 
					break;
				default:
					break;
				}
			}
			CinemaMode(deviceName, url, browserPath); // calls CinemaMode function, and passes to it, received from settings file, device name, website url and browser`s path
			return 0;
		}
		read.close();
	}
	// if file doesnt exist or first line is not "1", setup program
	std::cout << "Welcome to Cinema Mode. Enter your preferences." << std::endl << std::endl; 
	std::vector<CString> devicesList = CoutDefaultAudioDevices(); // prints all available sound output devices and save all of them to vector
	do
	{
		std::cout << std::endl << "Choose which output device will be automatically set as default (0,1,2...): ";
		std::cin >> chooseDevice;
	} while (chooseDevice < 0 || chooseDevice >= devicesList.size());
	CT2CA pszConvertedAnsiString(devicesList.at(chooseDevice)); // gets chosen device CString name and convert it to cw2a
	std::string deviceNameStr(pszConvertedAnsiString); // then converts it to string 
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
		// saves all settings in CinemaMode_arc-w.txt
		std::ofstream settings; 
		settings.open("CinemaMode_arc-w.txt", std::ios_base::trunc);
		for (int i = 0; i < browserPath.size(); i++) // changes all backslashes to regular slashes (so we can use it), for user not to do it manually (windows gives path with backslashes by default)
		{
			if (browserPath.at(i) == '\\') 
			{
				browserPath.at(i) = '/';
			}
		}
		settings << "1" << std::endl << deviceNameStr << std::endl << url << std::endl << browserPath; // write "1" in 1st line, (so next time program executes - it will work automatically), and pass to the next lines user`s settings
		settings.close();
	}
	return 0;
}