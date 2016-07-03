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

HANDLE StdInput;
HANDLE StdOutPut;
int GCD_choice(int argc, char *argv[])
{
    StdInput  = GetStdHandle(STD_INPUT_HANDLE);
    StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (1 == argc||strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD choice [[/ck choices] | [/cc choices]] [/t timeout /d choice]\n"
        "GCD choice [/vk virkey] | [/sk]\n\n"
        "描述:\n    该命令允许用户从选择列表中选择一个选项并返回该选项的索引,或检测一个虚拟按键的状况\n\n"
        "参数列表:\n    /cc   choices\t指定要创建的字符选项列表\n\n"
        "    /i\t\t\t说明/cc不区分大小写\n\n"
        "    /ck   choices\t指定要创建的虚拟键码选项列表\n\n"
        "    /t    timeout\t超时毫秒数\n\n"
        "    /d    choice\t超时默认选项,可能是字符或者虚拟键码\n\n"
        "    /vk   virkey\t检测指定的虚拟按键是否被按下,是则返回0,反之返回1\n\n"
        "    /sk\t\t返回接下来按下的一个按键的虚拟键码\n\n"
        "注意:\n    出现错误时ERRORLEVEL会被设定为 -1\n\n"
        );
    }
    if (stricmp(argv[1], "/vk") == 0) {
        return GCD_choice_checkVK(atoi(argv[2]));
    } else if (stricmp(argv[1], "/sk") == 0) {
        return GCD_choice_showVK();
    }

    int i;
    int list_index = 0, default_index = 0, callFunc  = 0, timeout = -1;
    bool ignore_case = false;
    
    /*TODO:srticmp太浪费,改成判断字符或许快一点 */
    for (i = 1; i < argc; ++i) {
        if (stricmp(argv[i], "/cc") == 0) {
            list_index = i + 1;
            callFunc   = CALL_WITHCHAR;
            ++i;
        } else if (stricmp(argv[i], "/ck") == 0) {
            list_index = i + 1;
            callFunc   = CALL_WITHVK;
            ++i;
        } else if (stricmp(argv[i], "/d") == 0) {
            default_index = i + 1;
            ++i;
        } else if (stricmp(argv[i], "/t") == 0) {
            timeout = atoi(argv[i]);
            ++i;
        } else if (stricmp(argv[i], "/i") == 0) {
            ignore_case = true;
        }
    }
    
    if (CALL_WITHCHAR == callFunc) { //根据callFunc的值决定调用哪个函数
        return GCD_choice_withChar(argv[list_index], timeout, argv[default_index][0], ignore_case);
    } else if (CALL_WITHVK == callFunc) {
        return GCD_choice_withVK(argv[list_index], timeout, atoi(argv[default_index]));
    }
    
    return -1;
}

//基础的choice
int GCD_choice_withChar(char *list, int Delay, char Default, bool ignore_case)
{
    int i, listLength = strlen(list);
    HANDLE hDelay;
    CHOICE_INFO choice_info = {Delay, 0};

    //将延时时间和默认选项打包传给延时线程
    if (Delay > 0) {
        for (i = 0; i < listLength; ++i) {
            if (list[i] == Default) {
                choice_info.Default = i + 1;
                break;
            }
        }

        hDelay = (HANDLE)_beginthreadex(NULL, 0, (unsigned (__stdcall *) (void *))GCD_delay_1, &choice_info, 0, NULL);
    }

    char cInput;
    for ( ; ;) {
        cInput = getch();
        for (i = 0; i < listLength; ++i) {
            /*TODO:不区分大小写  未测试 */
            if (list[i] == cInput||(ignore_case && chrequ(list[i], cInput))) {
                if (Delay > 0) {
                    /*FIXME: 用_beginthreadex开启的线程,但是用_endthreadex不会立即退出.CloseHandle代替不知道有没有问题 */
                    CloseHandle(hDelay);
                }
                return i + 1;
            }
        }
    }

    return -1;
}

//分割虚拟键码
VK_LIST GCD_choice_withVK_split(char *list)
{
    VK_LIST ret = {0, NULL};
    char tmpList[strlen(list) + 1];
    strcpy(tmpList, list);

    int i = 0;
    int listLength = strlen(list);
    int VK[MAX_VK_COUNT];
    
    char *split = strtok(tmpList, " ");
    while (NULL != split) {
        VK[i++] = atoi(split);
        split = strtok(NULL, " ");
    }

    ret.length = i;
    ret.item   = (int *)malloc(i * sizeof(int));
    
    for (--i ;i >= 0; --i) {
        ret.item[i] = VK[i];
    }
    return ret;
}

// 
int GCD_choice_withVK(char *list, int Delay, int Default)
{
    HANDLE hDelay;
    
    VK_LIST VK_list = GCD_choice_withVK_split(list);
    CHOICE_INFO choice_info = {Delay, 0};
    
    int i;
    int VK_list_length = VK_list.length;
    
    if (Delay > 0) {
        for (i = 0; i <= VK_list_length; ++i) {
            if (Default == VK_list.item[i]) {
                choice_info.Default = i + 1;
                break;
            }
        }
        
        hDelay = (HANDLE)_beginthreadex(NULL, 0, (unsigned (__stdcall *) (void *))GCD_delay_1, &choice_info, 0, NULL);
    }

    INPUT_RECORD ir;
    DWORD Res;
    for (; ; ) {
        ReadConsoleInput(StdInput, &ir, 1, &Res); //&Res不能是NULL
        if (KEY_EVENT == ir.EventType) {
            for (i = 0; i <= VK_list_length; ++i) {
                if (ir.Event.KeyEvent.wVirtualKeyCode == VK_list.item[i]) {
                    if (Delay > 0) {
                        /*FIXME: 用_beginthreadex开启的线程,但是用_endthreadex不会立即退出.CloseHandle代替不知道有没有问题 */
                        CloseHandle(hDelay);
                    }
                    free(VK_list.item);
                    return i + 1;
                }
            }
            
        }
    }
}

bool chrequ(char c1, char c2)
{
    if (c1 == c2) {
        return true;
    } else if (c1 >= 65 && c1 <= 90) {
        return (c1 + 32) == c2;
    } else if (c2 >= 65 && c2 <= 90){
        return c1 == (c2 + 32);
    } else {
        return false;
    }
}

int GCD_choice_checkVK(int VK)
{
    long VKState = GetAsyncKeyState(VK);
    if (VKState < 0 ) {
        return 0;
    }
    return 1;
}

int GCD_choice_showVK(void)
{
    INPUT_RECORD ir;
    DWORD Res;
    for (; ; ) {
        ReadConsoleInput(StdInput, &ir, 1, &Res);
        if (KEY_EVENT == ir.EventType) {
            return ir.Event.KeyEvent.wVirtualKeyCode;
        }
    }
}

void GCD_delay_1(void *choice_info)
{
#define ci ((CHOICE_INFO *)(choice_info))

    Sleep(ci->Delay);

    exit(ci->Default);
}
