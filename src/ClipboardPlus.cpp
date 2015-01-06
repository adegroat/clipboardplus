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

void ClipboardPlus::start() {
	if(running) return;
	running = true;

	for(int i = 0; i < 10; i++) {
		clipboardData[i] = "";
	}



	WindowSetup ws(hInstance, title, wProc);
	if(!ws.registerClass()){
		UIHandler::messageBox("Error registering window!");
		return;
	}

	mainWindow = ws.createWindow(title, width, height, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX);
	if(!mainWindow){
		UIHandler::messageBox("Error creating window!");
		return;
	}

	ShowWindow(mainWindow, nCmdShow);
	UpdateWindow(mainWindow);



	HINSTANCE kbHookLib = LoadLibrary("clipboardhook.dll");
	if(kbHookLib == NULL) {
		UIHandler::messageBox("Error loading clipboard hook!");
		stop();
		return;
	}

	HHOOK (*installHook)(HINSTANCE, HOOKPROC) = (HHOOK (*)(HINSTANCE, HOOKPROC))GetProcAddress(kbHookLib, "installHook");
	if(installHook == NULL) {
		UIHandler::messageBox("Error getting hook address!");
		stop();
		return;
	}

	kbHook = installHook(kbHookLib, kbProc);
	if(kbHook == NULL) {
		UIHandler::messageBox("Error installing hook!");
		stop();
		return;
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
	delete cbHandler;
	delete uiHandler;

	UnhookWindowsHookEx(kbHook);
	UnregisterHotKey(mainWindow, HOTKEY_SHOWWINDOW);
}

void ClipboardPlus::setupUI(HWND hwnd) {
	uiHandler = new UIHandler(hwnd, "");
	uiHandler->setupUI();
}

LRESULT CALLBACK ClipboardPlus::windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {

	case WM_CREATE:
	{
		setupUI(hwnd);
	} break;

	case WM_COMMAND:
	{
		if(HIWORD(wParam) == BN_CLICKED) {

			switch(LOWORD(wParam)) {

			case UIHandler::BTN_CLEAR:
				for(int i = 0; i < 10; i++) {
					clipboardData[i] = "";
					SetWindowText(uiHandler->getCBEditBox(i), "");
				}
				break;

			case UIHandler::BTN_HIDE:
				ShowWindow(hwnd, SW_HIDE);
				UIHandler::messageBox("Press Ctrl+F6 to show!", "Abracadabra!");
				break;

			case UIHandler::BTN_HELP:
			{
				int helpResponse = UIHandler::messageBox(
						"Press Ctrl+C+Number key to copy data to the respective clipboard.\nPress Ctrl+Number Key+V, in that order, to paste data.\n\nFor more detailed instructions, press the \"OK\" button below.",
						"Help", hwnd, MB_OKCANCEL | MB_ICONINFORMATION);

				if(helpResponse == IDOK) {
					//ShellExecute(NULL, "open", "http://google.com", NULL, NULL, SW_SHOW);
					ShellExecute(NULL, "open", "readme.txt", NULL, NULL, SW_SHOW);
				}
			} break;

			case UIHandler::BTN_CLEAR0:
			case UIHandler::BTN_CLEAR1:
			case UIHandler::BTN_CLEAR2:
			case UIHandler::BTN_CLEAR3:
			case UIHandler::BTN_CLEAR4:
			case UIHandler::BTN_CLEAR5:
			case UIHandler::BTN_CLEAR6:
			case UIHandler::BTN_CLEAR7:
			case UIHandler::BTN_CLEAR8:
			case UIHandler::BTN_CLEAR9:
			{
				int index = LOWORD(wParam) - 2000;
				clipboardData[index] = "";
				SetWindowText(uiHandler->getCBEditBox(index), "");
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

			if(ctrlDown && cDown && numKey != -1 && !done ) {
				int index = numKey - 0x30;

				std::string cbData = cbHandler->getClipboardText();
				cbHandler->emptyClipboard();

				if(dDown) {
					clipboardData[index].append(cbData);
				} else {
					clipboardData[index] = cbData;
				}

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
				SetWindowTextA(uiHandler->getCBEditBox(i), clipboardData[i].c_str());
			}
		} break;

	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}
