#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_

#include "SettingsHandler.h"

class Settings {

public:
	static bool standardPaste;
	static bool showBalloonTips;

	static void initSettings();

};

#endif /* SRC_SETTINGS_H_ */
