#include "GCD.h"

bool check_argv(char *argv[], int argc, int start, int arg_num)
{
    int i;
    if (argc <= start + arg_num) {
        return false;
    }
    for (i = start + 1; i <= start + arg_num; ++i) {
        if (argv[i][0] == '/') {  //遇到'/'说明是下一个开关
            return false;
        }
    }
    return true;
}

void char2wchar(char *str, wchar_t *wstr)
{
    int slen = strlen(str);
    int wlen = MultiByteToWideChar(CP_ACP, 0, str, slen, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, str, slen, wstr, wlen);
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

