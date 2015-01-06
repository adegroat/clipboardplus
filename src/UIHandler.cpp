#include "UIHandler.h"

#include <iostream>

UIHandler::UIHandler(HWND pHwnd, std::string fontName) : parentHwnd(pHwnd), fontName(fontName) {
	LPRECT parentRect = new RECT;
	GetWindowRect(parentHwnd, parentRect);
	parentWidth = parentRect->right - parentRect->left;
	parentHeight = parentRect->bottom - parentRect->top;
	font = CreateFont(16, 7, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
}

bool CALLBACK UIHandler::setChildrenFontProc(HWND hwnd, LPARAM lParam) {
	SendMessage(hwnd, WM_SETFONT, (WPARAM)lParam, 0);
	return true;
}

int UIHandler::messageBox(std::string message, std::string title, HWND parentHwnd, int style) {
	MessageBox(parentHwnd, message.c_str(), title.c_str(), style);
}

HWND UIHandler::createButton(std::string text, int x, int y, int width, int height, int id) {
	return CreateWindow(
			"BUTTON",
			text.c_str(),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_CENTER | BS_VCENTER,
			x, y,
			width, height,
			parentHwnd,
			(HMENU)id,
			NULL,
			NULL);
}

HWND UIHandler::createIconButton(std::string iconFileName, int x, int y, int width, int height, int iconWidth, int iconHeight, int id) {
	HICON icon = (HICON)LoadImage(NULL, iconFileName.c_str(), IMAGE_ICON, iconWidth, iconHeight, LR_LOADFROMFILE);
	if(icon == NULL) {
		std::cout << "Failed to load icon for button - Icon: " << iconFileName << std::endl;
		return NULL;
	}
	HWND button = CreateWindow(
			"BUTTON",
			"",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_CENTER | BS_VCENTER | BS_ICON,
			x, y,
			width, height,
			parentHwnd,
			(HMENU)id,
			NULL,
			NULL);
	SendMessage(button, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon);
	return button;
}

HWND UIHandler::createButton(std::string text, int x, int y, bool autoSize, int id) {
	return createButton(text, x, y, autoSize ? 20 + 8 * text.length() : DEFAULT_BTN_WIDTH, DEFAULT_BTN_HEIGHT, id);
}

HWND UIHandler::createLabel(std::string text, int x, int y, bool center) {
	return CreateWindow(
			"STATIC",
			text.c_str(),
			WS_CHILD | WS_VISIBLE | SS_CENTER | SS_ENDELLIPSIS,
			x, y,
			center ? parentWidth : 5 + 8 * text.length(), 20,
			parentHwnd,
			NULL,
			NULL,
			NULL);
}

HWND UIHandler::createEditBox(std::string defaultText, int x, int y, int width, int height, bool readOnly, int id) {
	int shouldReadOnly = 0;
	if(readOnly) shouldReadOnly = ES_READONLY;
	return CreateWindow(
			"EDIT",
			defaultText.c_str(),
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | shouldReadOnly,
			x, y,
			width, height,
			parentHwnd,
			(HMENU)id,
			NULL,
			NULL);
}

void UIHandler::setupUI() {
	createLabel("Clipboards", 0, 15, true);

	for(int i = 0; i < 10; i++) {
		char label[3] = {'#', (char)(i + 0x30), '\0'};
		createLabel(label, 8, 42 + i * 28, false);
		clipboardEditBox[i] = createEditBox("", 30, 40 + i * 28, 430, 20, true, 3000 + i);

		createButton("X", 465, 40 + i * 28, 20, 20, 2000 + i); // 2000 + i = BTN_CLEAR0, BTN_CLEAR1, etc
	}

	createButton("Clear All", 30, 320, false, BTN_CLEAR);
	createButton("Hide", 120, 320, false, BTN_HIDE);

//	createButton("Help", 380, 320, false, BTN_HELP);
	createIconButton("help.ico", 210, 320, 25, 25, 16, 16, BTN_HELP);

	EnumChildWindows(parentHwnd, (WNDENUMPROC)setChildrenFontProc, (LPARAM)font);
}
