//from http://forum.pellesc.de/index.php?topic=4214.0
#ifndef _MINIGDIPLUS_H
#define _MINIGDIPLUS_H

typedef struct tagIMAGE
{
    HBITMAP hBitmap;
    int w;
    int h;
} IMAGE;

IMAGE LoadThisImge(wchar_t *file, int _nWidth, int _nHeight)
{
    GpImage     *thisImage;
    GpGraphics  *graphics = NULL;
    ULONG_PTR   gdiplusToken;
    IMAGE       _img;

    GdiplusStartupInput GSI = {1, NULL, false, false};
    GdiplusStartup(&gdiplusToken, &GSI, NULL);
    GdipLoadImageFromFile(file, &thisImage);

    HDC hdc = GetDC(NULL);

    GdipGetImageWidth (thisImage, &_img.w);
    GdipGetImageHeight(thisImage, &_img.h);

    HDC hdcMem = CreateCompatibleDC(hdc);

    if (_nHeight != 0) {
        _img.w = _nWidth;
        _img.h = _nHeight;
    }

    _img.hBitmap = CreateCompatibleBitmap(hdc, _img.w, _img.h);

    SelectObject(hdcMem, _img.hBitmap);

    GdipCreateFromHDC (hdcMem, &graphics);
    GdipDrawImageRectI(graphics, thisImage, 0, 0, _img.w, _img.h);

    GdipDisposeImage  (thisImage);
    GdipDeleteGraphics(graphics);

    DeleteDC (hdcMem);
    ReleaseDC(NULL, hdc);

    GdiplusShutdown(gdiplusToken);

    return _img;
}
#endif //:133
