#include <windows.h>
#include <iostream>

int main()
{
	if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 0x53)) // S is 0x53 i thnk
	{
		std::cerr << "Failed to register hotkey. error code: " << GetLastError() << std::endl;
		return 1;
	}

	std::cout << "Hotkey reg success, ctrl alt s to trigger. listening,.. " << std::endl;

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			std::cout << "triggerded hotkey1 !!" << std::endl;
		}
	}

	UnregisterHotKey(NULL, 1);
	return 0;
}