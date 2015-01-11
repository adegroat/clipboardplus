#include "SettingsHandler.h"

const std::string SettingsHandler::SETTINGS_FILE = getAppDataDir() + "\\Clipboard+\\settings.txt";
const std::string SettingsHandler::ROOT_DIR = getAppDataDir() + "\\Clipboard+";

std::string SettingsHandler::getAppDataDir() {
	char buffer[MAX_PATH];

	if(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buffer) != S_OK) {
		std::cout << "Could not find appdata path!" << std::endl;
	}
	return std::string(buffer);
}

void SettingsHandler::readFile(std::string fileName, std::string& outputString) {
	std::ifstream inputFile(fileName.c_str());
	std::string temp;

	if(inputFile.is_open()) {
		while(std::getline(inputFile, temp)) {
			outputString.append(temp);
		}
	} else {
		std::cout << "Error opening file" << std::endl;
	}
	inputFile.close();
}

std::vector<std::string> SettingsHandler::readLines(const std::string& path) {
	std::vector<std::string> lines;
	lines.reserve(5);

	std::ifstream inputFile(path.c_str());

	if(inputFile.is_open()) {
		std::string temp = "";
		while(getline(inputFile, temp)) {
			lines.push_back(temp);
		}
	}
	inputFile.close();

	return lines;
}

std::string SettingsHandler::getKey(std::string key) {
	std::ifstream inputFile(SETTINGS_FILE.c_str());
	std::string temp = "", finalString = "";

	if(inputFile.is_open()) {

		while(std::getline(inputFile, temp)) {
			if(strstr(temp.c_str(), key.c_str())) {
				finalString = temp.substr(key.length() + 1, std::string::npos);
				break;
			}
		}
	} else {
		std::cout << "Failed to open settings file" << std::endl;
	}
	inputFile.close();

	if(finalString == "") {
		std::cout << "Key \"" << key << "\" not found." << std::endl;
	}

	return finalString;
}

std::string SettingsHandler::getClipboardContent(int clipboardId) {
	std::string finalString = "";

	if(clipboardId >= 0 && clipboardId <= 9) {
		char cbIdStr[2] = {(char)(clipboardId + 0x30), '\0'};
		std::string clipboardFile = std::string(ROOT_DIR).append("\\cb").append(cbIdStr).append(".txt");
		readFile(clipboardFile, finalString);
	}

	return finalString;
}

void SettingsHandler::clearClipboard(int clipboardId) {
	if(clipboardId < 0 || clipboardId > 9) return;

	char cbIdStr[2] = {(char)(clipboardId + 0x30), '\0'};
	std::string cbFileName = std::string(ROOT_DIR).append("\\cb").append(cbIdStr).append(".txt");
	std::ofstream cbFile(cbFileName.c_str(), std::ios::trunc);
	cbFile.close();
}

void SettingsHandler::clearAllClipboards() {
	for(int i = 0; i < 10; i++) {
		clearClipboard(i);
	}
}

void SettingsHandler::setKey(std::string key, std::string value) {
	std::vector<std::string> lines = readLines(SETTINGS_FILE);
	std::ofstream settingsFile(SETTINGS_FILE.c_str(), std::ios::trunc);

	if(settingsFile.is_open()) {
		for(unsigned int i = 0; i < lines.size(); i++) {
			if(lines[i].substr(0, key.length()) == key) {
				lines[i] = key + ":" + value;
			}
			settingsFile << lines[i] << "\n";
		}
	} else {
		std::cout << "Could not not set key \"" << key << "\"" << std::endl;
	}
	settingsFile.close();
}

void SettingsHandler::setClipboard(int clipboardId, std::string value) {
	if(clipboardId < 0 || clipboardId > 9) return;

	char cbIdStr[2] = {(char)(clipboardId + 0x30), '\0'};
	std::string cbFileName = std::string(ROOT_DIR).append("\\cb").append(cbIdStr).append(".txt");
	std::ofstream cbFile(cbFileName.c_str(), std::ios::trunc);

	if(cbFile.is_open()) {
		cbFile << value;
	} else {
		std::cout << "Could not save clipboard " << clipboardId << std::endl;
	}
	cbFile.close();
}
