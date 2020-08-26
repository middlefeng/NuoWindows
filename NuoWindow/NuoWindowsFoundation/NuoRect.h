#pragma once

template <class T>
class NuoRect
{
	T _x;
	T _y;
	T _cx;
	T _cy;

public:

	NuoRect(T x, T y, T cx, T cy);

	T X() const { return _x; }
	T Y() const { return _y; }
	T W() const { return _cx; }
	T H() const { return _cy; }

	void SetX(T x) { _x = x; }
	void SetY(T y) { _y = y; }
	void SetW(T w) { _cx = w; }
	void SetH(T h) { _cy = h; }
	
};



template <class T>
NuoRect<T>::NuoRect(T x, T y, T cx, T cy)
	: _x(x),
	  _y(y),
	  _cx(cx),
	  _cy(cy)
{
}