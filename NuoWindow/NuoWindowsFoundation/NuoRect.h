#pragma once



template <class T>
class NuoPoint
{
	T _x;
	T _y;

public:

	NuoPoint(T x, T y);

	T X() const { return _x; }
	T Y() const { return _y; }

	void SetX(T x) { _x = x; }
	void SetY(T y) { _y = y; }

};


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

	NuoPoint<T> TL() const { return NuoPoint<T>(_x, _y); }

};



template <class T>
NuoRect<T>::NuoRect(T x, T y, T cx, T cy)
	: _x(x),
	  _y(y),
	  _cx(cx),
	  _cy(cy)
{
}


template <class T>
NuoPoint<T>::NuoPoint(T x, T y)
	: _x(x),
	  _y(y)
{
}