#pragma once

#include "NuoWindow.h"
#include "NuoTimer.h"

#include "NuoDirect/NuoDirectView.h"
#include <functional>


class DirectView;
typedef std::shared_ptr<DirectView> PDirectView;


class DirectView : public NuoDirectView
{

	std::function<void()> _onPaint;
	std::function<void()> _onSize;

	PNuoTimer _refreshTimer;

public:

	void Init();

	DirectView(const PNuoDevice& device, const PNuoWindow& parent);

	virtual void OnPaint() override;
	virtual void OnSize(unsigned int x, unsigned int y) override;

	void SetOnPaint(std::function<void()> onPaint);
	void SetOnSize(std::function<void()> onSize);

};




