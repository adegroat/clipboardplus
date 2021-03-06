#include "ClipboardPlus.h"

const std::string ClipboardPlus::VERSION = "1.0";

ClipboardPlus::ClipboardPlus(HINSTANCE hInstance, WNDPROC wp, HOOKPROC kbHookProc, MSG message, LPSTR lpCmdLine, int nCmdShow) {
	this->hInstance = hInstance;
	this->wProc = wp;
	this->kbProc = kbHookProc;
	this->message = message;
	this->nCmdShow = nCmdShow;
	title = std::string("Clipboard+ (").append(VERSION).append(")");
	width = 500;
	height = 410;
	mainWindow = NULL;
	kbHook = NULL;
	this->ctrlDown = false;
	this->cDown = false;
	this->vDown = false;
	this->numKey = -1;
	cbHandler = new ClipboardHandler(mainWindow);
}

void ClipboardPlus::start() {
	Settings::initSettings();

	WindowSetup ws(hInstance, title.c_str(), wProc);
	if(!ws.registerClass()){
		UIHandler::messageBox("Error registering window!");
		return;
	}

	mainWindow = ws.createWindow(title.c_str(), width, height, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX);
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
	for(int i = 0; i < 10; i++) {
		RegisterHotKey(mainWindow, HOTKEY_CTRLNUM + i, MOD_CONTROL, 0x30 + i);
	}

	while(GetMessage(&message, mainWindow, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void ClipboardPlus::stop(const char message[128]) {
	std::cout << "Message: " << message << "\nError: " << GetLastError() << std::endl;
	PostQuitMessage(0);
}

void ClipboardPlus::cleanUp() {
	NOTIFYICONDATA nid;
	nid.uID = UIHandler::CBP_TRAY_ICON;
	Shell_NotifyIcon(NIM_DELETE, &nid);

	savePersistentData();

	delete cbHandler;
	delete uiHandler;

	UnhookWindowsHookEx(kbHook);
	UnregisterHotKey(mainWindow, HOTKEY_SHOWWINDOW);
	for(int i = 0; i < 10; i++) {
		UnregisterHotKey(mainWindow, HOTKEY_CTRLNUM + i);
	}
}

void ClipboardPlus::savePersistentData() {
	SettingsHandler::setKey("standardPaste", Settings::standardPaste ? "1" : "0");
	SettingsHandler::setKey("showBalloonTips", Settings::showBalloonTips ? "1" : "0");
	for(int i = 0; i < 10; i++) {
		SettingsHandler::setClipboard(i, clipboardData[i]);
	}
}

void ClipboardPlus::setupUI(HWND hwnd) {
	uiHandler = new UIHandler(hwnd, "");
	uiHandler->setupUI();

	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uVersion = 4;
	nid.uID = UIHandler::CBP_TRAY_ICON;
	nid.hIcon = UIHandler::CBP_ICON;
	nid.hWnd = hwnd;
	nid.uFlags = NIF_ICON | NIF_MESSAGE;
	nid.dwInfoFlags = NIIF_USER | NIIF_NOSOUND;
	nid.uCallbackMessage = UIHandler::CBP_ICON_MESSAGE;

	Shell_NotifyIcon(NIM_ADD, &nid);
	Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

LRESULT CALLBACK ClipboardPlus::windProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {

		case UIHandler::CBP_ICON_MESSAGE:
		{

			switch(LOWORD(lParam)) {
				case WM_LBUTTONUP:
					ShowWindow(hwnd, SW_SHOW);
					break;
			}

		} break;
		case WM_CREATE:
		{
			setupUI(hwnd);

			for(int i = 0; i < 10; i++) {
				std::string cbContent = SettingsHandler::getClipboardContent(i);
				clipboardData[i] = cbContent;
				SetWindowTextA(uiHandler->getCBEditBox(i), cbContent.c_str());
			}
		} break;

		case WM_COMMAND:
		{
			if(HIWORD(wParam) == BN_CLICKED) {
				switch(LOWORD(wParam)) {

					case UIHandler::BTN_CLEAR:
					{
						for(int i = 0; i < 10; i++) {
							clipboardData[i] = "";
							SetWindowText(uiHandler->getCBEditBox(i), "");
							SettingsHandler::clearAllClipboards();
						}
					} break;

					case UIHandler::BTN_HELP:
					{
						int helpResponse = UIHandler::messageBox(
								"Press Ctrl+C+NUMBER_KEY to copy data to the respective clipboard.\n"
								"Press Ctrl+V+NUMBER_KEY to paste data.\n"
								"If you have standard paste enabled, press Ctrl+NUMBER_KEY+V, in that order, to paste."
								"\n\nFor more detailed instructions, press the \"OK\" button below.",
								"Help", hwnd, MB_OKCANCEL | MB_ICONINFORMATION);

						if(helpResponse == IDOK) {
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

						SettingsHandler::clearClipboard(index);
					} break;

					case UIHandler::BTN_STD_PASTE:
						Settings::standardPaste = !Settings::standardPaste;
						cbHandler->emptyClipboard();
						break;

					case UIHandler::BTN_BALLOON_TIPS:
						Settings::showBalloonTips = !Settings::showBalloonTips;
						break;
				}
			}

		} break;

		case WM_QUERYENDSESSION:
			cleanUp();
			stop("Computer shutting down.");
			break;

		case WM_SYSCOMMAND:
			if(wParam == SC_MINIMIZE){
				ShowWindow(hwnd, SW_HIDE);
				uiHandler->showBaloonTip("Clipboard+ Hidden", "Click the Clipboard+ icon to show!");
			}
			else return DefWindowProc(hwnd, message, wParam, lParam);
			break;

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
	if(nCode < 0) return CallNextHookEx(NULL, nCode, wParam, lParam);

	int keyCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
	static bool doneCopy = false;
	static bool donePaste = false;

	switch(wParam) {
		case WM_KEYDOWN:
		{
			if(keyCode == VK_LCONTROL || keyCode == VK_RCONTROL) ctrlDown = true;
			if(keyCode == 0x43) cDown = true;
			if(keyCode == 0x56) vDown = true;
			if(keyCode >= 0x30 && keyCode <= 0x39) numKey = keyCode;

			if(ctrlDown && cDown && numKey != -1 && !doneCopy) {
				int index = numKey - 0x30;

				std::string cbData = cbHandler->getClipboardText();
				cbHandler->emptyClipboard();

				clipboardData[index] = cbData;

				if(Settings::showBalloonTips) {
					char numStr[2] = {(char)numKey, '\0'};
					std::string balloonTitle = std::string("Text copied to clipboard #").append(numStr);
					std::string balloonInfo = clipboardData[index].length() > 30 ? clipboardData[index].substr(0, 30).append("...") : clipboardData[index];
					uiHandler->showBaloonTip(balloonTitle, balloonInfo);
				}
				doneCopy = true;
			}

			if(Settings::standardPaste) {
				if(ctrlDown && numKey != -1 && !cDown) {
					int index = numKey - 0x30;
					cbHandler->setClipboardText(clipboardData[index]);
				}
			}
			else {
				if(ctrlDown && vDown) {
					if(IsClipboardFormatAvailable(CF_TEXT))
						cbHandler->emptyClipboard();

					if(numKey != -1){
						int index = numKey - 0x30;

						cbHandler->setClipboardText(clipboardData[index]);

						if(!donePaste){
							donePaste = true;
							INPUT input[2];
							for(int i = 0; i < 2; i++) {
								input[i].type = INPUT_KEYBOARD;
								input[i].ki.dwFlags = 0;
								input[i].ki.time = 0;
								input[i].ki.wScan = 0;
							}
							input[0].ki.wVk = 0x56;
							input[1].ki.dwFlags = KEYEVENTF_KEYUP;
							input[1].ki.wVk = 0x56;
							SendInput(2, input, sizeof(INPUT));
						}
					}

				}
			}

		} break; // WM_KEYDOWN

		case WM_KEYUP:
		{
			if(keyCode == VK_LCONTROL || keyCode == VK_RCONTROL) ctrlDown = false;
			if(keyCode == 0x43) cDown = false;
			if(keyCode == 0x56) vDown = false;

			if(keyCode >= 0x30 && keyCode <= 0x39){
				donePaste = false;
			}

			numKey = -1;
			doneCopy = false;

			for(int i = 0; i < 10; i++){
				SetWindowTextA(uiHandler->getCBEditBox(i), clipboardData[i].c_str());
			}
		} break;

	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

