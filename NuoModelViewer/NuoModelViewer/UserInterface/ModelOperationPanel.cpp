//
//  ModelOperationPanel.cpp
//  ModelViewer
//
//  Created by middleware on 4/29/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//

#include "ModelOperationPanel.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "NuoUI/NuoLabel.h"
#include "NuoUI/NuoSlider.h"
#include "NuoUI/NuoScrollView.h"

#include "ModelOptionUpdate.h"


static const float sFieldOfViewMax = 10000.0f;
static const float sFieldOfViewAngleMax = ((2 * (float)M_PI) / 8.0f);


ModelOperationPanel::ModelOperationPanel(const PNuoWindow& parent)
	: NuoView(parent)
{
}


void ModelOperationPanel::SetPosition(const NuoRect<float>& pos, bool activate)
{
	NuoView::SetPosition(pos, activate);

	NuoRect<float> scrollViewPos(0, 100, pos.W(), 180);

	PNuoScrollView scrollView = _scrollView.lock();
	scrollView->SetPosition(scrollViewPos, activate);
	scrollView->SetContentHeight(360);

	float rowCoord = 12.0;
	NuoRect<float> controlPos(12, rowCoord, pos.W() - 28, 25);

	PNuoLabel fieldOfViewLabel = _fieldOfViewLabel.lock();
	fieldOfViewLabel->SetPosition(controlPos, false);

	rowCoord += 20.0;
	NuoRect<float> backgroundColorPos(6, rowCoord, pos.W() - 28, 25);
	PNuoSlider fieldOfView = _fieldOfView.lock();
	fieldOfView->SetPosition(backgroundColorPos, false);
}


ModelOperationPanel::~ModelOperationPanel()
{
}


void ModelOperationPanel::Init()
{
	_parent.lock()->Add(shared_from_this());

	PNuoScrollView scrollView = std::make_shared<NuoScrollView>(shared_from_this(), "Scroll View");

	scrollView->Init(0);
	_scrollView = scrollView;

	PNuoLabel fieldOfViewLabel = std::make_shared<NuoLabel>(scrollView);
	fieldOfViewLabel->Init(false);
	fieldOfViewLabel->SetText("Field of View:");
	fieldOfViewLabel->SetFont(NuoFont::MenuFont(16.5));
	_fieldOfViewLabel = fieldOfViewLabel;

	PNuoSlider fieldOfView = std::make_shared<NuoSlider>(scrollView);
	fieldOfView->Init(0, 0, (int)sFieldOfViewMax);
	fieldOfView->SetValue(10000);
	fieldOfView->SetOnScroll([this](int)
		{
			auto updateDelegate = _optionUpdateDelegate.lock();
			updateDelegate->ModelOptionUpdated();
		});
	_fieldOfView = fieldOfView;
}


float ModelOperationPanel::FieldOfViewRadian()
{
	PNuoSlider fieldOfView = _fieldOfView.lock();

	float value = fieldOfView->Value();
	value = value / sFieldOfViewMax * sFieldOfViewAngleMax;
	return std::max<float>(3e-5, value);
}


void ModelOperationPanel::SetOptionUpdateDelegate(const PModelOptionUpdate& delegate)
{
	_optionUpdateDelegate = delegate;
}

