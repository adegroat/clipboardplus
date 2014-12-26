#include <windows.h>

#ifndef WINDOWSETUP_H
#define WINDOWSETUP_H

class WindowSetup {

private:
	HINSTANCE hInstance;
	char* className;
	WNDPROC windProc;
public:

	WindowSetup(HINSTANCE hInstance, char* className, WNDPROC windProc);
	bool setup();
};

#endif
