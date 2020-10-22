// NuoDirect.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NuoDirectWindow.h"

#include "NuoAppInstance.h"


NuoDirectWindow::NuoDirectWindow(const std::string& title)
	: NuoWindow(title)
{
}


void NuoDirectWindow::OnPaint()
{
	_onPaint();
}



void NuoDirectWindow::SetOnPaint(std::function<void()> onPaint)
{
	_onPaint = onPaint;
}