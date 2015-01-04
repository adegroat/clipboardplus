#include <windows.h>

#ifndef WINDOWSETUP_H
#define WINDOWSETUP_H

class WindowSetup {

private:
	HINSTANCE hInstance;
	LPCSTR className;
	WNDPROC windProc;
public:

	WindowSetup(HINSTANCE hInstance, LPCSTR className, WNDPROC windProc);
	bool registerClass();
	HWND createWindow(LPCSTR title, int width, int height, int style);
	HICON iconBig, iconSmall;
};

#endif
