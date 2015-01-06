#ifndef CLIPBOARDPLUS_H_
#define CLIPBOARDPLUS_H_

#include <windows.h>
#include <iostream>

#include "WindowSetup.h"
#include "ClipboardHandler.h"

#define HOTKEY_SHOWWINDOW 8880

class ClipboardPlus {

private:
	HINSTANCE hInstance;
	bool running;
	LPCSTR title;
	int width, height;
	HWND mainWindow;
	int nCmdShow;
	MSG message;
	std::string clipboardData[10];
	ClipboardHandler* cbHandler;
	UIHandler* uiHandler;

public:
	ClipboardPlus(HINSTANCE, WNDPROC, HOOKPROC, MSG, LPSTR, int);
	ClipboardPlus() {  }
	LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK kbHookProc(int, WPARAM, LPARAM);
	WNDPROC wProc;
	HOOKPROC kbProc;
	HHOOK kbHook;
	bool ctrlDown, cDown, vDown, dDown;
	int numKey;
	void start();
	void stop(const char message[128] = "NONE");
	void cleanUp();
	void setupUI(HWND hwnd);
};

#endif /* CLIPBOARDPLUS_H_ */
