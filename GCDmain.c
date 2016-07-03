#include "GCD.h"


/*TOFIX:"帮助"长度调整*/
/*TOFIX:统一ErrorLevel*/
int main(int argc, char *argv[])
{
    if (1 == argc||strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD [choice | mouse | image | text | about]\n\n"
        "描述:\n    GCD(Game Create Door)包含了多个第三方的功能,主要用于bat游戏开发\n"
        "    键入GCD 命令 /? 获得命令的详细帮助\n\n"
        "注意:\n    GCD没有太多异常处理,因为皇叔@黄田杰 说\"我们不需要为用户（写程序的）的愚蠢买单\"\n\n"
        );
        return 0;
    }
    if (stricmp(argv[1], "choice") == 0) {
        return GCD_choice(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "mouse") == 0) {
        return GCD_mouse(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "image") == 0) {
        return GCD_image(argc - 1, argv + 1, NULL);
    } else if (stricmp(argv[1], "text") == 0) {
        return GCD_text(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "about") == 0) {
        printf(
        "GCD 1.0\n\
Copyright (C) 2016  aiwozhonghuaba\n\
\n\
This program is free software: you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation, either version 3 of the License, or\n\
(at your option) any later version.\n\
\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\
\n\
You should have received a copy of the GNU General Public License\n\
along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
    }
    return 0;
}
