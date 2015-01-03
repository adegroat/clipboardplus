#include <iostream>

#include "ClipboardPlus.h"

LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK keyboardHookProc(int, WPARAM, LPARAM);

ClipboardPlus gClipboardPlus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG message;

	ClipboardPlus cbp(hInstance, windProc, keyboardHookProc, message, lpCmdLine, nCmdShow);
	gClipboardPlus = cbp;
	gClipboardPlus.start();

	return message.wParam;
}

LRESULT CALLBACK windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return gClipboardPlus.windProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	return gClipboardPlus.kbHookProc(nCode, wParam, lParam);
}
