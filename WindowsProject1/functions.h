#include "framework.h"
#include "WindowsProject1.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <conio.h> 
#include <stdlib.h>
#include <time.h> 
#include <Windows.h>
#include <string>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

// Глобальные переменные
int fruit_coordinate_x, fruit_coordinate_y;
int windowWidth = 640, windowHeight = 640, score = 1;
char direction;
int coordinates_of_tail_x[100];
int coordinates_of_tail_y[100];
RECT background_rect = { 0, 0, windowWidth, windowHeight };
RECT gameField_rect = { 40, 20, windowWidth - 20, windowHeight - 20 };
bool game_over_bool = FALSE;
bool menu_of_game_bool = TRUE;
char buf[256];
int countT = 0;
int current_menu_item = 1;

// Функция рандомного числа
int getRandomNum(int initial_border, int final_border) {
    int num = initial_border + rand() % final_border;
    return num;
}

int GetRandomCoordinateX() {
    while (1) {
        int rnd_n = getRandomNum(0, 39);
        if (rnd_n % 2 != 0) {
            int coord = 15 * rnd_n;
            bool flag = TRUE;
            for (int i = 0; i < score; i++) {
                if (coordinates_of_tail_x[i] == coord) {
                    flag = FALSE;
                }
            }
            if (flag) {
                return coord;
            }
        }
    }   
}

int GetRandomCoordinateY() {
    while (1) {
        int rnd_n = getRandomNum(9, 29);
        if (rnd_n % 2 != 0) {
            int coord = 15 * rnd_n;
            bool flag = TRUE;
            for (int i = 0; i < score; i++) {
                if (coordinates_of_tail_y[i] == coord) {
                    flag = FALSE;
                }
            }
            if (flag) {
                return coord;
            }
        }
    }
}

int setRecord(int record)
{
    FILE* file;
    if ((file = fopen("sources/record/max_score.txt", "w")) == NULL)
    {
        printf("NO FILE\n");
        return 0;
    }
    else {
        fprintf(file, "%d", record);
        fclose(file);
        return 1;
    }
}
int a;
int getRecord()
{
    FILE* file;
    if ((file = fopen("sources/record/max_score.txt", "r")) == NULL)
    {
        printf("NO FILE\n");
        return 0;
    }
    else {
        while ((fgets(buf, 256, file)) != NULL)
        {
            a = atoi(buf);
        }
        fclose(file);
        return a;
    }
}

void logic() {
    // Координаты в массиве сдигаются на одну ячейку назад, освобождая место для первой
    for (int i = score - 1; i > 0; i--) {
        coordinates_of_tail_x[i] = coordinates_of_tail_x[i - 1]; 
        coordinates_of_tail_y[i] = coordinates_of_tail_y[i - 1];
    }
    // Первая ячейка заполняется новой координатой
    switch (direction) {
    case 'L': 
        coordinates_of_tail_x[0] -= 30;
        break;
    case 'R':
        coordinates_of_tail_x[0] += 30;
        break;
    case 'U':
        coordinates_of_tail_y[0] -= 30;
        break;
    case 'D':
        coordinates_of_tail_y[0] += 30;
        break;
    }

    // Если попали на фрукт
    if (coordinates_of_tail_x[0] == fruit_coordinate_x && coordinates_of_tail_y[0] == fruit_coordinate_y) {
        score += 1;
        //coordinates_of_tail_x[score] = -30;
        fruit_coordinate_x = GetRandomCoordinateX();
        fruit_coordinate_y = GetRandomCoordinateY();
    }

    // проверка не съеден ли хвост
    bool flag = FALSE;
    for (int i = 1; i < score; i++) {
        if (coordinates_of_tail_x[0] == coordinates_of_tail_x[i] && coordinates_of_tail_y[0] == coordinates_of_tail_y[i]) {
            flag = TRUE;
        }
    }
    if (flag) {
        game_over_bool = TRUE;
        if (getRecord() < score) {
            setRecord(score);
        }
    }


    // Если вышли за границы поля
    if (coordinates_of_tail_x[0] > 640) {
        coordinates_of_tail_x[0] = 15;
    }
    if (coordinates_of_tail_x[0] < 0) {
        coordinates_of_tail_x[0] = 645;
    }
    if (coordinates_of_tail_y[0] > 640) {
        coordinates_of_tail_y[0] = 105;
    }
    if (coordinates_of_tail_y[0] < 105) {
        coordinates_of_tail_y[0] = 645;
    }
}

