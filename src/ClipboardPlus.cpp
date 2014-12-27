#include "ClipboardPlus.h"

ClipboardPlus::ClipboardPlus(HINSTANCE hInstance, WNDPROC wp, MSG message, LPSTR lpCmdLine, int nCmdShow) {
	this->hInstance = hInstance;
	this->wp = wp;
	this->nCmdShow = nCmdShow;
	this->message = message;
	running = false;
	title = "Clipboard+";
	width = 400;
	height = 200;
	mainWindow = NULL;
	//clipboardData = new LPTSTR[10];
	for(int i = 0; i < 10; i++) clipboardData[i] = "";
}

ClipboardPlus::~ClipboardPlus() {
//	delete[] clipboardData;
}

void ClipboardPlus::start() {
	if(running) return;

	running = true;

	WindowSetup ws(hInstance, title, wp);
	if(!ws.setup()){
		MessageBox(NULL, "Error registering window!", "Error", MB_OK);
		return;
	}
	mainWindow = CreateWindow(
			title,
			title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL,
			NULL,
			hInstance,
			NULL);

	if(!mainWindow){
		MessageBox(NULL, "Error creating window!", "Error", MB_OK);
		return;
	}
	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);

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
			if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x43)) {

				for(int i = 0x30; i <= 0x39; i++) {
					if(GetAsyncKeyState(i)) {
						int index = i - 0x30;

						if(!OpenClipboard(mainWindow)) {
							MessageBox(mainWindow, "Error opening clipboard! (copy)", "Error", MB_OK);
							//stop();
						}

						HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
						if(hGlobal) {
							LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);

							if(temp == NULL) {
								MessageBox(NULL, "Error copying data to CB+", "Error", MB_OK);
							} else {
								clipboardData[index] = temp;
							}

							GlobalUnlock(hGlobal);
						} else {
							MessageBox(NULL, "Couldn't copy data", "Error", MB_OK);
						}

						EmptyClipboard();
						CloseClipboard();
					}
				}
			}

			// TODO: Figure out Ctrl+V+NUM instead of Ctrl+NUM+V
			if(GetAsyncKeyState(VK_CONTROL) /*&& GetAsyncKeyState(0x56)*/) {

				for(int i = 0x30; i <= 0x39; i++) {

					if(GetAsyncKeyState(i)) {
						int index = i - 0x30;

						if(!OpenClipboard(mainWindow)) {
							MessageBox(mainWindow, "Error opening clipboard! (paste)", "Error", MB_OK);
							//stop();
						}

						HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, strlen(clipboardData[index]) + 1);
						LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);
						memcpy(temp, clipboardData[index], strlen(clipboardData[index]) + 1);
						GlobalUnlock(hGlobal);

						if(!SetClipboardData(CF_TEXT, hGlobal)){
							std::cout << GetLastError() << std::endl;
							MessageBox(mainWindow, "Error setting  clipboard data!", "Error", MB_OK);
						}

						if(GetAsyncKeyState(0x56)) EmptyClipboard();
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

	case WM_DESTROY:
		stop();
		break;
	default:
		return DefWindowProcA(hwnd, message, wParam, lParam);
	}

	return 0;
}
