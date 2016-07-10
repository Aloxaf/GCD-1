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
        "����:\n    �����������û���������\n\n"
        "�����б�:\n    text\t\t   ��Ҫ���Ƶ�����\n\n"
        "    /p    x y\t\t   ���Ƶ�����(Ĭ��0, 0)\n\n"
        "    /f    front\t\t   ����(Ĭ������)\n\n"
        "    /z    width height     �ֵĸ߶ȺͿ��\n\n"
        "    /b    weight\t   �ֵĴ�ϸ(0~1000) 400Ϊ��ͨ,800Ϊ�Ӵ�\n\n"
        "    /i\t\t\t   б��\n\n"
        "    /u\t\t\t   �»���\n\n"
        "    /s\t\t\t   ɾ����\n\n"
        "    /fc   R G B\t\t   ǰ��ɫ(Ĭ�ϰ�ɫ)\n\n"
        "    /bc   R G B\t\t   ����ɫ(Ĭ��͸��)\n\n"
        "    /r    x y width height �����������ڵ�����\n\n"
        "    /dp   dp\t\t   ��ʽ,��ѡֵ����������\n\n"
        "\t  Top\t\t   ������\n"
        "\t  Lef\t\t   �����\n"
        "\t  Center\t   ����\n"
        "\t  Right\t\t   �Ҷ���\n"
        "\t  Vecenter\t   ��ֱ����\n"
        "\t  Bottom\t   �׶���\n"
        "\t  WordBreak\t   �Զ�����\n"
        "\t  SingleLine\t   �����ı�\n"
        "\t  ExpandTabs\t   ��չTAB�ĳ���\n"
        "\t  TabsTop\t   TAB����ӵ��ַ���\n"
        "\t  NoPrefix\t   ������'&',Ĭ��'&'����ַ��ᱻ����»���\n"
        "\t  HidePrefix\t   �����»���\n"
        "\t  PrefixOnly\t   ��'&'����Ϊǰ׺\n"
        "\t  EditControl\t   ������һ�й���,����������\n"
        "\t  Path_Ellipsis\t   �ı�����ʱ,�м�ʹ��'...',���ǻᾡ���������һ��\\������ַ�\n"
        "\t  End_Ellipsis\t   �ı�����ʱ,β��ʹ��'...'\n"
        "\t  ModifyString\t   ʡ����ʾ,��Path_Ellipsis��End_Ellipsis���ʹ��\n"
        "\t  RtlReading\t   ���ҵ�������ı�\n"
        "\t  Word_Ellipsis\t   �ضϹ����ĵ���,��ʹ��'...'\n\n"
        );
    }
    
    int i;

    char DTParams[][25] = {"TOP","LEFT","CENTER","RIGHT","VCENTER","BOTTOM","WORDBREAK","SINGLELINE","EXPANDTABS","TABSTOP","","","","NOPREFIX","","EDITCONTROL","PATH_ELLIPSIS","END_ELLIPSIS","MODIFYSTRING","RTLREADING","WORD_ELLIPSIS","","HIDEPREFIX","PREFIXONLY"};
    
    FUNC_TEXT_INFO fti = {argv[1], "����", 0, 0, 0, 20, 0, RGB(0, 0, 0), RGB(255, 255, 255), true, false, false, false, false, {0, 0, 0, 0}, 0};

    for (i = 2; i < argc; ++i) {
        if (stricmp(argv[i], "/p") == 0) {
            if (!check_argv(argv, argc, i, 2)) {
                fprintf(stderr, "ERROR:'/p'������Ҫ������������!\n");
                return 1;
            }
            fti.x = atoi(argv[i + 1]);
            fti.y = atoi(argv[i + 2]);
            i += 2;
        } else if (stricmp(argv[i], "/z") == 0) {
            if (!check_argv(argv, argc, i, 2)) {
                fprintf(stderr, "ERROR:'/z'������Ҫ������������!\n");
                return 1;
            }
            fti.width  = atoi(argv[i + 1]);
            fti.height = atoi(argv[i + 2]);
            i += 2;
        } else if (stricmp(argv[i], "/b") == 0) {
            if (!check_argv(argv, argc, i, 1)) {
                fprintf(stderr, "ERROR:'/b'������Ҫ����һ������!\n");
                return 1;
            }
            fti.weight = atoi(argv[i + 1]);
            i += 1;
        } else if (stricmp(argv[i], "/f") == 0) {
            if (!check_argv(argv, argc, i, 1)) {
                fprintf(stderr, "ERROR:'/f'������Ҫ����һ������!\n");
                return 1;
            }
            fti.front = argv[i + 1];
            //strcpy(front, argv[i + 1]);
            i += 1;
        } else if (stricmp(argv[i], "/fc") == 0) {
            if (!check_argv(argv, argc, i, 3)) {
                fprintf(stderr, "ERROR:'/fc'������Ҫ������������!\n");
                return 1;
            }
            fti.clrFr = RGB(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]));
            i += 3;
        } else if (stricmp(argv[i], "/bc") == 0) {
            if (!check_argv(argv, argc, i, 3)) {
                fprintf(stderr, "ERROR:'/bc'������Ҫ������������!\n");
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
                fprintf(stderr, "ERROR:'/r'������Ҫ�����ĸ�����!\n");
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
            fprintf(stderr, "ERROR:δ֪����'%s'!\n", argv[i]);
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

