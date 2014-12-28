#ifndef CLIPBOARDPLUS_H_
#define CLIPBOARDPLUS_H_

#include <windows.h>
#include <iostream>
#include <ctime>

#include "WindowSetup.h"

#define BTN_CLEAR 0x9990
#define BTN_HIDE  0x9991

#define HOTKEY_SHOWWINDOW 0x8880

class ClipboardPlus {

private:
	HINSTANCE hInstance;
	bool running;
	LPCSTR title;
	int width, height;
	HWND mainWindow;
	int nCmdShow;
	MSG message;
	LPTSTR clipboardData[10];

	HWND clipboardEditBox[10];

public:
	ClipboardPlus(HINSTANCE, WNDPROC, MSG, LPSTR, int);
	ClipboardPlus() {  }
	LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
	WNDPROC wProc;
	void start();
	void mainLoop();
	void stop();
	bool isRunning() { return running; }
	long timeMs() { return ((float)clock() / CLOCKS_PER_SEC) * 1000; }

	// 1000 0000 0000 0000 = 0x8000
	bool keyDown(int vKey) { return (GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0; }


};

#endif /* CLIPBOARDPLUS_H_ */
