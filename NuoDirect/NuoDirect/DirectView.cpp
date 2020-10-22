// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DirectView.h"

#include "NuoAppInstance.h"


DirectView::DirectView(const PNuoWindow& parent)
	: NuoView(parent)
{
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


