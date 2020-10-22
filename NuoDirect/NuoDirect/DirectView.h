#pragma once

#include "NuoWindow.h"
#include "NuoView.h"
#include <functional>


class DirectView;
typedef std::shared_ptr<DirectView> PDirectView;


class DirectView : public NuoView
{

	std::function<void()> _onPaint;

public:

	void Init();

	DirectView(const PNuoWindow& parent);
	virtual void OnPaint();

	void SetOnPaint(std::function<void()> onPaint);

};




