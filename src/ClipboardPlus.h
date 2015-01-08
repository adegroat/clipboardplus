#ifndef CLIPBOARDPLUS_H_
#define CLIPBOARDPLUS_H_

#include <windows.h>
#include <iostream>
#include <ctime>

#include "WindowSetup.h"
#include "ClipboardHandler.h"

#define HOTKEY_SHOWWINDOW 8880
#define HOTKEY_CTRLNUM 9990

class ClipboardPlus {

private:
	HINSTANCE hInstance;
	bool running;
	LPCSTR title;
	int width, height;
	HWND mainWindow;
	int nCmdShow;
	MSG message;
	HHOOK kbHook;
	std::string clipboardData[10];
	ClipboardHandler* cbHandler;
	UIHandler* uiHandler;
	WNDPROC wProc;
	HOOKPROC kbProc;
	bool ctrlDown, cDown, vDown, standardPaste;
	int numKey;

public:
	ClipboardPlus(HINSTANCE, WNDPROC, HOOKPROC, MSG, LPSTR, int);
	ClipboardPlus() {  }
	LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK kbHookProc(int, WPARAM, LPARAM);
	void start();
	void stop(const char message[128] = "NONE");
	void cleanUp();
	void setupUI(HWND hwnd);
	float timeMs() { return ((float)clock() / CLOCKS_PER_SEC) * 1000; }
};

#endif /* CLIPBOARDPLUS_H_ */
