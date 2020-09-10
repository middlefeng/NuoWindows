
#include "NuoImage.h"
#include "NuoStrings.h"

#include <windows.h>
#include <wincodec.h>
#include <sys/stat.h>
#include <stdio.h>


// code on https://faithlife.codes/blog/2008/09/displaying_a_splash_screen_with_c_part_i/


static IStream* CreateStreamOnFile(const std::string path)
{
	std::wstring wpath = StringToUTF16(path);

	FILE* file = 0;
	_wfopen_s(&file, wpath.c_str(), L"rb");

	struct _stat fileState;
	_wstat(wpath.c_str(), &fileState);

	IStream* iStream = 0;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileState.st_size);
	HRESULT result = CreateStreamOnHGlobal(hGlobal, true, &iStream);
	if (!result)
	{
		GlobalFree(hGlobal);
		return 0;
	}

	return iStream;
}


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


void NuoImage::Load(const std::string& path)
{
    IStream* iStream = CreateStreamOnFile(path);

    do
    {
        if (!iStream)
            break;

        auto source = LoadBitmapFromStream(iStream);
        if (!source)
            break;

        _hBitmap = CreateHBITMAP(source);
    }
    while (0);
}

