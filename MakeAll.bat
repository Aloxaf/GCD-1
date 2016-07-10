@echo off
setlocal enabledelayedexpansion
set "cmd=choice image mouse syba text"s
set "tmp=int main(int argc, char *argv[]){StdInput = GetStdHandle(STD_INPUT_HANDLE);StdOutPut = GetStdHandle(STD_OUTPUT_HANDLE);return GCD$cmd(argc, argv);}"

rem 编译所有.c文件
for %%i in (*.c) do (
    echo compiling %%~nxi
    set .o=!.o! %%~ni.o
    tcc -o %%~ni.o -c %%~nxi
)

rem 生成每个小功能.c文件并编译
for %%i in (%cmd%) do (
    echo compiling GCD%%imain.c
    set .c_del=!.c_del! GCD%%imain.c
    >GCD%%imain.c echo #include "GCD.h"
    >>GCD%%imain.c echo HANDLE StdInput, StdOutPut;
    if "%%i"=="image" (
        set "image=!tmp:$cmd(argc, argv)=_%%i(argc, argv, NULL)!"
        >>GCD%%imain.c echo !image!
    ) else >>GCD%%imain.c echo !tmp:$cmd=_%%i!
    tcc -o GCD%%imain.o -c GCD%%imain.c
)

rem 链接GCD
echo linking GCD.exe
tcc -o GCD.exe %.o% -lgdi32 -lmsimg32 -luser32 -lkernel32 -lgdiplus  -lshlwapi

rem 链接每个小功能
set .o=!.o:GCDmain.o=!
for %%i in (%cmd%) do set .o=!.o:GCD%%i.o=!
for %%i in (%cmd%) do (
    echo linking %%i.exe
    tcc -o GCD%%i.exe GCD%%i.o GCD%%imain.o !.o! -lgdi32 -lmsimg32 -luser32 -lkernel32 -lgdiplus  -lshlwapi
)
del *.o !.c_del!
echo OK!
pause
