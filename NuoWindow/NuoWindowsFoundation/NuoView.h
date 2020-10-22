
#pragma once

#include "NuoControl.h"


class NuoView;
typedef std::shared_ptr<NuoView> PNuoView;


class NuoView : public NuoControl
{

public:

	NuoView(const PNuoWindow& parent);

	void Init();
	
};



