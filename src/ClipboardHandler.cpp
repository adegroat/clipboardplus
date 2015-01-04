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
	closeClipboard();
	return status;
}

std::string ClipboardHandler::getClipboardText() {
	std::string tempStr;

	openClipboard();

	HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
	if(hGlobal != NULL) {
		char* temp = (char*)GlobalLock(hGlobal);

		if(temp != NULL) {
			tempStr = temp;
		} else {
			notify("getClipboardText(), temp NULL");
		}
	} else {
		notify("getClipboardText(), hGlobal");
	}

	closeClipboard();
	return tempStr;
}

void ClipboardHandler::setClipboardText(std::string newText) {
	char* buffer = const_cast<char*>(getClipboardText().c_str());

	openClipboard();
	EmptyClipboard();
	int len = strlen(buffer) + 1;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
	char* temp = (char*)GlobalLock(hGlobal);
	memcpy(temp, newText.c_str(), len);
	GlobalUnlock(hGlobal);

	if(!SetClipboardData(CF_TEXT, hGlobal)) {
		notify("setClipboardText(), Could not set clipboard text");
	}

	closeClipboard();
}
