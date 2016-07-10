#include "GCD.h"


/*TOFIX:"����"���ȵ���*/
/*TOFIX:ͳһErrorLevel*/
HANDLE StdInput;
HANDLE StdOutPut;

int main(int argc, char *argv[])
{
    StdInput  = GetStdHandle(STD_INPUT_HANDLE);
    StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (1 == argc||strcmp(argv[1], "/?") == 0) {
        printf(
        "GCD [choice | mouse | image | syba | text | about]\n\n"
        "����:\n    GCD(Game Create Door)�����˶���������Ĺ���,��Ҫ����bat��Ϸ����\n"
        "    ����GCD ���� /? ����������ϸ����\n\n"
        "ע��:\n    GCDû��̫���쳣����,��Ϊ����@����� ˵\"���ǲ���ҪΪ�û���д����ģ����޴���\"\n\n"
        );
        return 0;
    }
    if (stricmp(argv[1], "choice") == 0) {
        return GCD_choice(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "mouse") == 0) {
        return GCD_mouse(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "image") == 0) {
        return GCD_image(argc - 1, argv + 1, NULL);
    } else if (stricmp(argv[1], "syba") == 0) {
        return GCD_syba(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "text") == 0) {
        return GCD_text(argc - 1, argv + 1);
    } else if (stricmp(argv[1], "about") == 0) {
        printf(
        "GCD 1.1\n\n"
        "Copyright (C) 2016  aiwozhonghuaba\n\n"
        "\n\n"
        "This program is free software: you can redistribute it and/or modify\n\n"
        "it under the terms of the GNU General Public License as published by\n\n"
        "the Free Software Foundation, either version 3 of the License, or\n\n"
        "(at your option) any later version.\n\n"
        "\n\n"
        "This program is distributed in the hope that it will be useful,\n\n"
        "but WITHOUT ANY WARRANTY; without even the implied warranty of\n\n"
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\n"
        "GNU General Public License for more details.\n\n"
        "\n\n"
        "You should have received a copy of the GNU General Public License\n\n"
        "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
    }
    return 0;
}
