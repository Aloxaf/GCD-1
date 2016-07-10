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

/*TODO:DrawTextEx*/
int GCD_text(int argc, char *argv[])
{
    if (argc == 1 || strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD Text text [/p x y] [/f front] [/z width height] [/b weight] [/i] [/u] [/s] [/fc R G B] [/bc R G B] [/r x y width height [/dp dp1 dp2 ...]]\n\n"
        "描述:\n    该命令允许用户绘制文字\n\n"
        "参数列表:\n    text\t\t   需要绘制的文字\n\n"
        "    /p    x y\t\t   绘制的坐标(默认0, 0)\n\n"
        "    /f    front\t\t   字体(默认宋体)\n\n"
        "    /z    width height     字的高度和宽度\n\n"
        "    /b    weight\t   字的粗细(0~1000) 400为普通,800为加粗\n\n"
        "    /i\t\t\t   斜体\n\n"
        "    /u\t\t\t   下划线\n\n"
        "    /s\t\t\t   删除线\n\n"
        "    /fc   R G B\t\t   前景色(默认白色)\n\n"
        "    /bc   R G B\t\t   背景色(默认透明)\n\n"
        "    /r    x y width height 限制文字所在的区域\n\n"
        "    /dp   dp\t\t   格式,可选值及含义如下\n\n"
        "\t  Top\t\t   顶对齐\n"
        "\t  Lef\t\t   左对齐\n"
        "\t  Center\t   居中\n"
        "\t  Right\t\t   右对齐\n"
        "\t  Vecenter\t   垂直居中\n"
        "\t  Bottom\t   底对齐\n"
        "\t  WordBreak\t   自动换行\n"
        "\t  SingleLine\t   单行文本\n"
        "\t  ExpandTabs\t   扩展TAB的长度\n"
        "\t  TabsTop\t   TAB后添加的字符数\n"
        "\t  NoPrefix\t   不处理'&',默认'&'后的字符会被添加下划线\n"
        "\t  HidePrefix\t   隐藏下划线\n"
        "\t  PrefixOnly\t   将'&'解释为前缀\n"
        "\t  EditControl\t   如果最后一行过长,将不被绘制\n"
        "\t  Path_Ellipsis\t   文本过长时,中间使用'...',但是会尽量保留最后一个\\后面的字符\n"
        "\t  End_Ellipsis\t   文本过长时,尾部使用'...'\n"
        "\t  ModifyString\t   省略显示,与Path_Ellipsis或End_Ellipsis配合使用\n"
        "\t  RtlReading\t   从右到左绘制文本\n"
        "\t  Word_Ellipsis\t   截断过长的单词,并使用'...'\n\n"
        );
    }
    
    int i;

    char DTParams[][25] = {"TOP","LEFT","CENTER","RIGHT","VCENTER","BOTTOM","WORDBREAK","SINGLELINE","EXPANDTABS","TABSTOP","","","","NOPREFIX","","EDITCONTROL","PATH_ELLIPSIS","END_ELLIPSIS","MODIFYSTRING","RTLREADING","WORD_ELLIPSIS","","HIDEPREFIX","PREFIXONLY"};
    
    FUNC_TEXT_INFO fti = {argv[1], "宋体", 0, 0, 0, 20, 0, RGB(0, 0, 0), RGB(255, 255, 255), true, false, false, false, false, {0, 0, 0, 0}, 0};

    for (i = 2; i < argc; ++i) {
        if (stricmp(argv[i], "/p") == 0) {
            if (!check_argv(argv, argc, i, 2)) {
                fprintf(stderr, "ERROR:'/p'开关需要接收两个参数!\n");
                return 1;
            }
            fti.x = atoi(argv[i + 1]);
            fti.y = atoi(argv[i + 2]);
            i += 2;
        } else if (stricmp(argv[i], "/z") == 0) {
            if (!check_argv(argv, argc, i, 2)) {
                fprintf(stderr, "ERROR:'/z'开个需要接收两个参数!\n");
                return 1;
            }
            fti.width  = atoi(argv[i + 1]);
            fti.height = atoi(argv[i + 2]);
            i += 2;
        } else if (stricmp(argv[i], "/b") == 0) {
            if (!check_argv(argv, argc, i, 1)) {
                fprintf(stderr, "ERROR:'/b'开关需要接收一个参数!\n");
                return 1;
            }
            fti.weight = atoi(argv[i + 1]);
            i += 1;
        } else if (stricmp(argv[i], "/f") == 0) {
            if (!check_argv(argv, argc, i, 1)) {
                fprintf(stderr, "ERROR:'/f'开关需要接收一个参数!\n");
                return 1;
            }
            fti.front = argv[i + 1];
            //strcpy(front, argv[i + 1]);
            i += 1;
        } else if (stricmp(argv[i], "/fc") == 0) {
            if (!check_argv(argv, argc, i, 3)) {
                fprintf(stderr, "ERROR:'/fc'开关需要接收三个参数!\n");
                return 1;
            }
            fti.clrFr = RGB(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]));
            i += 3;
        } else if (stricmp(argv[i], "/bc") == 0) {
            if (!check_argv(argv, argc, i, 3)) {
                fprintf(stderr, "ERROR:'/bc'开关需要接收三个参数!\n");
                return 1;
            }
            fti.bktran = false;
            fti.clrBk  = RGB(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]));
            i += 3;
        } else if (stricmp(argv[i], "/i") == 0) {
            fti.italic = true;
        } else if (stricmp(argv[i], "/u") == 0) {
            fti.underline = true;
        } else if (stricmp(argv[i], "/s") == 0) {
            fti.strikeout = true;
        } else if (stricmp(argv[i], "/r") == 0) {
            if (!check_argv(argv, argc, i, 4)) {
                fprintf(stderr, "ERROR:'/r'开关需要接收四个参数!\n");
                return 1;
            }
            fti.DrawText  = true;
            fti.rc.left   = atoi(argv[i + 1]);
            fti.rc.top    = atoi(argv[i + 2]);
            fti.rc.right  = atoi(argv[i + 3]);
            fti.rc.bottom = atoi(argv[i + 4]);
            i += 4;
        } else if (stricmp(argv[i], "/dp") == 0) {
            int j, k;
            for (j = i + 1; j < argc; ++j) {
                if (argv[j][0] == '/') {
                    break;
                }
                for (k = 0; k < 24; ++k) {
                    if (stricmp(argv[j], DTParams[k]) == 0)  {
                        fti.DTParams |= k <= 1 ? 0 : 1 << (k - 2);
                    }
                }
            }
        } else {
            fprintf(stderr, "ERROR:未知开关'%s'!\n", argv[i]);
            return 1;
        }
    }

    GCD_text_show(&fti);
    return 0;
}

