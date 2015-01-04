
#ifndef CLIPBOARDHANDLER_H_
#define CLIPBOARDHANDLER_H_

#include <windows.h>
#include <iostream>

class ClipboardHandler {

private:
	HWND hwnd;
	void notify(std::string message);

public:
	ClipboardHandler(HWND hwnd);
	~ClipboardHandler();

	bool openClipboard();
	bool emptyClipboard();
	bool closeClipboard() { return CloseClipboard(); }
	std::string getClipboardText();
	void setClipboardText(std::string newText);

};



#endif /* CLIPBOARDHANDLER_H_ */
