
#include "NuoImage.h"
#include "NuoStrings.h"
#include "NuoFile.h"

#include <windows.h>
#include <wincodec.h>
#include <gdiplus.h>
#include <sys/stat.h>
#include <stdio.h>


// code on https://faithlife.codes/blog/2008/09/displaying_a_splash_screen_with_c_part_i/



static IWICBitmapSource* LoadBitmapFromStream(IStream* ipImageStream)
{
    IWICBitmapSource* ipBitmap = NULL;

    do
    {
        IWICBitmapDecoder* ipDecoder = NULL;
        if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER,
                                    __uuidof(ipDecoder), reinterpret_cast<void**>(&ipDecoder))))
            break;

        do
        {
            // load the PNG

            if (FAILED(ipDecoder->Initialize(ipImageStream, WICDecodeMetadataCacheOnLoad)))
                break;

            // check for the presence of the first frame in the bitmap

            UINT nFrameCount = 0;
            if (FAILED(ipDecoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
                break;

            // load the first frame (i.e., the image)

            IWICBitmapFrameDecode* ipFrame = NULL;
            if (FAILED(ipDecoder->GetFrame(0, &ipFrame)))
                break;

            // convert the image to 32bpp BGRA format with pre-multiplied alpha

            //   (it may not be stored in that format natively in the PNG resource,

            //   but we need this format to create the DIB to use on-screen)

            WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame, &ipBitmap);
            ipFrame->Release();
        } 
        while (0);

        ipDecoder->Release();
    } 
    while (0);

    return ipBitmap;
}


static HBITMAP CreateHBITMAP(IWICBitmapSource* ipBitmap)
{
    // initialize return value

    HBITMAP hbmp = NULL;

    do
    {
        // get image attributes and check for valid image

        UINT width = 0;
        UINT height = 0;
        if (FAILED(ipBitmap->GetSize(&width, &height)) || width == 0 || height == 0)
            break;

        // prepare structure giving bitmap information (negative height indicates a top-down DIB)

        BITMAPINFO bminfo;
        ZeroMemory(&bminfo, sizeof(bminfo));
        bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bminfo.bmiHeader.biWidth = width;
        bminfo.bmiHeader.biHeight = -((LONG)height);
        bminfo.bmiHeader.biPlanes = 1;
        bminfo.bmiHeader.biBitCount = 32;
        bminfo.bmiHeader.biCompression = BI_RGB;

        // create a DIB section that can hold the image

        void* pvImageBits = NULL;
        HDC hdcScreen = GetDC(NULL);
        hbmp = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pvImageBits, NULL, 0);
        ReleaseDC(NULL, hdcScreen);
        if (hbmp == NULL)
            break;

        // extract the image into the HBITMAP

        const UINT cbStride = width * 4;
        const UINT cbImage = cbStride * height;
        if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE*>(pvImageBits))))
        {
            // couldn't extract image; delete HBITMAP

            DeleteObject(hbmp);
            hbmp = NULL;
        }
    }
    while (0);

    return hbmp;
}


NuoImage::NuoImage()
    : _hBitmap(0),
      _iStream(0)
{
}


NuoImage::~NuoImage()
{
    if (_hBitmap)
        DeleteObject(_hBitmap);
}


void NuoImage::Load(const std::string& path)
{
    NuoFile imageFile(path);
    PNuoStream stream = imageFile.Stream();

    do
    {
        if (!stream)
            break;

        auto source = LoadBitmapFromStream(*stream);
        if (!source)
            break;

        _hBitmap = CreateHBITMAP(source);
    }
    while (0);

    _iStream = stream;
}


PNuoIcon NuoImage::Icon()
{
    if (!_iStream)
        return nullptr;

    Gdiplus::Bitmap gdiBitmap(*_iStream);
    HICON icon = 0;
    HRESULT hr = gdiBitmap.GetHICON(&icon);

    PNuoIcon result(new NuoIcon(icon));
    return result;
}


NuoIcon::NuoIcon(HICON icon)
    : _hIcon(icon)
{
}


HICON NuoIcon::Handle() const
{
    return _hIcon;
}



NuoIcon::~NuoIcon()
{
    if (_hIcon)
        DestroyIcon(_hIcon);
}