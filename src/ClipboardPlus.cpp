#include "ClipboardPlus.h"

ClipboardPlus::ClipboardPlus(HINSTANCE hInstance, WNDPROC wp, MSG message, LPSTR lpCmdLine, int nCmdShow) {
	this->hInstance = hInstance;
	this->wProc = wp;
	this->message = message;
	this->nCmdShow = nCmdShow;
	running = false;
	title = "Clipboard+";
	width = 500;
	height = 400;
	mainWindow = NULL;
	for(int i = 0; i < 10; i++) clipboardData[i] = "";
}

void ClipboardPlus::start() {
	if(running) return;

	running = true;

	WindowSetup ws(hInstance, title, wProc);
	if(!ws.registerClass()){
		MessageBox(NULL, "Error registering window!", "Error", MB_OK);
		return;
	}
	mainWindow = ws.createWindow(title, width, height, WS_OVERLAPPEDWINDOW);
	if(!mainWindow){
		MessageBox(NULL, "Error creating window!", "Error", MB_OK);
		return;
	}
	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);

	RegisterHotKey(mainWindow, HOTKEY_SHOWWINDOW, MOD_CONTROL, VK_F6);

	mainLoop();
}

void ClipboardPlus::stop() {
	if(!running) return;
	running = false;
	PostQuitMessage(0);
}

void ClipboardPlus::mainLoop() {
	float startTime = timeMs();

	while(running) {

		if(timeMs() - startTime > 150) {
			if(keyDown(VK_CONTROL) && keyDown(0x43)) {

				for(int i = 0x30; i <= 0x39; i++) {
					if(keyDown(i)) {
						int index = i - 0x30;

						if(!OpenClipboard(mainWindow)) {
							MessageBox(mainWindow, "Error opening clipboard! (copy)", "Error", MB_OK);
						}

						HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
						if(hGlobal) {
							LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);
							GlobalUnlock(hGlobal);

							if(temp != NULL) {
								clipboardData[index] = temp;
								SetWindowTextA(clipboardEditBox[index], (LPCSTR)clipboardData[index]);
							}

						}

						EmptyClipboard();
						CloseClipboard();
					}
				}
			}

			// TODO: Figure out Ctrl+V+NUM instead of Ctrl+NUM+V
			if(keyDown(VK_CONTROL) /*&& keyDown(0x56)*/) {

				for(int i = 0x30; i <= 0x39; i++) {

					if(keyDown(i)) {
						int index = i - 0x30;

						if(!OpenClipboard(mainWindow)) {
							MessageBox(mainWindow, "Error opening clipboard! (paste)", "Error", MB_OK);
						}

						HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, strlen(clipboardData[index]) + 1);
						LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);
						memcpy(temp, clipboardData[index], strlen(clipboardData[index]) + 1);
						GlobalUnlock(hGlobal);

						if(!SetClipboardData(CF_TEXT, hGlobal)) {
							MessageBox(mainWindow, "Error setting  clipboard data!", "Error", MB_OK);
						}

						if(keyDown(0x56)) EmptyClipboard();
						CloseClipboard();
					}
				}
			}

			startTime = timeMs();
		}

		if(PeekMessage(&message, mainWindow, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
}

LRESULT CALLBACK ClipboardPlus::windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {

	case WM_CREATE:
	{
		CreateWindow("STATIC", "Clipboards", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 15, width, 20, hwnd, NULL, NULL, NULL);

		for(int i = 0; i < 10; i++) {
			char iBuf[2];
			itoa(i, iBuf, 10);
			char finalBuf[] = "#";
			strcat(finalBuf, iBuf);
			LPCSTR data = const_cast<const char*>(finalBuf);

			CreateWindow("STATIC", data, WS_CHILD | WS_VISIBLE | SS_SIMPLE, 8, i * 28 + 42, 20, 14, hwnd, NULL, NULL, NULL);

			clipboardEditBox[i] = CreateWindow(
					"EDIT",
					clipboardData[i],
					WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED | ES_AUTOHSCROLL,
					30, 40 + i * 28,
					435, 20,
					hwnd,
					(HMENU)i,
					NULL,
					NULL);
		}

		CreateWindow(
				"BUTTON",
				"Clear All",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				10, 320,
				100, 30,
				hwnd,
				(HMENU)BTN_CLEAR,
				NULL,
				NULL);

		CreateWindow(
				"BUTTON",
				"Hide",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				120, 320,
				100, 30,
				hwnd,
				(HMENU)BTN_HIDE,
				NULL,
				NULL);
	} break;

	case WM_COMMAND:
	{
		if(HIWORD(wParam) == BN_CLICKED) {

			switch(LOWORD(wParam)) {

			case BTN_CLEAR:
				for(int i = 0; i < 10; i++) {
					clipboardData[i] = "";
					SetWindowText(clipboardEditBox[i], "");
				}
				break;

			case BTN_HIDE:
				ShowWindow(hwnd, SW_HIDE);
				MessageBox(NULL, "Press Ctrl+F6 to show!", "Abracadabra!", MB_OK | MB_ICONINFORMATION);
				break;

			}
		}

	} break;

	case WM_HOTKEY:
		if(wParam == HOTKEY_SHOWWINDOW) {
			ShowWindow(hwnd, SW_SHOW);
		}
		break;

	case WM_DESTROY:
		UnregisterHotKey(hwnd, HOTKEY_SHOWWINDOW);
		stop();
		break;
	default:
		return DefWindowProcA(hwnd, message, wParam, lParam);
	}

	return 0;
}
