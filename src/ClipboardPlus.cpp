#include "ClipboardPlus.h"

ClipboardPlus::ClipboardPlus(HINSTANCE hInstance, WNDPROC wp, HOOKPROC kbHookProc, MSG message, LPSTR lpCmdLine, int nCmdShow) {
	this->hInstance = hInstance;
	this->wProc = wp;
	this->kbProc = kbHookProc;
	this->message = message;
	this->nCmdShow = nCmdShow;
	running = false;
	title = "Clipboard+";
	width = 500;
	height = 400;
	mainWindow = NULL;
	kbHook = NULL;
	this->ctrlDown = false;
	this->cDown = false;
	this->vDown = false;
	this->dDown = false;
	this->numKey = -1;
	cbHandler = new ClipboardHandler(mainWindow);
}

bool CALLBACK ClipboardPlus::setChildrenFontProc(HWND hwnd, LPARAM lParam) {
	SendMessage(hwnd, WM_SETFONT, (WPARAM)lParam, 0);
	return true;
}

void ClipboardPlus::start() {
	if(running) return;

	running = true;

	for(int i = 0; i < 10; i++) {
		clipboardData[i] = new char[1];
		memcpy(clipboardData[i], "", 1);
	}

	WindowSetup ws(hInstance, title, wProc);
	if(!ws.registerClass()){
		MessageBox(NULL, "Error registering window!", "Error", MB_OK);
		return;
	}
	mainWindow = ws.createWindow(title, width, height, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX);
	if(!mainWindow){
		MessageBox(NULL, "Error creating window!", "Error", MB_OK);
		return;
	}

	EnumChildWindows(mainWindow, (WNDENUMPROC)setChildrenFontProc, (LPARAM)font);

	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);

	HINSTANCE kbHookLib = LoadLibrary("kbhook.dll");
	if(kbHookLib == NULL) {
		MessageBox(mainWindow, "Error loading kbhook.dll", "Error", MB_OK);
		stop();
	}

	HHOOK (*installHook)(HINSTANCE, HOOKPROC) = (HHOOK (*)(HINSTANCE, HOOKPROC))GetProcAddress(kbHookLib, "installHook");
	if(installHook == NULL) {
		MessageBox(mainWindow, "Error getting hook address!", "Error", MB_OK);
		stop();
	}

	kbHook = installHook(kbHookLib, kbProc);
	if(kbHook == NULL) {
		MessageBox(mainWindow, "Error installing hook!", "Error", MB_OK);
		stop();
	}

	RegisterHotKey(mainWindow, HOTKEY_SHOWWINDOW, MOD_CONTROL, VK_F6);

	while(GetMessage(&message, mainWindow, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void ClipboardPlus::stop(const char message[128]) {
	if(!running) return;
	running = false;
	std::cout << "Message: " << message << "\nError: " << GetLastError() << std::endl;
	PostQuitMessage(0);
}

void ClipboardPlus::cleanUp() {
	for(int i = 0; i < 10; i++) {
		delete[] clipboardData[i];
	}
	delete cbHandler;

	UnhookWindowsHookEx(kbHook);
	UnregisterHotKey(mainWindow, HOTKEY_SHOWWINDOW);
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
					WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_AUTOHSCROLL,
					30, 40 + i * 28,
					430, 20,
					hwnd,
					(HMENU)i,
					NULL,
					NULL);

			CreateWindow(
					"BUTTON",
					"X",
					WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
					465, 40 + i * 28,
					20, 20,
					hwnd,
					(HMENU)(0x5550 + i),
					NULL,
					NULL);
		}

		CreateWindow(
				"BUTTON",
				"Clear All",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				30, 320,
				80, 25,
				hwnd,
				(HMENU)BTN_CLEAR,
				NULL,
				NULL);

		CreateWindow(
				"BUTTON",
				"Hide",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				120, 320,
				80, 25,
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
					clipboardData[i] = new char[1];
					memcpy(clipboardData[i], "", 1);
					SetWindowText(clipboardEditBox[i], "");
				}
				break;

			case BTN_HIDE:
				ShowWindow(hwnd, SW_HIDE);
				MessageBox(NULL, "Press Ctrl+F6 to show!", "Abracadabra!", MB_OK | MB_ICONINFORMATION);
				break;

			case BTN_CLEAR0:
			case BTN_CLEAR1:
			case BTN_CLEAR2:
			case BTN_CLEAR3:
			case BTN_CLEAR4:
			case BTN_CLEAR5:
			case BTN_CLEAR6:
			case BTN_CLEAR7:
			case BTN_CLEAR8:
			case BTN_CLEAR9:
			{
				int index = LOWORD(wParam) - 0x5550;
				clipboardData[index] = new char[1];
				memcpy(clipboardData[index], "", 1);
				SetWindowText(clipboardEditBox[index], "");
			} break;

			}
		}

	} break;

	case WM_HOTKEY:
		if(wParam == HOTKEY_SHOWWINDOW) {
			ShowWindow(hwnd, SW_SHOW);
		}
		break;

	case WM_DESTROY:
		cleanUp();
		stop();
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK ClipboardPlus::kbHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	int keyCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
	static bool done = false;

	switch(wParam) {
		case WM_KEYDOWN:
		{
			if(keyCode == VK_LCONTROL) ctrlDown = true;
			if(keyCode == 0x43) cDown = true;
			if(keyCode == 0x56) vDown = true;
			if(keyCode == 0x44) dDown = true;
			if(keyCode >= 0x30 && keyCode <= 0x39) numKey = keyCode;

			if(ctrlDown && cDown && numKey != -1 && !done) {
				int index = numKey - 0x30;

				std::string cbData = cbHandler->getClipboardText();

				if(dDown) {
					strcat(clipboardData[index], cbData.c_str());
				} else {
					clipboardData[index] = new char[cbData.length()];
					memcpy(clipboardData[index], cbData.c_str(), cbData.length() + 1);
				}

				cbHandler->emptyClipboard();
				done = true;
			}

			if(ctrlDown && numKey != -1 && !cDown) {
				int index = numKey - 0x30;

				cbHandler->setClipboardText(clipboardData[index]);
			}

		} break;

		case WM_KEYUP:
		{
			if(keyCode == VK_LCONTROL) ctrlDown = false;
			if(keyCode == 0x43) cDown = false;
			if(keyCode == 0x56) vDown = false;
			if(keyCode == 0x44) dDown = false;
			numKey = -1;
			done = false;

			for(int i = 0; i < 10; i++){
				SetWindowTextA(clipboardEditBox[i], clipboardData[i]);
			}
		} break;

	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
