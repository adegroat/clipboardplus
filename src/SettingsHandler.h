#ifndef SETTINGSHANDLER_H_
#define SETTINGSHANDLER_H_

#define SETTINGS_DIR "D:\\Program Files\\Clipboard+"
#define SETTINGS_FILE "D:\\Program Files\\Clipboard+\\settings.txt"

#include <iostream>
#include <fstream>
#include <vector>

class SettingsHandler {

private:

	static void readFile(std::string fileName, std::string& outputString);
	static std::vector<std::string> readLines(const std::string& path);
public:

	// get/setKey(std::string key)
	// get/setClipboardContent(int clipboardId)

	SettingsHandler();

	static std::string getKey(std::string key);
	static void setKey(std::string key, std::string value);
	static std::string getClipboardContent(int clipboardId);
	static void setClipboard(int clipboardId, std::string value);
	static void clearAllClipboards();
	static void clearClipboard(int clipboardId);

};


#endif /* SETTINGSHANDLER_H_ */
