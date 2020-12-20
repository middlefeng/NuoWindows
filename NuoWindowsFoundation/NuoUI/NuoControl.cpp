
#include "NuoControl.h"


NuoControl::NuoControl(const std::string& title, const PNuoWindow& parent)
	: NuoWindow(), _id(-1),
	  _parent(parent), _autoPosition(kNuoControl_NoneAuto)
{
	_title = title;
}


NuoControl::~NuoControl()
{
	if (!_hWnd)
	{
		::DestroyWindow(_hWnd);
		_hWnd = 0;
	}
}


int NuoControl::ID() const
{
	return _id;
}


void NuoControl::SetFocus()
{
	::SetFocus(_hWnd);
}


void NuoControl::SetAutoPosition(NuoControlAutoPosition autoPos)
{
	_autoPosition = autoPos;
}


NuoControlAutoPosition NuoControl::AutoPosition() const
{
	return _autoPosition;
}



void NuoControl::SetMargin(const NuoInset<float>& inset)
{
	_parentMargin = inset;
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


NuoRect<float> NuoControl::Position()
{
	auto result = PositionDevice();

	auto parent = _parent.lock();
	if (parent)
	{
		float dpi = DPI();
		return (result / dpi);
	}

	return result / 1.0;
}


NuoRect<float> NuoControl::AutoPosition()
{
	static const long margin = 20;

	NuoRect<float> originalRect = Position();
	NuoRect<float> result(0, 0, originalRect.W(), originalRect.H());

	auto parent = _parent.lock();
	if (!parent)
		return originalRect;

	auto parentBound = parent->ClientRect();

	if (_autoPosition & kNuoControl_Stretch)
	{
		if (_autoPosition & kNuoControl_Stretch_L)
			result.SetX(_parentMargin._left);
		
		if (_autoPosition & kNuoControl_Stretch_T)
			result.SetY(_parentMargin._top);

		if (_autoPosition & kNuoControl_Stretch_R)
			result.SetW(parentBound.W() - _parentMargin._left - _parentMargin._right);

		if (_autoPosition & kNuoControl_Stretch_B)
			result.SetH(parentBound.H() - _parentMargin._top - _parentMargin._bottom);

		return result;
	}

	switch (_autoPosition)
	{
	case kNuoControl_NoneAuto:
		break;
	case kNuoControl_RB:
	{
		result.SetX(parentBound.W() - result.W() - margin);
		result.SetY(parentBound.H() - result.H() - margin);
		break;
	}
	case kNuoControl_R:
	{
		result.SetX(parentBound.W() - result.W() - margin);
		result.SetY(originalRect.Y());
		break;
	}
	case kNuoControl_RT:
	{
		result.SetX(parentBound.W() - result.W() - margin);
		result.SetY(margin);
		break;
	}
	default:
		break;
	}

	return result;
}



void NuoControl::SetPosition(const NuoRect<float>& pos, bool activate)
{
	PNuoWindow parent = _parent.lock();
	if (!parent)
		return;

	float dpi = parent->DPI();
	NuoRect<long> devicePos((long)(pos.X() * dpi), (long)(pos.Y() * dpi),
							(long)(pos.W() * dpi), (long)(pos.H() * dpi));

	SetPositionDevice(devicePos, activate);
}


void NuoControl::OnCommand()
{
	_commandFunc();
}


void NuoControl::SetOnCommand(const CommandFunc& func)
{
	_commandFunc = func;
}
