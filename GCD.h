#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <process.h>
#include <stdbool.h>
#include <string.h>
#include <wchar.h>

#define MAX_VK_COUNT 128
#define MAX_TITLE_LENGTH 256
#define MOUSE_CLICK 0
#define CALL_WITHCHAR 2
#define CALL_WITHVK 4
#define MAX_IMG_COMMAND_LENGTH 1024

typedef struct {
    int length;
    int *item;
} VK_LIST;

typedef struct {
    int Delay;
    int Default;
} CHOICE_INFO;

typedef struct {
    int Delay;
    CONSOLE_CURSOR_INFO cci;
} MOUSE_INFO;

typedef struct {
    char *imgFile;
    int x;
    int y;
    int width;
    int height;
    COLORREF clrTran;
    bool tran;
    bool alpha;
    HBITMAP hBitmap;
    HDC hdcMem;
} FUNC_IMG_INFO;

typedef struct {
    char *text;
    char *front;
    int x;
    int y;
    int width;
    int height;
    int weight;
    COLORREF clrBk;
    COLORREF clrFr;
    bool bktran;
    bool italic;
    bool underline;
    bool strikeout;
    bool DrawText;
    RECT rc;
    int DTParams;
} FUNC_TEXT_INFO;

#if defined (__GNUC__) //_MSC_VER
typedef bool (WINAPI *TRANSPARENTBLT)(HDC, int, int, int, int, HDC, int, int, int, int, UINT);
typedef bool (WINAPI *ALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
typedef HWND (WINAPI *GETCONSOLEWINDOW)(void);
#endif

int GCD_choice(int, char **);
int GCD_choice_withChar(char *, int, char, bool);
int GCD_choice_withVK(char *, int, int);
int GCD_choice_checkVK(int);
int GCD_chioce_showVK(void);
bool chrequ(char, char);
VK_LIST GCD_choice_withVK_split(char *);

int GCD_mouse(int, char **);
int GCD_mouse_click(int, int, bool);
bool GCD_mouse_goto(int, int);
bool GCD_mouse_set(int, bool);

int GCD_image(int, char **, FUNC_IMG_INFO *);
void GCD_image_show(FUNC_IMG_INFO *);
bool GCD_image_clean(int, int, int, int);
int GCD_image_fromfile(char *);
PCHAR* CommandLineToArgvA(PCHAR, int*);
void FreehBitmapFromEnv(void);

int GCD_text(int, char **);
void GCD_text_show(FUNC_TEXT_INFO *);

void char2wchar(char *, wchar_t *);
void wchar2char(wchar_t *, char);

void GCD_delay_1(void *);
void GCD_delay_2(void *);
