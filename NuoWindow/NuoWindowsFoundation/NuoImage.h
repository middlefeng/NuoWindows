#pragma once

#include <windows.h>

#include <string>
#include <memory>

class NuoIcon;
typedef std::shared_ptr<NuoIcon> PNuoIcon;

class NuoImage : public std::enable_shared_from_this<NuoImage>
{
	HBITMAP _hBitmap;
	IStream* _iStream;

public:

	NuoImage();
	~NuoImage();

	void Load(const std::string& path);
	PNuoIcon Icon();

};


class NuoIcon : public std::enable_shared_from_this<NuoIcon>
{
	HICON _hIcon;

	NuoIcon(HICON icon);

public:

	~NuoIcon();

	HICON Handle() const;

	friend NuoImage;
};