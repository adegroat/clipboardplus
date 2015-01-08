#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <windows.h>
#include <string>

#define DEFAULT_BTN_WIDTH 80
#define DEFAULT_BTN_HEIGHT 25

class UIHandler {

private:
	HWND parentHwnd;
	std::string fontName;
	HWND clipboardEditBox[10];
	HFONT font;

	static bool CALLBACK setChildrenFontProc(HWND, LPARAM);
public:
	int parentWidth, parentHeight, parentX, parentY;

	UIHandler(HWND pHwnd, std::string fontName);
	HWND createButton(std::string text, int x, int y, int width, int height, int id);
	HWND createButton(std::string text, int x, int y, bool autoSize, int id);
	HWND createCheckbox(std::string text, int x, int y, bool autoSize, int id);
	HWND createIconButton(std::string iconFileName, int x, int y, int width, int height, int iconWidth, int iconHeight, int id);
	HWND createLabel(std::string text, int x, int y, bool center);
	HWND createEditBox(std::string defaultText, int x, int y, int width, int height, bool readOnly, int id);
	HWND createMultiLineEditBox(std::string defaultText, int x, int y, int width, int height, bool readOnly, int id);

	HWND getCBEditBox(int index) { return clipboardEditBox[index]; }
	void setupUI();

	static int messageBox(std::string message, std::string title = "Clipboard+", HWND parentHwnd = NULL, int style = MB_OK | MB_ICONINFORMATION);

	static const int BTN_CLEAR = 1000;
	static const int BTN_HIDE = 1001;
	static const int BTN_HELP = 1002;
	static const int BTN_STD_PASTE = 1003;

	static const int BTN_CLEAR0 = 2000;
	static const int BTN_CLEAR1 = 2001;
	static const int BTN_CLEAR2 = 2002;
	static const int BTN_CLEAR3 = 2003;
	static const int BTN_CLEAR4 = 2004;
	static const int BTN_CLEAR5 = 2005;
	static const int BTN_CLEAR6 = 2006;
	static const int BTN_CLEAR7 = 2007;
	static const int BTN_CLEAR8 = 2008;
	static const int BTN_CLEAR9 = 2009;

	static const int BTN_EDIT0 = 2500;
	static const int BTN_EDIT1 = 2501;
	static const int BTN_EDIT2 = 2502;
	static const int BTN_EDIT3 = 2503;
	static const int BTN_EDIT4 = 2504;
	static const int BTN_EDIT5 = 2505;
	static const int BTN_EDIT6 = 2506;
	static const int BTN_EDIT7 = 2507;
	static const int BTN_EDIT8 = 2508;
	static const int BTN_EDIT9 = 2509;
};

#endif
