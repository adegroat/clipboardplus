#ifndef UI_HANDLER_H
#define UI_HANDLER_H

#include <windows.h>
#include <string>

#define DEFAULT_BTN_WIDTH 80
#define DEFAULT_BTN_HEIGHT 25

class UIHandler {

private:
	HWND parentHwnd;
	int parentWidth, parentHeight;
	std::string fontName;
	HWND clipboardEditBox[10];
	HFONT font;

	static bool CALLBACK setChildrenFontProc(HWND, LPARAM);
public:

	UIHandler(HWND pHwnd, std::string fontName);
	HWND createButton(std::string text, int x, int y, int width, int height, int id);
	HWND createButton(std::string text, int x, int y, bool autoSize, int id);
	HWND createIconButton(std::string iconFileName, int x, int y, int width, int height, int iconWidth, int iconHeight, int id);
	HWND createLabel(std::string text, int x, int y, bool center);
	HWND createEditBox(std::string defaultText, int x, int y, int width, int height, bool readOnly, int id);

	HWND getCBEditBox(int index) { return clipboardEditBox[index]; }
	void setupUI();

	static int messageBox(std::string message, std::string title = "Clipboard+", HWND parentHwnd = NULL, int style = MB_OK | MB_ICONINFORMATION);

	static const int BTN_CLEAR = 1000;
	static const int BTN_HIDE = 1001;
	static const int BTN_HELP = 1002;

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
};

#endif
