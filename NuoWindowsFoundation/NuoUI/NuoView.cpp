

#include "NuoView.h"
#include "NuoAppInstance.h"
#include <windows.h>

NuoView::NuoView(const PNuoWindow& parent)
	: NuoControl("", parent)
{
	_hWnd = CreateWindow(kClassName, L"", WS_VISIBLE | WS_CHILD,
						 CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, parent->Handle(), nullptr,
						 NuoAppInstance::GetInstance()->Instance(), nullptr);

	SetWindowLongPtr(_hWnd, kWindowPtr, (LONG_PTR)this);
}



void NuoView::Init()
{
}
