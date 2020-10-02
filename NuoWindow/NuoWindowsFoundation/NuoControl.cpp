
#include "NuoControl.h"


NuoControl::NuoControl(const std::string& title, const PNuoWindow& parent)
	: NuoWindow(), _id(-1),
	  _parent(parent), _autoPosition(kNuoControl_NoneAuto)
{
	_title = title;
}


NuoControl::~NuoControl()
{
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


void NuoControl::SetPositionDevice(const NuoRect<long>& pos, bool activate)
{
	PNuoWindow parent = _parent.lock();
	if (parent)
	{
		NuoRect<long> parentRect = parent->ClientRectDevice();
		_parentMargin = parentRect - pos;
	}

	NuoWindow::SetPositionDevice(pos, activate);
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


void NuoControl::SetPosition(const NuoRect<float>& pos, bool activate)
{
	auto parent = _parent.lock();
	if (!parent)
		return;

	auto posDevice = pos * DPI();
	NuoRect<long> posLong(posDevice.X(), posDevice.Y(), posDevice.W(), posDevice.H());
	SetPositionDevice(posLong, activate);
}


NuoRect<long> NuoControl::AutoPositionDevice(float scale, NuoRect<long> parentBound)
{
	static const long margin = (long)(20 * scale);

	NuoRect<long> originalRect = PositionDevice();
	NuoRect<long> result(0, 0, originalRect.W(), originalRect.H());

	switch (_autoPosition)
	{
	case kNuoControl_NoneAuto:
		break;
	case kNuoControl_LT_Stretch:
	{
		auto parent = _parent.lock();
		NuoRect<long> parentRect = parent->ClientRectDevice();

		result.SetX(margin);
		result.SetY(margin);
		result.SetW(parentRect.W() - result.X() - _parentMargin._right);
		result.SetH(parentRect.H() - result.Y() - _parentMargin._bottom);

		break;
	}
	case kNuoControl_RB:
	{
		result.SetX(parentBound.W() - result.W() - margin);
		result.SetY(parentBound.Y() - result.H() - margin);
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



void NuoControl::SetPosition(const NuoRect<long>& pos, bool activate)
{
	PNuoWindow parent = _parent.lock();
	if (!parent)
		return;

	float dpi = parent->DPI();
	auto devicePos = pos * dpi;

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