// Инициализация игры
void setupGame() {
    game_over_bool = FALSE;
    menu_of_game_bool = TRUE;
    score = 1;
    direction = NULL;
    memset(coordinates_of_tail_x, NULL, 100);
    memset(coordinates_of_tail_y, NULL, 100);

    srand(time(NULL));
    coordinates_of_tail_x[0] = 285;
    coordinates_of_tail_y[0] = 345;

    fruit_coordinate_x = GetRandomCoordinateX();
    fruit_coordinate_y = GetRandomCoordinateY();
}


// Отрисовка
void Draw(HWND hWnd) {
    // Основная часть игры
    if (!menu_of_game_bool) {
        PAINTSTRUCT ps;
        RECT rectangle_SNAKE_GAME = { 0, 0, windowWidth, 100 }, rectangle_score_text = { 80, 120, windowWidth , 50 };

        HDC hdc = BeginPaint(hWnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc); // создает контекст памяти устройства
        HBITMAP memBM = CreateCompatibleBitmap(hdc, windowWidth, windowHeight); // создает битмап
        SelectObject(memDC, memBM);

        // Фон игры
        HBRUSH hBrush;
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(memDC, hBrush);
        FillRect(memDC, &background_rect, hBrush);

        if (countT % 2 == 0) {
            // Змейка
            hBrush = CreateSolidBrush(RGB(34, 139, 34));
            SelectObject(memDC, hBrush);
            for (int i = 0; i < score; i++) {
                if (coordinates_of_tail_x[i] != 0) {
                    Rectangle(memDC, coordinates_of_tail_x[i] - 15, coordinates_of_tail_y[i] - 15, coordinates_of_tail_x[i] + 15, coordinates_of_tail_y[i] + 15);
                }
            }
        }


        // Верхняя граница игрового поля
        HPEN hPen;
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        SelectObject(memDC, hPen);
        MoveToEx(memDC, 0, 89, NULL);
        LineTo(memDC, 640, 89);
        hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
        SelectObject(memDC, hPen);


        // Текст
        // Вывод SNAKE GAME
        HFONT Font;
        SetTextColor(memDC, RGB(255, 255, 255));
        SetBkColor(memDC, RGB(0, 0, 0));
        Font = (HFONT)SelectObject(memDC, CreateFont(40, 25, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"));
        if (game_over_bool) {
            DrawTextA(memDC, "GAME OVER", sizeof("GAME OVER"), &rectangle_SNAKE_GAME, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            DeleteObject(Font);
        }
        else {
            DrawTextA(memDC, "SNAKE GAME", sizeof("SNAKE GAME"), &rectangle_SNAKE_GAME, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            DeleteObject(Font);
        }
        

        // Score:
        Font = (HFONT)SelectObject(memDC, CreateFont(25, 10, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"));
        char str[15];
        sprintf_s(str, "Score: %d", score);
        TCHAR strT[15];
        OemToCharBuff(str, strT, 15);
        TextOut(memDC, 10, 60, (LPCWSTR)strT, _tcslen(strT));

        // Best Score:
        sprintf_s(str, "Best: %d", getRecord());
        OemToCharBuff(str, strT, 15);
        TextOut(memDC, 500, 60, (LPCWSTR)strT, _tcslen(strT));
        DeleteObject(Font);


        // Фрукт
        SelectObject(memDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(memDC, RGB(128, 0, 0));
        Rectangle(memDC, fruit_coordinate_x - 15, fruit_coordinate_y - 15, fruit_coordinate_x + 15, fruit_coordinate_y + 15);
        BitBlt(hdc, 0, 0, windowWidth, windowHeight, memDC, 0, 0, SRCCOPY);
        DeleteDC(memDC);
        DeleteObject(memBM);

        EndPaint(hWnd, &ps);
    }



    // Если мы в меню
    else {
        PAINTSTRUCT ps;
        RECT rectangle_SNAKE_GAME = { 0, 0, windowWidth, 100 }, rectangle_score_text = { 80, 120, windowWidth , 50 };

        HDC hdc = BeginPaint(hWnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc); // создает контекст памяти устройства
        HBITMAP memBM = CreateCompatibleBitmap(hdc, windowWidth, windowHeight); // создает битмап
        SelectObject(memDC, memBM);

        // Фон игры
        HBRUSH hBrush;
        hBrush = CreateSolidBrush(RGB(0, 0, 0));
        SelectObject(memDC, hBrush);
        FillRect(memDC, &background_rect, hBrush);


        // Верхняя граница игрового поля
        HPEN hPen;
        hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
        SelectObject(memDC, hPen);
        MoveToEx(memDC, 0, 89, NULL);
        LineTo(memDC, 640, 89);
        hPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
        SelectObject(memDC, hPen);


        // Текст
        // Вывод SNAKE GAME
        HFONT Font;
        SetTextColor(memDC, RGB(255, 255, 255));
        SetBkColor(memDC, RGB(0, 0, 0));
        Font = (HFONT)SelectObject(memDC, CreateFont(40, 25, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"));
        DrawTextA(memDC, "SNAKE GAME", sizeof("SNAKE GAME"), &rectangle_SNAKE_GAME, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DeleteObject(Font);

        // Меню
        RECT new_game_rect = { 0, 200, windowWidth, 250 }, continue_rect = { 0, 250, windowWidth, 300 }, about_rect = { 0, 300, windowWidth, 350 }, exit_rect = {0, 350, windowWidth, 400};
        Font = (HFONT)SelectObject(memDC, CreateFont(25, 10, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial"));
        DrawTextA(memDC, "NEW GAME", sizeof("NEW GAME"), &new_game_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawTextA(memDC, "CONTINUE", sizeof("CONTINUE"), &continue_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawTextA(memDC, "ABOUT", sizeof("ABOUT"), &about_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        DrawTextA(memDC, "EXIT", sizeof("EXIT"), &exit_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        switch (current_menu_item)
        {
            case 1:
                SetTextColor(memDC, RGB(0, 0, 0));
                SetBkColor(memDC, RGB(255, 255, 255));
                DrawTextA(memDC, "NEW GAME", sizeof("NEW GAME"), &new_game_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                SetTextColor(memDC, RGB(255, 255, 255));
                SetBkColor(memDC, RGB(0, 0, 0));
                break;
            case 2:
                SetTextColor(memDC, RGB(0, 0, 0));
                SetBkColor(memDC, RGB(255, 255, 255));
                DrawTextA(memDC, "CONTINUE", sizeof("CONTINUE"), &continue_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                SetTextColor(memDC, RGB(255, 255, 255));
                SetBkColor(memDC, RGB(0, 0, 0));
                break;
            case 3:
                SetTextColor(memDC, RGB(0, 0, 0));
                SetBkColor(memDC, RGB(255, 255, 255));
                DrawTextA(memDC, "ABOUT", sizeof("ABOUT"), &about_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                SetTextColor(memDC, RGB(255, 255, 255));
                SetBkColor(memDC, RGB(0, 0, 0));
                break;
            case 4:
                SetTextColor(memDC, RGB(0, 0, 0));
                SetBkColor(memDC, RGB(255, 255, 255));
                DrawTextA(memDC, "EXIT", sizeof("EXIT"), &exit_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                SetTextColor(memDC, RGB(255, 255, 255));
                SetBkColor(memDC, RGB(0, 0, 0));
                break;
            default:
                break;
        }
        

        BitBlt(hdc, 0, 0, windowWidth, windowHeight, memDC, 0, 0, SRCCOPY);
        DeleteDC(memDC);
        DeleteObject(memBM);

        EndPaint(hWnd, &ps);
    }
    
}