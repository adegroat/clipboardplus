#include "WindowSetup.h"

WindowSetup::WindowSetup(HINSTANCE hInstance, LPCSTR className, WNDPROC windProc) : hInstance(hInstance), className(className), windProc(windProc) {

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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE); // COLOR_BTNFACE
	wc.lpszMenuName = NULL;

	return RegisterClassEx(&wc);
}

HWND WindowSetup::createWindow(LPCSTR title, int width, int height, int style) {

	return CreateWindow(
			title,
			title,
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL,
			NULL,
			hInstance,
			NULL);
}
