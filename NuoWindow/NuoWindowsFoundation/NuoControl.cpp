
#include "NuoControl.h"


NuoControl::NuoControl(const std::string& title, const PNuoWindow& parent)
	: NuoWindow(), _parent(parent)
{
	_title = title;
}


NuoControl::~NuoControl()
{
}


void NuoControl::SetFocus()
{
	::SetFocus(_hWnd);
}


NuoRect<long> NuoControl::PositionDevice()
{
	auto result = NuoWindow::PositionDevice();

	auto parent = _parent.lock();
	if (parent)
	{
		RECT rect;
		rect.left = result.X();
		rect.top = result.Y();
		rect.right = rect.left + result.W();
		rect.bottom = rect.top + result.H();

		POINT lt{ rect.left, rect.top };
		POINT rb{ rect.right, rect.bottom };
		ScreenToClient(parent->Handle(), &lt);
		ScreenToClient(parent->Handle(), &rb);

		result = NuoRect<long>(lt.x, lt.y, rb.x - lt.x, rb.y - lt.y);
	}

	return result;
}