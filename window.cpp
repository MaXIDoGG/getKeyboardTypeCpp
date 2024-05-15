#include <windows.h>
#include <iostream>
#include <tchar.h>
#define CRT_SECURE_NO_WARNINGS
using namespace std;

const DWORD msg_len = 128;
TCHAR keyboard_type_msg[msg_len];


/**
 * \brief Функция потока
 * \param lp_param Указатель на параметр
 * \return Код завершения потока
 */
DWORD WINAPI ThreadProc(CONST LPVOID lp_param) {

	typedef int(*ImportFunction)(TCHAR*);
	ImportFunction KeyboardTypeDLL;
	HINSTANCE hinstLib = LoadLibrary(TEXT("KeyboardType.dll"));
	KeyboardTypeDLL = (ImportFunction)GetProcAddress(hinstLib, "KeyboardType");

	int res = KeyboardTypeDLL(keyboard_type_msg);

	ExitThread(0);
}

/**
 * \brief Функция оконной процедуры, обрабатывающая сообщения для окна
 * \param hWnd Дескриптор окна
 * \param uMsg Код сообщения
 * \param wParam Доп. информация о сообщении
 * \param lParam Доп. информация о сообщении
 * \return Результат обработки сообщения
 */
LRESULT CALLBACK WindowFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HANDLE h_thread;
	HFONT hFont = CreateFont(20, 0, 0, 0, FW_THIN, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Times New Roman"));
	switch (uMsg)
	{
		case WM_CREATE:
			h_thread = CreateThread(nullptr, 0, &ThreadProc, nullptr, 0, nullptr);
		    WaitForSingleObject(h_thread, INFINITE);
		    CloseHandle(h_thread);
			break;
		case WM_DESTROY:
			PostQuitMessage(EXIT_SUCCESS);
			break;
		case WM_PAINT:
			HDC hDC = BeginPaint(hWnd, &ps);
			SelectObject(hDC, hFont);
			TextOut(hDC, 20, 20, keyboard_type_msg, _tcslen(keyboard_type_msg));
			EndPaint(hWnd, &ps);
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
 * \brief Главная функция Windows-приложения
 * \param hInstance дескриптор экземпляра приложения
 * \param szCmdLine командная строка
 * \param nCmdShow флаги состояния окна
 * \return Возврат кода завершения
 */
int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
	MSG	msg{};
	HWND hwnd{};
	WNDCLASSEX wc{sizeof(WNDCLASSEX)};
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowFunc;
	wc.lpszClassName = L"WinAPI program";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc)) return EXIT_FAILURE;

	hwnd = CreateWindow(wc.lpszClassName, L"Get Keyboard Type", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, wc.hInstance, nullptr);
	if (hwnd == INVALID_HANDLE_VALUE) return EXIT_FAILURE;

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}
