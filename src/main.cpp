// Almost 99.99% accuracy and input detection for copy/paste actions
// No corruption or overridden data
// Easy to use

// -mwindows linker flag

#include <iostream>

#include "ClipboardPlus.h"

LRESULT CALLBACK windProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK keyboardHookProc(int, WPARAM, LPARAM);

ClipboardPlus gClipboardPlus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	HANDLE mutex = CreateMutex(NULL, true, "clipboardplus");

	if(GetLastError() == ERROR_ALREADY_EXISTS) {
		UIHandler::messageBox("Clipboard+ is already running.\n\nIf the window is hidden, press Ctrl+F6 to show it.", "Clipboard+ already running");
		return 0;
	}

	MSG message;

	ClipboardPlus cbp(hInstance, windProc, keyboardHookProc, message, lpCmdLine, nCmdShow);
	gClipboardPlus = cbp;
	gClipboardPlus.start();

	ReleaseMutex(mutex);

	return message.wParam;
}

LRESULT CALLBACK windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	return gClipboardPlus.windProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	return gClipboardPlus.kbHookProc(nCode, wParam, lParam);
}
