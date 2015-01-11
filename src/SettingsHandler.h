#ifndef SETTINGSHANDLER_H_
#define SETTINGSHANDLER_H_

#include <iostream>
#include <fstream>
#include <vector>

#include "shlobj.h"

class SettingsHandler {

private:

	static void readFile(std::string fileName, std::string& outputString);
	static std::vector<std::string> readLines(const std::string& path);

	static const std::string SETTINGS_FILE, ROOT_DIR;

public:
	static std::string getAppDataDir();

	static std::string getKey(std::string key);
	static void setKey(std::string key, std::string value);
	static std::string getClipboardContent(int clipboardId);
	static void setClipboard(int clipboardId, std::string value);
	static void clearAllClipboards();
	static void clearClipboard(int clipboardId);

};


#endif /* SETTINGSHANDLER_H_ */
