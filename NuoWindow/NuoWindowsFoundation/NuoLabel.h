
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"
#include "NuoImage.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoLabel : public NuoControl
{

	PNuoImage _image;

public:

	NuoLabel(const PNuoWindow& parent);
	virtual ~NuoLabel();

	void Init(bool image);
	void SetText(const std::string& text);
	void SetImage(const PNuoImage& image);
	PNuoImage Image() const;

	friend NuoDialog;

};


typedef std::shared_ptr<NuoLabel> PNuoLabel;

