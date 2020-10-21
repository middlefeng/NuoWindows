#pragma once

#include "NuoWindow.h"

#include "NuoButton.h"
#include "NuoListView.h"



class ListViewWindow : public NuoWindow
{
	std::weak_ptr<NuoWindow> _appWindow;
	PNuoButton _loadButton;
	PNuoListView _listView;

public:
	ListViewWindow(const PNuoWindow& appWindow);

	void Init();
	NuoRect<long> PreferredRect() const;
};


typedef std::shared_ptr<ListViewWindow> PListViewWindow;
