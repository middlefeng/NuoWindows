
#pragma once

#include <windows.h>

#include <string>
#include <functional>
#include <memory>

#include "NuoRect.h"
#include "NuoControl.h"
#include "NuoImage.h"

#include "NuoWindowTypes.h"


class NuoDialog;
class NuoListView;
typedef std::shared_ptr<NuoListView> PNuoListView;



enum NuoListProperty
{
	kNuoList_Checkbox	= 1,
	kNuoList_Edit		= 1 << 1,
};



class NuoListView : public NuoControl
{

public:

	NuoListView(const PNuoWindow& parent);
	virtual ~NuoListView();

	void Init(int controlID, int properties);
	void AddColumn(unsigned int index, const std::string& title,
				   unsigned int width, NuoAlign align);
	void AddItem(unsigned int index, unsigned int col, const std::string& str);

	friend NuoDialog;

};

