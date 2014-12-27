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
	bool setup();
};

#endif
