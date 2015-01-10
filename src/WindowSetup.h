#ifndef WINDOWSETUP_H
#define WINDOWSETUP_H

#include <windows.h>

class WindowSetup {

private:
	HINSTANCE hInstance;
	LPCSTR className;
	WNDPROC windProc;
	HICON iconBig, iconSmall;
public:

	WindowSetup(HINSTANCE hInstance, LPCSTR className, WNDPROC windProc);
	WindowSetup() {  }
	bool registerClass();
	HWND createWindow(LPCSTR title, int width, int height, int style);
};

#endif
