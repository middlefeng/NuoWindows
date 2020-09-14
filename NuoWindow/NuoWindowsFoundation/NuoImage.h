#pragma once

#include <windows.h>

#include <string>
#include <memory>

class NuoIcon;
typedef std::shared_ptr<NuoIcon> PNuoIcon;

class NuoReadStream;
typedef std::shared_ptr<NuoReadStream> PNuoReadStream;

class NuoImage : public std::enable_shared_from_this<NuoImage>
{
	HBITMAP _hBitmap;
	PNuoReadStream _iStream;

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
	void Save16(const std::string& path);
	void Save(const std::string& path);

	friend NuoImage;
};