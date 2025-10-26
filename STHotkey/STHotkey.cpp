#include <windows.h>
#include <iostream>
#include <vector>
#include <string>


HHOOK g_hHook = NULL;
bool persistentMode = true;
const wchar_t combinging_long_stroke = 0x0336;


void sendWideChar(wchar_t ch) {
	INPUT in[2] = {};
	in[0].type = INPUT_KEYBOARD;
	in[0].ki.dwFlags = KEYEVENTF_UNICODE;
	in[0].ki.wScan = ch;

	in[1] = in[0];
	in[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

	SendInput(2, in, sizeof(INPUT));
}

int vkToUnicode(UINT vk, UINT scan, bool isKeyDown, wchar_t* outBuf, int bufSize) {
	BYTE keyState[256];
	if (!GetKeyboardState(keyState)) {
		return 0;
	}

	HKL layout = GetKeyboardLayout(0);
	return ToUnicodeEx(vk, scan, keyState, outBuf, bufSize, 0, layout);
}

LRESULT CALLBACK LLKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
			if (kb->vkCode == 'S') {
				if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_MENU) & 0x8000)) {
					persistentMode = !persistentMode;
					std::cout << "Persistent mode: " << (persistentMode ? "ON" : "OFF") << std::endl;
				
					return 1;
				}
			}


			//test | eats and spit out same char a-z
			wchar_t out[4] = {0};
			int ret = vkToUnicode(kb->vkCode, kb->scanCode, true, out, 4);
			if (ret > 0 && iswprint(out[0])) {
				sendWideChar(out[0]);
				return 1;
			}

			std::cout << "VK: " << kb->vkCode << " sc: " << kb->scanCode << std::endl;

		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

int main()
{
	g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyboardProc, GetModuleHandle(NULL), 0);
	if (!g_hHook)
	{
		std::cerr << "Failed to become a hooker. error code: " << GetLastError() << std::endl;
		return 1;
	}

	std::cout << "hook installed, press stuff to see vk codes,.. " << std::endl;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(g_hHook);
	return 0;
}