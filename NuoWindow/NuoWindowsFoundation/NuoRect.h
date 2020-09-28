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
	NuoRect operator - (const NuoRect& other) const;
	NuoRect operator * (const NuoPoint<T>& scale) const;
	NuoRect operator * (float scale) const;
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


template <class T>
NuoRect<T> NuoRect<T>::operator - (const NuoRect<T>& other) const
{
	NuoRect<T> result = *this;

	result.SetX(result.X() - other.X());
	result.SetY(result.Y() - other.Y());

	return result;
}


template <class T>
NuoRect<T> NuoRect<T>::operator * (const NuoPoint<T>& scale) const
{
	NuoRect<T> result = *this;

	result.SetX(X() * scale.X());
	result.SetY(Y() * scale.Y());
	result.SetW(W() * scale.X());
	result.SetH(H() * scale.Y());

	return result;
}

template <class T>
NuoRect<T> NuoRect<T>::operator * (float scale) const
{
	NuoRect<T> result = *this;

	result.SetX(X() * scale);
	result.SetY(Y() * scale);
	result.SetW(W() * scale);
	result.SetH(H() * scale);

	return result;
}

