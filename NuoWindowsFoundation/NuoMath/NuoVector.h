#pragma once

#include <memory>
#include <set>
#include <string>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>


#include <DirectXMath.h>


class NuoVector;
typedef std::shared_ptr<NuoVector> PNuoVector;




class NuoVector4
{
public:

	DirectX::XMFLOAT4 _vector;
	FLOAT _float[4];

	NuoVector4() = default;
	NuoVector4(float x, float y, float z, float w);
};

