#include "WindowSetup.h"

WindowSetup::WindowSetup(HINSTANCE hInstance, LPCSTR className, WNDPROC windProc) : hInstance(hInstance), className(className), windProc(windProc) {

}

bool WindowSetup::setup() {

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
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
	wc.lpszMenuName = NULL;

	return RegisterClassEx(&wc);
}
