//
//  ModelOperationPanel.h
//  ModelViewer
//
//  Created by middleware on 4/29/22.
//  Copyright © 2022 Dong Feng. All rights reserved.
//


#include "NuoUI/NuoView.h"


class NuoScrollView;
typedef std::shared_ptr<NuoScrollView> PNuoScrollView;
typedef std::weak_ptr<NuoScrollView> WPNuoScrollView;

class NuoSlider;
typedef std::shared_ptr<NuoSlider> PNuoSlider;
typedef std::weak_ptr<NuoSlider> WPNuoSlider;

class NuoLabel;
typedef std::weak_ptr<NuoLabel> WPNuoLabel;


class ModelOperationPanel : public NuoView
{

	WPNuoScrollView _scrollView;

	WPNuoLabel _fieldOfViewLabel;
	WPNuoSlider _fieldOfView;
	

public:

	ModelOperationPanel(const PNuoWindow& parent);
	virtual ~ModelOperationPanel();

	void Init(); // actual constructor where shared_from_this() is functioning

	virtual void SetPosition(const NuoRect<float>& pos, bool activate) override;

private:


};

