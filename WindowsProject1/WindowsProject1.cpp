#include "functions.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
RECT clientRect;
HWND hWnd;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    getCommandLine,
                     _In_ int       nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(getCommandLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nShowCmd))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 10, 10, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);
   SetTimer(hWnd, 1, 250, NULL);
   setupGame();

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{   
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_KEYDOWN: // если нажата клавиша
        if (!menu_of_game_bool) {
            switch (wParam)
            {
            case VK_LEFT: // стрелка ВЛЕВО
                if (direction != 'R') {
                    direction = 'L';
                }
                break;
            case VK_RIGHT: // стрелка ВПРАВО
                if (direction != 'L') {
                    direction = 'R';
                }
                break;
            case VK_UP: // стрелка ВВЕРХ
                if (direction != 'D') {
                    direction = 'U';
                }
                break;
            case VK_DOWN: // стрелка ВНИЗ
                if (direction != 'U') {
                    direction = 'D';
                }
                break;
            }
        }
        else {
            switch (wParam)
            {
            case VK_DOWN:
                if (current_menu_item == 4) {
                    current_menu_item = 1;
                }
                else {
                    current_menu_item++;
                }
                break;
            case VK_UP:
                if (current_menu_item == 1) {
                    current_menu_item = 4;
                }
                else {
                    current_menu_item--;
                }
                break;
            case VK_RETURN:
                switch (current_menu_item)
                {
                case 1:
                    menu_of_game_bool = FALSE;
                    setRecord(0);
                    break;
                case 2:
                    menu_of_game_bool = FALSE;
                    break;
                case 3:
                    break;
                case 4:
                    PostQuitMessage(0);
                    break;
                default:
                    break;
                }
            }
        }
        break;

    case WM_TIMER:
    {
        switch (wParam)
        {
        case 1:
            InvalidateRect(hWnd, NULL, FALSE);
            logic();
            if (game_over_bool) {
                KillTimer(hWnd, 1);
                SetTimer(hWnd, 2, 400, NULL);
            }
            break;
            return 0;

        case 2:
            if (countT < 6) {
                InvalidateRect(hWnd, NULL, FALSE);
                countT += 1;
            }
            else {
                countT = 0;
                KillTimer(hWnd, 2);
                SetTimer(hWnd, 1, 250, NULL);
                setupGame();
            }
            return 0;
        }
    }

    case WM_PAINT:
        Draw(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}