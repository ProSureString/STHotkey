#include <windows.h>
#include <iostream>


HHOOK g_hHook = NULL;

LRESULT CALLBACK LLKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
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