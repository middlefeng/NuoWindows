#pragma once

#include <windows.h>

#include <string>
#include <vector>
#include <memory>
#include <wrl.h>

class NuoIcon;
typedef std::shared_ptr<NuoIcon> PNuoIcon;

class NuoReadStream;
typedef std::shared_ptr<NuoReadStream> PNuoReadStream;

struct IWICBitmapSource;


class NuoImage : public std::enable_shared_from_this<NuoImage>
{
	PNuoReadStream _iStream;
	Microsoft::WRL::ComPtr<IWICBitmapSource> _bitmap;
	HBITMAP _hBitmap;
	bool _backgroundGrid;

	UINT _width;
	UINT _height;

public:

	NuoImage();
	~NuoImage();

	void Load(const std::string& path, int backgroundGrid);
	void LoadPNG(const std::string& path, int backgroundGrid);
	PNuoIcon Icon();
	operator HBITMAP ();

	void CopyPixel(std::vector<UINT8>& data);

	UINT Width() const;
	UINT Height() const;

private:

	void UpdateImageInfo();

};

typedef std::shared_ptr<NuoImage> PNuoImage;


class NuoIcon : public std::enable_shared_from_this<NuoIcon>
{
	HICON _hIcon;

	NuoIcon(HICON icon);

public:

	~NuoIcon();

	HICON Handle() const;
	void Save16(const std::string& path);
	void Save(const std::string& path);

	friend NuoImage;
};