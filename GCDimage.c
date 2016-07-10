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
#include <gdiplus\gdiplus.h>
#include "miniGdiPlus.h"
#include <shellapi.h>

#define PathFileExists PathFileExistsA

HWND hCMD;

int GCD_image(int argc, char *argv[], FUNC_IMG_INFO *_fii)
{
#if defined (__GNUC__)
    HMODULE hKernel32 = LoadLibrary("kernel32.dll");
    hCMD = ((GETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow"))();
    FreeLibrary(hKernel32);
#else
    hCMD = GetConsoleWindow();
#endif
    //puts("->GCD_image");
    if (argc == 1||strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD image [imgfile] [/p [x y]] [/tran [R G B]] [/z width height]\n"
        "GCD image [/c [x y width height]]\n"
        "GCD image [/f file]\n\n"
        "描述:\n    该命令允许用户在命令行界面打印或清除图片,支持bmp,jpg,png,gif(第一帧)等格式\n\n"
        "参数列表:\n    imgfile\t\t\t需要打印的图片\n\n"
        "    /c     [x y width height]   清除以x,y为左端点,宽为width,高为height的矩形区\n\t\t\t\t域内的图片,缺省值为整个窗口\n\n"
        "    /p     [x y]\t\t将图片打印到指定位置,缺省值为0,0\n\n"
        "    /tran  [R G B]\t\t将某颜色设为透明色,缺省值为白色.省略该参数则\t\t\t\t\t不指定透明色\n\n"
        "    /alpha\t\t\t显示本身具有透明度的图片\n\n"
        "    /z     width height\t\t指定图片大小.省略该参数则使用图片原宽度和高度\n\n"
        "    /f     file\t\t\t从文件中读取命令执行\n\n"
        "注意:\n    1./tran与/alpha不兼容\n\n"
        "    2./f模式启用新的开关/v var, 该开关会将图片的句柄复制到变量var.\n"
        "      然后可以通过$var的方式来引用图片.(该方式只能与/z开关同时使用,\n"
        "      使用其他开关需要在加载图片时指定)\n\n"
        );
        return 0;
    } else if (stricmp(argv[1], "/c") == 0) {
        if (argc == 6) {
            return GCD_image_clean(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
        } else {
            return GCD_image_clean(0, 0, 0, 0);
        }
    } else if (stricmp(argv[1], "/f") == 0) {
        if (argc == 3) {
            return GCD_image_fromfile(argv[2]);
        } else {
            fprintf(stderr, "ERROR:'/f'开关需要接收一个参数!\n");
            return 1;
        }
        //puts("->GCD_image_fromfile");
        
    }
    int i;

    FUNC_IMG_INFO fii = {argv[1], 0, 0, 0, 0, RGB(255, 255, 255), false, false, 0, 0};
    if (_fii != NULL) {
        fii.width   = _fii->width;
        fii.height  = _fii->height;
        fii.hBitmap = _fii->hBitmap;
        fii.hdcMem  = _fii->hdcMem;
    }

    for (i = 2; i < argc; ++i) {
        if (stricmp(argv[i], "/p") == 0) {
            if (check_argv(argv, argc, i, 2)) {
                fii.x = atoi(argv[i + 1]);
                fii.y = atoi(argv[i + 2]);
                i += 2;
            } else {
                fprintf(stderr, "ERROR:'/p'开关需要接收两个参数!\n");
                return 1;
            }
        } else if (stricmp(argv[i], "/tran") == 0) {
            fii.tran = true;
            if (check_argv(argv, argc, i, 3)) { //有三个参数
                fii.clrTran = RGB(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]));
                i += 3;
            } else if (argc == i + 1 || argv[i + 1][0] == '/') { //一个参数都没有? 先判断参数个数,免得argv[i+1]越界
                fii.clrTran = RGB(255, 255, 255);
            } else {
                fprintf(stderr, "ERROR:'/tran'开关需要接收三个参数!\n");
                return 1;
            }

        } else if (stricmp(argv[i], "/alpha") == 0) {
            fii.alpha = true;
        } else if (stricmp(argv[i], "/z") == 0) {
            if (check_argv(argv, argc, i, 2)) {
                fii.width = atoi(argv[i + 1]);
                fii.height = atoi(argv[i + 2]);
                i += 2;
            } else {
                fprintf(stderr, "ERROR:'/z'开关需要接收两个参数!\n");
                return 1;
            }
        }
    }
    //printf("tran:%d eX:%d eY:%d\n", tran, width, height);
    GCD_image_show(&fii);
    return 0;
}

