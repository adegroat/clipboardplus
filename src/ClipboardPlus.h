#ifndef CLIPBOARDPLUS_H_
#define CLIPBOARDPLUS_H_

#include <windows.h>
#include <iostream>
#include <ctime>

#include "WindowSetup.h"

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

public:
	ClipboardPlus(HINSTANCE, MSG, LPSTR, int);
	~ClipboardPlus();
	void start();
	LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
	void mainLoop();
	void stop();
	bool isRunning() { return running; }
	long timeMs() { return ((float)clock() / CLOCKS_PER_SEC) * 1000; }


};

#endif /* CLIPBOARDPLUS_H_ */
