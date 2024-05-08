#include <iostream>
#include <windows.h>
using namespace std;

int keyboard_type, keyboard_buttons;

DWORD WINAPI ThreadProc(CONST LPVOID lp_param) {
    keyboard_type = GetKeyboardType(0);
    keyboard_buttons = GetKeyboardType(2);
    ExitThread(0);
}

VOID Error(const string& sz_message) {
    cout << sz_message;
    cout << "LastError = " << GetLastError();
    ExitProcess(0);
}

INT main() {

    HANDLE h_thread = CreateThread(nullptr, 0, &ThreadProc,nullptr, 0, nullptr);
    if (h_thread == nullptr) {
        Error("Failed to create thread.\r\n");
    }

    WaitForSingleObject(h_thread, INFINITE);
    cout << keyboard_type;

    CloseHandle(h_thread);
    ExitProcess(0);
}