void GCD_text_show(FUNC_TEXT_INFO *fti)
{
#if defined (__GNUC__)
    HMODULE hKernel32 = LoadLibrary("kernel32.dll");
    HWND hCMD = ((GETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow"))();
    FreeLibrary(hKernel32);
#else
    HWND hCMD = GetConsoleWindow();
#endif

    HDC     hdcDst    = GetDC(hCMD);
    //printf("%S", front);
    LOGFONT frontRC  = {fti->height, fti->width, 0, 0, fti->weight, fti->italic, fti->underline, fti->strikeout, GB2312_CHARSET, 0, 0, 0, 0, 0};
    wsprintf(frontRC.lfFaceName, "%s", TEXT(fti->front));

    HFONT   hFontNew = CreateFontIndirect(&frontRC);
    HFONT   hFontOld = SelectObject(hdcDst, hFontNew);

    SetBkMode(hdcDst, fti->bktran ? TRANSPARENT : OPAQUE);
    SetBkColor(hdcDst, fti->clrBk);
    COLORREF clr = SetTextColor(hdcDst, fti->clrFr);

    if (fti->DrawText) {
        DrawTextEx(hdcDst, TEXT(fti->text), -1, &fti->rc, fti->DTParams, NULL);
    } else {
        TextOut(hdcDst, fti->x, fti->y, TEXT(fti->text), lstrlen(TEXT(fti->text)));
    }

    SetTextColor(hdcDst, clr);
    SelectObject(hdcDst, hFontOld);
    ReleaseDC(hCMD, hdcDst);
}

