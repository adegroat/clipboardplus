#ifndef CLIPBOARDPLUS_H_
#define CLIPBOARDPLUS_H_

#include <windows.h>
#include <iostream>
#include <ctime>

#include "WindowSetup.h"

#define BTN_CLEAR 0x9990
#define BTN_HIDE  0x9991

#define BTN_CLEAR0 0x5550
#define BTN_CLEAR1 0x5551
#define BTN_CLEAR2 0x5552
#define BTN_CLEAR3 0x5553
#define BTN_CLEAR4 0x5554
#define BTN_CLEAR5 0x5555
#define BTN_CLEAR6 0x5556
#define BTN_CLEAR7 0x5557
#define BTN_CLEAR8 0x5558
#define BTN_CLEAR9 0x5559

#define HOTKEY_SHOWWINDOW 0x8880
#define HOTKEY_CTRLC 0x8881
#define HOTKEY_CTRLV 0x8882

const HFONT font = CreateFont(16, 7, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");

class ClipboardPlus {

private:
	HINSTANCE hInstance;
	bool running;
	LPCSTR title;
	int width, height;
	HWND mainWindow;
	int nCmdShow;
	MSG message;
	LPSTR clipboardData[10];
	HWND clipboardEditBox[10];

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
	bool isRunning() { return running; }
	static bool CALLBACK setChildrenFontProc(HWND, LPARAM);
};

#endif /* CLIPBOARDPLUS_H_ */
