
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"
#include "NuoImage.h"


class NuoDialog;
class NuoListView;
typedef std::shared_ptr<NuoListView> PNuoListView;


class NuoListView : public NuoControl
{

public:

	NuoListView(const PNuoWindow& parent);
	virtual ~NuoListView();

	void Init(int controlID);

	friend NuoDialog;

};



