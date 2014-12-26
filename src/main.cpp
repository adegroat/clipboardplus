#include <windows.h>
#include <iostream>
#include <ctime>

const char* TITLE = "Clipboard+";
const int WIDTH = 400, HEIGHT = 200;
bool running = false;

LPTSTR clipboardData[10] = {0};

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

	HGLOBAL hGlobal;
	LPTSTR temp;

	float startTime = timeMs();

	while(running) {

		if(timeMs() - startTime > 150) {


			if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(0x43)) {

				for(int i = 0x30; i < 0x39; i++) {
					int index = i - 0x30;
					if(GetAsyncKeyState(i)) {
						//std::cout << "Ctrl+C+" << i - 0x30 << std::endl;

						if(!OpenClipboard(hwnd)) {
							MessageBox(hwnd, "Error opening clipboard!", "Error", MB_OK);
							stop();
						}

						hGlobal = GetClipboardData(CF_TEXT);
						if(hGlobal) {
							temp = (LPTSTR)GlobalLock(hGlobal);

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
						MessageBox(hwnd, clipboardData[index], "Clipboard+", MB_OK);
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
