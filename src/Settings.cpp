#include "Settings.h"

bool Settings::standardPaste = false;
bool Settings::showBalloonTips = true;

void Settings::initSettings() {
	standardPaste = SettingsHandler::getKey("standardPaste") == "0" ? false : true;
	showBalloonTips = SettingsHandler::getKey("showBalloonTips") == "0" ? false : true;
}
