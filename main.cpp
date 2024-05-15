#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>
#include <windows.h>

/**
 * \brief Функция узнаёт тип клавиатуры пользователя
 * \param keyboard_type_msg Строка с сообщением о типе клавиатуры
 * \return Код возврата
 */
extern "C" _declspec(dllexport) int KeyboardType(TCHAR* keyboard_type_msg)
{
    int keyboard_type = GetKeyboardType(0);
    const TCHAR* keyboard_type_str;

    switch (keyboard_type)
    {
    case 4:
        keyboard_type_str = L"Улучшенные 101- или 102-клавишные клавиатуры (и совместимые)";
        break;
    case 7:
        keyboard_type_str = L"Японская клавиатура";
        break;
    case 8:
        keyboard_type_str = L"Корейская клавиатура";
        break;
    default:
        keyboard_type_str = L"Неизвестный тип клавиатуры";
        break;
    }

    swprintf(keyboard_type_msg, L"Тип вашей клавиатуры: %d (%s)", keyboard_type, keyboard_type_str);
    return EXIT_SUCCESS;
}