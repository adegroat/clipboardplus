#include "WindowSetup.h"

WindowSetup::WindowSetup(HINSTANCE hInstance, LPCSTR className, WNDPROC windProc) : hInstance(hInstance), className(className), windProc(windProc) {
	iconSmall = (HICON)LoadImageA(NULL, "icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	iconBig = (HICON)LoadImageA(NULL, "icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
}

bool WindowSetup::registerClass() {

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;
	wc.hInstance = hInstance;
	wc.lpszClassName = className;
	wc.lpfnWndProc = windProc;
	wc.hIcon = iconBig;
	wc.hIconSm = iconSmall;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE); // COLOR_3DFACE
	wc.lpszMenuName = NULL;

	return RegisterClassEx(&wc);
}

HWND WindowSetup::createWindow(LPCSTR title, int width, int height, int style) {

	return CreateWindow(
			title,
			title,
			style,
			100, 100,
			width, height,
			NULL,
			NULL,
			hInstance,
			NULL);
}
