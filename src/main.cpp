#include <windows.h>
#include <iostream>
#include <ctime>

#include "ClipboardPlus.h"

LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);

ClipboardPlus gClipboardPlus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG message;

	ClipboardPlus cbp(hInstance, windProc, message, lpCmdLine, nCmdShow);
	gClipboardPlus = cbp;
	gClipboardPlus.start();

	return message.wParam;
}

LRESULT CALLBACK windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return gClipboardPlus.windProc(hwnd, message, wParam, lParam);
}