/*TODO:统一一下img fii _ffi*/
void GCD_image_show(FUNC_IMG_INFO *fii)
{
#if defined  (__GNUC__)
    HMODULE hMsimg32  = LoadLibrary("Msimg32.dll");
    TRANSPARENTBLT TransparentBlt   = (TRANSPARENTBLT)GetProcAddress(hMsimg32, "TransparentBlt");
    ALPHABLEND     AlphaBlend       = (ALPHABLEND)GetProcAddress(hMsimg32, "AlphaBlend");
#endif
    IMAGE img;
    HDC   hdcDst;
    HDC   hdcSrc;
    //printf("IMAGE:%p %d %d %p\n", fii->hBitmap, fii->width, fii->height, fii->hdcMem);
    if (fii->hBitmap != 0) {
        img.hBitmap = fii->hBitmap;
        img.w       = fii->width;
        img.h       = fii->height;
    } else {
        if (!PathFileExists(fii->imgFile)) {
            fprintf(stderr, "ERROR:无法打开%s!\n", fii->imgFile);
            return;
        }
        wchar_t wImgFile[512] = {0};
        char2wchar(fii->imgFile, wImgFile);
        img = LoadThisImge(wImgFile, fii->width, fii->height);
    }

    if (fii->hdcMem != 0) {
        hdcDst = fii->hdcMem;
    } else {
        hdcDst = GetDC(hCMD);
    }
    hdcSrc = CreateCompatibleDC(hdcDst);
    SelectObject(hdcSrc, img.hBitmap);

    if (fii->tran) {
        TransparentBlt(hdcDst, fii->x, fii->y, img.w, img.h, hdcSrc, 0, 0, img.w, img.h, fii->clrTran);
    } else if (fii->alpha) {
        BLENDFUNCTION ftn = {AC_SRC_OVER, 0, -1, AC_SRC_ALPHA};
        AlphaBlend(hdcDst, fii->x, fii->y, img.w, img.h, hdcSrc, 0, 0, img.w, img.h, ftn);
    } else {
        BitBlt(hdcDst, fii->x, fii->y, img.w, img.h, hdcSrc, 0, 0, SRCCOPY);
    }

    if (fii->hBitmap == 0) {
        DeleteObject(img.hBitmap);
    }

    if (fii->hdcMem == 0) {
        ReleaseDC(hCMD, hdcDst);
    }
    DeleteDC(hdcSrc);
}

bool GCD_image_clean(int x, int y, int width, int height)
{
    RECT rc   = {x, y, width, height};

    return InvalidateRect(hCMD, (x | y | width | height) == 0 ? NULL : &rc, true);
}

