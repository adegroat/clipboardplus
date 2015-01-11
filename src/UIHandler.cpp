#include "UIHandler.h"

#include <iostream>

HICON UIHandler::CBP_ICON = (HICON)LoadImage(NULL, "icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);

UIHandler::UIHandler(HWND pHwnd, std::string fontName) : parentHwnd(pHwnd), fontName(fontName) {
	LPRECT parentRect = new RECT;
	GetWindowRect(parentHwnd, parentRect);
	parentWidth = parentRect->right - parentRect->left;
	parentHeight = parentRect->bottom - parentRect->top;
	parentX = parentRect->left;
	parentY = parentRect->top;
	font = CreateFont(16, 7, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
}

bool CALLBACK UIHandler::setChildrenFontProc(HWND hwnd, LPARAM lParam) {
	SendMessage(hwnd, WM_SETFONT, (WPARAM)lParam, 0);
	return true;
}

int UIHandler::messageBox(std::string message, std::string title, HWND parentHwnd, int style) {
	return MessageBox(parentHwnd, message.c_str(), title.c_str(), style);
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

HWND UIHandler::createCheckbox(std::string text, int x, int y, bool autoSize, int id) {
	return CreateWindow(
			"BUTTON",
			text.c_str(),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_CHECKBOX | BS_CENTER | BS_VCENTER,
			x, y,
			autoSize ? 20 + 8 * text.length() : DEFAULT_BTN_WIDTH, DEFAULT_BTN_HEIGHT,
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
			"Test",
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
	int shouldReadOnly = readOnly ? ES_READONLY : 0;
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

HWND UIHandler::createMultiLineEditBox(std::string defaultText, int x, int y, int width, int height, bool readOnly, int id) {
	int shouldReadOnly = readOnly ? ES_READONLY : 0;
	return CreateWindow(
			"EDIT",
			defaultText.c_str(),
			WS_CHILD | WS_VISIBLE | WS_BORDER | shouldReadOnly | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			x, y,
			width, height,
			parentHwnd,
			(HMENU)id,
			NULL,
			NULL);
}

void UIHandler::showBaloonTip(std::string title, std::string info) {
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uVersion = 4;
	nid.uID = CBP_TRAY_ICON;
	nid.hWnd = parentHwnd;
	nid.hIcon = CBP_ICON;
	nid.uFlags = NIF_INFO | NIF_ICON;
	nid.dwInfoFlags = NIIF_USER | NIIF_NOSOUND;
	memcpy(nid.szInfoTitle, title.c_str(), title.length() + 1);
	memcpy(nid.szInfo, info.c_str(), info.length() + 1);

	Shell_NotifyIcon(NIM_MODIFY, &nid);

}

void UIHandler::setupUI() {
	createLabel("Clipboards", 0, 13, true);

	for(int i = 0; i < 10; i++) {
		char label[3] = {'#', (char)(i + 0x30), '\0'};
		int yOffset = 40;
		int yPos = i == 0 ? yOffset + 9 * 28 : yOffset + (i - 1) * 28;
		createLabel(label, 8, yPos, false);
		clipboardEditBox[i] = createEditBox("", 30, yPos, 430, 20, true, 3000 + i);

		createIconButton("delete.ico", 465, yPos, 22, 22, 16, 16, 2000 + i);
	}

	createButton("Clear All", 30, 325, false, BTN_CLEAR);
	//createButton("Hide", 120, 325, false, BTN_HIDE);
	createIconButton("help.ico", 434, 325, 25, 25, 16, 16, BTN_HELP);

	HWND stdPasteCB = createCheckbox("Standard paste", 30, 350, true, BTN_STD_PASTE);
	SendMessage(stdPasteCB, BM_SETCHECK, (WPARAM)Settings::standardPaste, 0);

	HWND balloonTipsCB = createCheckbox("Balloon Tips", 180, 350, true, BTN_BALLOON_TIPS);
	SendMessage(balloonTipsCB, BM_SETCHECK, (WPARAM)Settings::showBalloonTips, 0);

	EnumChildWindows(parentHwnd, (WNDENUMPROC)setChildrenFontProc, (LPARAM)font);
}
