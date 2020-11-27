
#include "NuoImage.h"
#include "NuoStrings.h"
#include "NuoFile.h"

#include <windows.h>
#include <wincodec.h>
#include <OleCtl.h>
#include <gdiplus.h>
#include <sys/stat.h>
#include <stdio.h>
#include <cassert>
#include <vector>
#include <wrl.h>


// code on https://faithlife.codes/blog/2008/09/displaying_a_splash_screen_with_c_part_i/


static Microsoft::WRL::ComPtr<IWICBitmapSource> LoadBitmapFromDecoder(const Microsoft::WRL::ComPtr<IWICBitmapDecoder>& decoder);



static Microsoft::WRL::ComPtr<IWICBitmapSource> LoadBitmapFromStream(const Microsoft::WRL::ComPtr<IStream>& ipImageStream)
{
    Microsoft::WRL::ComPtr<IWICBitmapSource> ipBitmap;

    do
    {
        Microsoft::WRL::ComPtr<IWICBitmapDecoder> ipDecoder;
        if (FAILED(CoCreateInstance(CLSID_WICPngDecoder, NULL, CLSCTX_INPROC_SERVER,
                   IID_PPV_ARGS(&ipDecoder))))
            break;

        // load the PNG

        if (FAILED(ipDecoder->Initialize(ipImageStream.Get(), WICDecodeMetadataCacheOnLoad)))
            break;

        ipBitmap = LoadBitmapFromDecoder(ipDecoder);
    } 
    while (0);

    return ipBitmap;
}


static Microsoft::WRL::ComPtr<IWICBitmapSource> LoadBitmapFromDecoder(const Microsoft::WRL::ComPtr<IWICBitmapDecoder>& decoder)
{
    Microsoft::WRL::ComPtr<IWICBitmapSource> ipBitmap;

    do
    {
        // check for the presence of the first frame in the bitmap

        UINT nFrameCount = 0;
        if (FAILED(decoder->GetFrameCount(&nFrameCount)) || nFrameCount != 1)
            break;

        // load the first frame (i.e., the image)

        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> ipFrame;
        if (FAILED(decoder->GetFrame(0, &ipFrame)))
            break;

        // convert the image to 32bpp BGRA format with pre-multiplied alpha

        //   (it may not be stored in that format natively in the PNG resource,

        //   but we need this format to create the DIB to use on-screen)

        WICConvertBitmapSource(GUID_WICPixelFormat32bppPBGRA, ipFrame.Get(), &ipBitmap);
    }
    while (0);

    return ipBitmap;
}


static void BlendCheckerboard(void* buffer, size_t width, size_t height, size_t grid)
{
    const size_t cbStride = width * 4;

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            BYTE color = 255;
            if ((col / grid) % 2 != (row / grid) % 2)
                color = (BYTE)(255.0 * 0.8f);

            BYTE* ptr = static_cast<BYTE*>(buffer);
            ptr = ptr + row * cbStride + col * 4;

            float alpha = ptr[3] / 255.0f;

            int b = ptr[0];
            int g = ptr[1];
            int r = ptr[2];

            ptr[0] = (BYTE)(b * alpha + color * (1 - alpha));
            ptr[1] = (BYTE)(g * alpha + color * (1 - alpha));
            ptr[2] = (BYTE)(r * alpha + color * (1 - alpha));
            ptr[3] = 255;
        }
    }
}


static HBITMAP CreateHBITMAP(const Microsoft::WRL::ComPtr<IWICBitmapSource>& ipBitmap, int backgroundGrid)
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

        std::vector<BYTE> buffer;
        buffer.resize(cbImage);

        if (FAILED(ipBitmap->CopyPixels(NULL, cbStride, cbImage, static_cast<BYTE*>(&buffer[0]))))
        {
            // couldn't extract image; delete HBITMAP

            DeleteObject(hbmp);
            hbmp = NULL;
            break;
        }

        if (backgroundGrid > 0)
            BlendCheckerboard(&buffer[0], width, height, backgroundGrid);

        memcpy(pvImageBits, (void*)(&buffer[0]), cbImage);
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


void NuoImage::Load(const std::string& path, int backgroundGrid)
{
    NuoFile imageFile(path);
    PNuoReadStream stream = imageFile.ReadStream();

    do
    {
        if (!stream)
            break;

        auto source = LoadBitmapFromStream(stream->Stream());
        if (!source)
            break;

        _hBitmap = CreateHBITMAP(source, backgroundGrid);
    }
    while (0);

    _iStream = stream;
}


