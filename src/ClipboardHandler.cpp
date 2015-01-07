#include "ClipboardHandler.h"

ClipboardHandler::ClipboardHandler(HWND hwnd) : hwnd(hwnd) {

}

ClipboardHandler::~ClipboardHandler() {
}

void ClipboardHandler::notify(std::string message) {
	std::cout << "ClipboardHandler:\n\tError:" << GetLastError() << std::endl;
	std::cout << "\tMessage: " << message << std::endl;
}

bool ClipboardHandler::openClipboard() {
	if(!OpenClipboard(hwnd)) {
		notify("openClipboard() failure");
		return false;
	}
	return true;
}

bool ClipboardHandler::emptyClipboard() {
	openClipboard();
	bool status = EmptyClipboard();
	CloseClipboard();
	return status;
}

std::string ClipboardHandler::getClipboardText() {
	if(!IsClipboardFormatAvailable(CF_TEXT)) {
		notify("getClipboardData(), Clipboard Empty!");
		return "";
	}
	openClipboard();
	std::string tempStr = "";

	HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
	if(hGlobal != NULL) {
		char* temp = (char*)GlobalLock(hGlobal);

		if(temp != NULL) {
			tempStr = temp;
			GlobalUnlock(hGlobal);
		} else {
			notify("getClipboardText(), temp NULL");
		}
	} else {
		notify("getClipboardText(), hGlobal NULL");
	}

	CloseClipboard();
	return tempStr;
}

void ClipboardHandler::setClipboardText(std::string newText) {
	openClipboard();
	EmptyClipboard();
	int len = newText.length() + 1;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	char* temp = (char*)GlobalLock(hGlobal);
	memcpy(temp, newText.c_str(), len);
	GlobalUnlock(hGlobal);

	if(!SetClipboardData(CF_TEXT, hGlobal)) {
		notify("setClipboardText(), Could not set clipboard text");
	}

	CloseClipboard();
}