/*直接使用int来储存HBITMAP会溢出为负数??*/
int GCD_image_fromfile(char *imgFile)
{
    FILE *fp = fopen(imgFile, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR:无法打开%s!\n", imgFile);
        return 1;
    }

    char cmd[MAX_IMG_COMMAND_LENGTH] = {0};
    int  argc, i;
    char **argv;

    RECT rc;
    GetClientRect(hCMD, &rc);

    HDC  hdcCMD = GetDC(hCMD);
    HDC  hdcMem = CreateCompatibleDC(hdcCMD); //创建内存画布
    HBITMAP  hdcBMP = CreateCompatibleBitmap(hdcCMD, rc.right, rc.bottom);
    SelectObject(hdcMem, hdcBMP);
    while (!feof(fp)) {
        fgets(cmd, MAX_IMG_COMMAND_LENGTH, fp);
        argv = CommandLineToArgvA(cmd, &argc);
        wchar_t wImgFile[512] = {0};
        int     varName_index = 0, width = 0, height = 0;
        char    varValue[512] = {0};
        char    varName[512]  = "$GCD";
        HBITMAP hImgFile = 0;

        if (argv[0][0] == '$') {
            strcat(varName, &argv[0][1]);
            
            GetEnvironmentVariable(varName, varValue, 512);

            sscanf(varValue, "%p:%d:%d", &hImgFile, &width, &height);

        } else {
            for (i = 1; i < argc; ++i) {
                if (stricmp(argv[i], "/v") == 0) {
                    varName_index = i + 1;
                    //printf("varName_index=%d\n", varName_index);
                } else if (stricmp(argv[i], "/z") == 0) {
                    width  = atoi(argv[i + 1]);
                    height = atoi(argv[i + 2]);
                    i += 2;
                }
            }
        }

        if (varName_index != 0) { // 存在/v开关,只赋值
            if (!PathFileExists(argv[0])) {
                fprintf(stderr, "ERROR:无法打开%s!\n", argv[0]);
                return;
            }
            char2wchar(argv[0], wImgFile);
            IMAGE img = LoadThisImge(wImgFile, width, height);

            sprintf(varName,  "$GCD%s", argv[varName_index]);
            sprintf(varValue, "%p:%d:%d", img.hBitmap, img.w, img.h);

            SetEnvironmentVariable(varName, varValue);
        } else {
            //printf("CALL:%d %d\n", hImgFile, varName_index);
            FUNC_IMG_INFO fii = {NULL, 0, 0, width, height, 0, false, false, hImgFile == 0 ? 0 : hImgFile, hdcMem};
            GCD_image(argc + 1, argv - 1, &fii);
        }
    }
    fclose(fp);
    FreehBitmapFromEnv();
    BitBlt(hdcCMD, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
    DeleteObject(hdcBMP);
    DeleteDC(hdcMem);
    ReleaseDC(hCMD, hdcCMD);
    return 0;
}

void FreehBitmapFromEnv()
{
    char *env = (char *)GetEnvironmentStrings();
    char *pEnv  = env;  
    HBITMAP hBitmap;
    for (; ;) {
        if (pEnv[0] == '$' &&
            pEnv[1] == 'G' &&
            pEnv[2] == 'C' &&
            pEnv[3] == 'D') {
            sscanf(pEnv + 5, "%p", &hBitmap);
            DeleteObject(hBitmap);
        }
        pEnv += strlen(pEnv) + 1;
        if (*pEnv == '\0') {
            break;
        }
    }
    FreeEnvironmentStrings(env);
}

//http://alter.org.ua/docs/win/args/
PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc)
{
    PCHAR* argv;
    PCHAR  _argv;
    ULONG   len;
    ULONG   argc;
    CHAR   a;
    ULONG   i, j;

    BOOLEAN  in_QM;
    BOOLEAN  in_TEXT;
    BOOLEAN  in_SPACE;

    len = strlen(CmdLine);
    i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

    argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
    i + (len+2)*sizeof(CHAR));

    _argv = (PCHAR)(((PUCHAR)argv)+i);

    argc = 0;
    argv[argc] = _argv;
    in_QM = FALSE;
    in_TEXT = FALSE;
    in_SPACE = TRUE;
    i = 0;
    j = 0;

    while( a = CmdLine[i] ) {
        if(in_QM) {
            if(a == '\"') {
                in_QM = FALSE;
            } else {
                _argv[j] = a;
                j++;
            }
        } else {
            switch(a) {
            case '\"':
                in_QM = TRUE;
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                in_SPACE = FALSE;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                if(in_TEXT) {
                    _argv[j] = '\0';
                    j++;
                }
                in_TEXT = FALSE;
                in_SPACE = TRUE;
                break;
            default:
                in_TEXT = TRUE;
                if(in_SPACE) {
                    argv[argc] = _argv+j;
                    argc++;
                }
                _argv[j] = a;
                j++;
                in_SPACE = FALSE;
                break;
            }
        }
        i++;
    }
    _argv[j] = '\0';
    argv[argc] = NULL;

    (*_argc) = argc;
    return argv;
}