PNuoIcon NuoImage::Icon()
{
    if (!_iStream)
        return nullptr;

    Gdiplus::Bitmap gdiBitmap(_iStream->Stream().Get());
    HICON icon = 0;
    HRESULT hr = gdiBitmap.GetHICON(&icon);

    PNuoIcon result(new NuoIcon(icon));
    return result;
}


NuoImage::operator HBITMAP () const
{
    return _hBitmap;
}


// code of saving ico file
// https://stackoverflow.com/questions/2289894/how-can-i-save-hicon-to-an-ico-file

struct ICONHEADER
{
    WORD idReserved; // must be 0
    WORD idType; // 1 = ICON, 2 = CURSOR
    WORD idCount; // number of images (and ICONDIRs)

    // ICONDIR [1...n]
    // ICONIMAGE [1...n]
};

struct ICONDIR
{
    BYTE bWidth;
    BYTE bHeight;
    BYTE bColorCount;
    BYTE bReserved;
    WORD wPlanes; // for cursors, this field = wXHotSpot
    WORD wBitCount; // for cursors, this field = wYHotSpot
    DWORD dwBytesInRes;
    DWORD dwImageOffset; // file-offset to the start of ICONIMAGE

};


static void WriteIconHeader(NuoFile& file)
{
    ICONHEADER iconheader;

    // Setup the icon header
    iconheader.idReserved = 0; // Must be 0
    iconheader.idType = 1; // Type 1 = ICON (type 2 = CURSOR)
    iconheader.idCount = 1; // number of ICONDIRs

    // Write the header to disk
    file.Write(&iconheader, sizeof(iconheader));
}

static bool GetIconBitmapInfo(HICON hIcon, ICONINFO* pIconInfo, BITMAP* pbmpColor, BITMAP* pbmpMask)
{
    if (!GetIconInfo(hIcon, pIconInfo))
        return false;

    if (!GetObject(pIconInfo->hbmColor, sizeof(BITMAP), pbmpColor))
        return false;

    if (!GetObject(pIconInfo->hbmMask, sizeof(BITMAP), pbmpMask))
        return false;

    return true;
}


static UINT NumBitmapBytes(BITMAP* pBitmap)
{
    int nWidthBytes = pBitmap->bmWidthBytes;

    // bitmap scanlines MUST be a multiple of 4 bytes when stored
    // inside a bitmap resource, so round up if necessary
    if (nWidthBytes & 3)
        nWidthBytes = (nWidthBytes + 4) & ~3;

    return nWidthBytes * pBitmap->bmHeight;
}


static void WriteIconImageHeader(NuoFile& file, BITMAP* pbmpColor, BITMAP* pbmpMask)
{
    BITMAPINFOHEADER biHeader;
    UINT nImageBytes;

    // calculate how much space the COLOR and MASK bitmaps take
    nImageBytes = NumBitmapBytes(pbmpColor) + NumBitmapBytes(pbmpMask);

    // write the ICONIMAGE to disk (first the BITMAPINFOHEADER)
    ZeroMemory(&biHeader, sizeof(biHeader));

    // Fill in only those fields that are necessary
    biHeader.biSize = sizeof(biHeader);
    biHeader.biWidth = pbmpColor->bmWidth;
    biHeader.biHeight = pbmpColor->bmHeight * 2; // height of color+mono
    biHeader.biPlanes = pbmpColor->bmPlanes;
    biHeader.biBitCount = pbmpColor->bmBitsPixel;
    biHeader.biSizeImage = nImageBytes;

    // write the BITMAPINFOHEADER
    file.Write(&biHeader, sizeof(biHeader));

    // write the RGBQUAD color table (for 16 and 256 colour icons)
    if (pbmpColor->bmBitsPixel == 2 || pbmpColor->bmBitsPixel == 8)
    {
        assert(false);
    }
}


