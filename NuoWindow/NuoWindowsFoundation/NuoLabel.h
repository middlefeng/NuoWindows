
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"


class NuoDialog;
typedef std::shared_ptr<NuoDialog> PNuoDialog;


class NuoLabel : public NuoControl
{

public:

	NuoLabel(const PNuoWindow& parent);
	virtual ~NuoLabel();

	void Init();
	void SetText(const std::string& text);

	friend NuoDialog;

};


typedef std::shared_ptr<NuoLabel> PNuoLabel;

