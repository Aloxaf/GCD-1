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
extern HANDLE StdOutPut;
int GCD_syba(int argc, char *argv[])
{
    int i;
    if (argc == 1 || strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD syba [cmd | [/v var]]\n\n"
        "描述:\n    该命令可以批量输出彩色文字\n\n"
        "参数列表:\n    cmd\t\t\tsyba命令\n\n"
        "    /v   var1 var2 ...  从变量中读取命令执行\n\n"
        "syba命令格式:\n"
        "    :xxxxxx \t\t坐标,前三位为X,后三位为Y\n"
        "    $xx\t\t\t指定颜色\n"
        "    ;\t\t\t换行\n"
        "    #\t\t\t不输出字符,只改变当前位置的字符的颜色\n\n"
        );
    } else if (stricmp(argv[1], "/v") == 0) {
        for (i = 2; i < argc; ++i) {
            GCD_syba_core(getenv(argv[2]));
        }
    } else {
        for (i = 1; i < argc; ++i) {
            GCD_syba_core(argv[i]);
        }
    }
    return 0;
}

void GCD_syba_core(char *cmdline)
{
    //HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    unsigned int length = strlen(cmdline);
    unsigned int i;

    char color[] = "07";
    char coX[] = "000";
    char coY[] = "000";
    char tmp;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(StdOutPut, &csbi);
    COORD co = {csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y};
    WORD  oldAttr = csbi.wAttributes;
    
    for (i = 0; i < length; ++i) {
        switch (cmdline[i]) {// :000000123456$CF123456
            case ':':
                tmp = cmdline[i + 4];
                cmdline[i + 4] = '\0';
                co.X = atoi(&cmdline[i + 1]);
                cmdline[i + 4] = tmp;

                tmp = cmdline[i + 7];
                cmdline[i + 7] = '\0';
                co.Y = atoi(&cmdline[i + 4]);
                cmdline[i + 7] = tmp;
                SetConsoleCursorPosition(StdOutPut, co);
                i += 6;
                break;
            case ';':
                co.Y += 1;
                SetConsoleCursorPosition(StdOutPut, co);
                break;         
            case '$':
                color[0] = cmdline[i + 1];
                color[1] = cmdline[i + 2];
                SetConsoleTextAttribute(StdOutPut, strtol(color, NULL, 16));
                i += 2;
                break;
            case '`':
                if (cmdline[i + 1] == '`' && cmdline[i + 2] == '`') {
                    printf(&cmdline[i + 3]);
                    SetConsoleTextAttribute(StdOutPut, oldAttr);
                    return;
                }
                putchar(cmdline[i + 1]);
                i += 1;
                break;
            case '#':
                GetConsoleScreenBufferInfo(StdOutPut, &csbi);
                FillConsoleOutputAttribute(StdOutPut, strtol(color, NULL, 16), 1, csbi.dwCursorPosition, NULL);
                break;
            default:
                putchar(cmdline[i]);
                break;
        }

    }
    SetConsoleTextAttribute(StdOutPut, oldAttr);
    return;

}
