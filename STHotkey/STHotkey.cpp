#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
	if (!RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_ALT, 0x53)) // S is 0x53 i thnk
	{
		cerr << "Failed to register hotkey. error code: " << GetLastError() << endl;
		return 1;
	}

	cout << "Hotkey reg success, ctrl alt s to trigger. listening,.. " << endl;

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			cout << "triggerded hotkey1 !!" << endl;
		}
	}

	UnregisterHotKey(NULL, 1);
	return 0;
}