

#include "NuoVector.h"


NuoVector4::NuoVector4(float x, float y, float z, float w)
{
	_vector = { x, y, z, w };
	_float[0] = x;
	_float[1] = y;
	_float[2] = z;
	_float[3] = w;
}



