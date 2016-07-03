/*
GCD 1.0
Copyright (C) 2016  aiwozhonghuaba

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "GCD.h"

bool GCD_Mouse_Exit = FALSE;

int GCD_mouse(int argc, char *argv[])
{

    if (1 == argc||strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD mouse [/b button] [/t timeout] [/c]\n"
        "GCD mouse [[/hide] | [/reset] | [/g x y]]\n\n"
        "描述:\n    该命令提供了对控制台光标的操作\n\n"
        "参数列表:\n    /b     button\t获取鼠标单击事件,1-左键(缺省值),2-右键\n\n"
        "    /t     timeout\t设定超时毫秒数\n\n"
        "    /c\t\t\t在做出单击之前禁止鼠标移出控制台窗口\n\n"
        "    /hide\t\t隐藏控制台光标\n\n"
        "    /show\t\t重置控制台光标\n\n"
        "    /g     x y\t\t将光标跳转到指定位置\n\n"
        );
        return 0;
    }
    if (stricmp(argv[1], "/hide") == 0) {
        return GCD_mouse_set(25, false);
    } else if (stricmp(argv[1], "/show") == 0) {
        return GCD_mouse_set(25, true);
    } else if (stricmp(argv[1], "/g") == 0) {
        return GCD_mouse_goto(atoi(argv[2]), atoi(argv[3]));

    }
    int i, button = 1, timeout = -1;
    bool clip = false;

    for (i = 1; i < argc; ++i) {
        if (stricmp(argv[i], "/b") == 0) {
            if (argc > i + 1 - 1 /*&& argv[i + 1][0] != '/'*/) {
                button = atoi(argv[i + 1]);
                ++i;
            } else {
                button = FROM_LEFT_1ST_BUTTON_PRESSED;
            }
        } else if (stricmp(argv[i], "/t") == 0) {
            timeout = atoi(argv[i + 1]);
            ++i;
        } else if (stricmp(argv[i], "/c") == 0) {
            clip = true;
        }
    }

    return GCD_mouse_click(button, timeout, clip);
}

int GCD_mouse_click(int clickMode, int Delay, bool clip)
{
    HANDLE StdInput  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hDelay;
    
    if (clip) {

        char ConsoleTitle[MAX_TITLE_LENGTH];
        WINDOWPLACEMENT wndpl;

        GetConsoleTitle(ConsoleTitle, MAX_TITLE_LENGTH);
        HWND hConsole = FindWindow(NULL, ConsoleTitle);
        GetWindowPlacement(hConsole, &wndpl);
        ClipCursor(&wndpl.rcNormalPosition);
    }

    DWORD OldMode, Res;
    INPUT_RECORD ir;
    CONSOLE_CURSOR_INFO oldCCI, newCCI;
    
    GetConsoleCursorInfo(StdOutPut, &oldCCI);
    newCCI.bVisible = oldCCI.bVisible;
    newCCI.dwSize   = 100;

    MOUSE_INFO mouse_info = {Delay, oldCCI.dwSize, oldCCI.bVisible};
    if (Delay > 0) {
        //printf("%d", Delay);
        hDelay = (HANDLE)_beginthreadex(NULL, 0, (unsigned (__stdcall *) (void *))GCD_delay_2, (void *)&mouse_info, 0, NULL);
    }
    
    GetConsoleMode(StdInput, &OldMode);
    SetConsoleMode(StdInput, OldMode | ENABLE_MOUSE_INPUT);
    SetConsoleCursorInfo(StdOutPut, &newCCI);
    
    for (; ; ) {
        ReadConsoleInput(StdInput, &ir, 1, &Res);
        SetConsoleCursorPosition(StdOutPut, ir.Event.MouseEvent.dwMousePosition);
        
        if (MOUSE_CLICK == ir.Event.MouseEvent.dwEventFlags && clickMode == ir.Event.MouseEvent.dwButtonState) {
            CloseHandle(hDelay);
            SetConsoleMode(StdInput, OldMode);
            SetConsoleCursorInfo(StdOutPut, &oldCCI);
            if (clip) {
                ClipCursor(NULL);
            }
            return ir.Event.MouseEvent.dwMousePosition.X * 1000 + ir.Event.MouseEvent.dwMousePosition.Y;
        }
    }
}

bool GCD_mouse_goto(int x, int y)
{
    HANDLE StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD  curPos    = {x, y};

    return SetConsoleCursorPosition(StdOutPut, curPos);
}

bool GCD_mouse_set(int size, bool visible)
{
    HANDLE StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_CURSOR_INFO cci = {size, visible};
    return SetConsoleCursorInfo(StdOutPut, &cci);
}

void GCD_delay_2(void *mouse_info)
{
#define mi ((MOUSE_INFO *)(mouse_info))
    Sleep(mi->Delay);
    GCD_mouse_set(mi->cci.dwSize, mi->cci.bVisible);
    ClipCursor(NULL);
    exit(-1);
}
