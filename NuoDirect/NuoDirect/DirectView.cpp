// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DirectView.h"

#include "NuoAppInstance.h"


DirectView::DirectView(const PNuoDevice& device,
					   const PNuoWindow& parent)
	: NuoDirectView(device, parent)
{
}


void DirectView::OnSize(unsigned int x, unsigned int y)
{
	NuoDirectView::OnSize(x, y);
	_onSize();
}


void DirectView::Init()
{
}


void DirectView::OnPaint()
{
	_onPaint();
}



void DirectView::SetOnPaint(std::function<void()> onPaint)
{
	_onPaint = onPaint;
}


void DirectView::SetOnSize(std::function<void()> onSize)
{
	_onSize = onSize;
}


