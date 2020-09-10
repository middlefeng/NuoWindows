#pragma once

#include <windows.h>

#include <string>

class NuoImage
{
	HBITMAP _hBitmap;

public:

	void Load(const std::string& path);

};
