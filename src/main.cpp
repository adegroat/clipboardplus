#include <windows.h>
#include <iostream>
#include <ctime>

const char* TITLE = "Clipboard+";
const int WIDTH = 400, HEIGHT = 200;
bool running = false;

LPTSTR clipboardData[10] = {"", "", "", "", "", "", "", "", "", ""};

LRESULT CALLBACK WindProc(HWND, UINT, WPARAM, LPARAM);

long timeMs() {
	return ((float)clock() / CLOCKS_PER_SEC) * 1000;
}

void stop() {
	running = false;
	PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = TITLE;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.lpszMenuName = NULL;

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Error registering window!", "Error", MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindow(
			TITLE,
			TITLE,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			WIDTH, HEIGHT,
			NULL,
			NULL,
			hInstance,
			NULL);

	if(!hwnd){
		std::cout << GetLastError() << std::endl;
		MessageBox(NULL, "Error creating window!", "Error", MB_OK);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG message;
	running = true;

	float startTime = timeMs();

	while(running) {

		if(timeMs() - startTime > 150) {
			if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x43)) {

				for(int i = 0x30; i <= 0x39; i++) {
					if(GetAsyncKeyState(i)) {
						int index = i - 0x30;

						if(!OpenClipboard(hwnd)) {
							MessageBox(hwnd, "Error opening clipboard!", "Error", MB_OK);
							stop();
						}

						HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
						if(hGlobal) {
							LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);

							if(temp == NULL) {
								MessageBox(hwnd, "Error copying data to CB+", "Error", MB_OK);
							} else {
								clipboardData[index] = temp;
							}

							GlobalUnlock(hGlobal);
						} else {
							MessageBox(hwnd, "Couldn't copy data", "Error", MB_OK);
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

						if(!OpenClipboard(hwnd)) {
							MessageBox(hwnd, "Error opening clipboard!", "Error", MB_OK);
							stop();
						}

						HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, strlen(clipboardData[index]) + 1);
						LPTSTR temp = (LPTSTR)GlobalLock(hGlobal);
						memcpy(temp, clipboardData[index], strlen(clipboardData[index]) + 1);
						GlobalUnlock(hGlobal);

						if(!SetClipboardData(CF_TEXT, hGlobal)){
							std::cout << GetLastError() << std::endl;
							MessageBox(hwnd, "Error setting  clipboard data!", "Error", MB_OK);
						}

						if(GetAsyncKeyState(0x56)) EmptyClipboard();
						CloseClipboard();
					}
				}
			}

			startTime = timeMs();
		}

		if(PeekMessage(&message, hwnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return message.wParam;
}

LRESULT CALLBACK WindProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch(message) {

	case WM_DESTROY:
		stop();
		break;

	}

	return DefWindowProcA(hwnd, message, wParam, lParam);
}