static UINT WriteIconData(NuoFile& file, HBITMAP hBitmap)
{
    BITMAP bmp;
    int i;
    BYTE* pIconData;
    UINT nBitmapBytes;

    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    nBitmapBytes = NumBitmapBytes(&bmp);
    pIconData = (BYTE*)malloc(nBitmapBytes);

    LONG copied = GetBitmapBits(hBitmap, nBitmapBytes, pIconData);
    assert(copied != 0);

    // bitmaps are stored inverted (vertically) when on disk..
    // so write out each line in turn, starting at the bottom + working
    // towards the top of the bitmap. Also, the bitmaps are stored in packed
    // in memory - scanlines are NOT 32bit aligned, just 1-after-the-other
    for (i = bmp.bmHeight - 1; i >= 0; i--)
    {
        // Write the bitmap scanline
        file.Write(
            pIconData + (i * bmp.bmWidthBytes), // calculate offset to the line
            bmp.bmWidthBytes /* 1 line of BYTES */ );

        // extend to a 32bit boundary (in the file) if necessary
        size_t remain = bmp.bmWidthBytes & 3;
        if (remain)
        {
            size_t paddingSize = 4 - remain;
            DWORD padding = 0;
            file.Write(&padding, paddingSize);
        }
    }

    free(pIconData);

    return nBitmapBytes;
}


static void WriteIconDirectoryEntry(NuoFile& file, int nIdx, HICON hIcon, UINT nImageOffset)
{
    ICONINFO iconInfo;
    ICONDIR iconDir;

    BITMAP bmpColor;
    BITMAP bmpMask;

    UINT nColorCount;
    UINT nImageBytes;

    GetIconBitmapInfo(hIcon, &iconInfo, &bmpColor, &bmpMask);

    nImageBytes = NumBitmapBytes(&bmpColor) + NumBitmapBytes(&bmpMask);

    if (bmpColor.bmBitsPixel >= 8)
        nColorCount = 0;
    else
        nColorCount = 1 << (bmpColor.bmBitsPixel * bmpColor.bmPlanes);

    // Create the ICONDIR structure
    iconDir.bWidth = (BYTE)bmpColor.bmWidth;
    iconDir.bHeight = (BYTE)bmpColor.bmHeight;
    iconDir.bColorCount = nColorCount;
    iconDir.bReserved = 0;
    iconDir.wPlanes = bmpColor.bmPlanes;
    iconDir.wBitCount = bmpColor.bmBitsPixel;
    iconDir.dwBytesInRes = sizeof(BITMAPINFOHEADER) + nImageBytes;
    iconDir.dwImageOffset = nImageOffset;

    // Write to disk
    file.Write(&iconDir, sizeof(iconDir));

    // Free resources
    DeleteObject(iconInfo.hbmColor);
    DeleteObject(iconInfo.hbmMask);
}


NuoIcon::NuoIcon(HICON icon)
    : _hIcon(icon)
{
}


HICON NuoIcon::Handle() const
{
    return _hIcon;
}


void NuoIcon::Save16(const std::string& path)
{
    // Create the IPicture intrface
    PICTDESC desc = { sizeof(PICTDESC) };
    desc.picType = PICTYPE_ICON;
    desc.icon.hicon = _hIcon;

    IPicture* pPicture = 0;
    HRESULT hr = OleCreatePictureIndirect(&desc, IID_IPicture, FALSE, (void**)&pPicture);
    if (FAILED(hr))
        return;

    NuoFile file(path);
    PNuoWriteStream wStream = std::make_shared<NuoWriteStream>();

    LONG cbSize = 0;
    pPicture->SaveAsFile(wStream->Stream().Get(), true, &cbSize);

    file.SaveStream(wStream, cbSize);
}


void NuoIcon::Save(const std::string& path)
{
    NuoFile file(path);

    int imageOffset = 0;

    WriteIconHeader(file);

    imageOffset = sizeof(ICONHEADER) + sizeof(ICONDIR);
    file.Seek(imageOffset);

    ICONINFO iconInfo;
    BITMAP bmpColor, bmpMask;

    GetIconBitmapInfo(_hIcon, &iconInfo, &bmpColor, &bmpMask);
    WriteIconImageHeader(file, &bmpColor, &bmpMask);

    WriteIconData(file, iconInfo.hbmColor);
    WriteIconData(file, iconInfo.hbmMask);

    DeleteObject(iconInfo.hbmColor);
    DeleteObject(iconInfo.hbmMask);

    file.Seek(sizeof(ICONHEADER));
    WriteIconDirectoryEntry(file, 0, _hIcon, imageOffset);
}



NuoIcon::~NuoIcon()
{
    if (_hIcon)
        DestroyIcon(_hIcon);
}