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

	NuoRect();
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
	NuoRect operator * (const NuoPoint<T>& scale) const;
	NuoRect operator * (float scale) const;
	NuoRect<float> operator / (float scale) const;
};



template <class T>
class NuoInset
{

public:

	T _left;
	T _top;
	T _bottom;
	T _right;

	NuoInset();
	NuoInset(const NuoInset& other);
	NuoInset(T left, T top, T bottom, T right);

	NuoInset& operator = (const NuoInset& other);

};


template <class T>
NuoRect<T>::NuoRect()
	: _x(0),
	  _y(0),
      _cx(0),
	  _cy(0)
{
}


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
NuoInset<T>::NuoInset(T left, T top, T bottom, T right)
	: _left(left),
	  _top(top),
	  _right(right),
	  _bottom(bottom)
{
}


template <class T>
NuoInset<T>::NuoInset()
	: _left(0),
	  _top(0),
	  _right(0),
	  _bottom(0)
{
}


template <class T>
NuoInset<T>::NuoInset(const NuoInset<T>& other)
	: _left(other._left),
	  _top(other._top),
	  _right(other._right),
	  _bottom(other._bottom)
{
}


template <class T>
NuoInset<T>& NuoInset<T>::operator = (const NuoInset<T>& other)
{
	_left = other._left;
	_top = other._top;
	_bottom = other._bottom;
	_right = other._right;

	return *this;
}


template <class T>
NuoInset<T> operator - (const NuoRect<T>& a, const NuoRect<T>& b)
{
	NuoInset<T> result(
		a.X() - b.X(),
		a.Y() - b.Y(),
		a.X() + a.W() - b.X() - b.W(),
		a.Y() + a.H() - b.Y() - b.H()
	);

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


template <class T>
NuoRect<float> NuoRect<T>::operator / (float scale) const
{
	NuoRect<float> result;

	result.SetX(X() / scale);
	result.SetY(Y() / scale);
	result.SetW(W() / scale);
	result.SetH(H() / scale);

	return result;
}

