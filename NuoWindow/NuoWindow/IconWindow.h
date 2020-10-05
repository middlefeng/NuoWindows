#pragma once

#include "NuoWindow.h"

#include "NuoButton.h"
#include "NuoLabel.h"


class IconWindow : public NuoWindow
{
	std::weak_ptr<NuoWindow> _appWindow;
	PNuoButton _loadButton;
	PNuoButton _saveButton;
	PNuoLabel _iconLabel;

public:
	IconWindow(const PNuoWindow& appWindow);

	void Init();
	NuoRect<long> PreferredRect() const;
};


typedef std::shared_ptr<IconWindow> PIconWindow;
