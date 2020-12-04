//
//  NuoModelBase.cpp
//  ModelViewer
//
//  Created by middleware on 8/28/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#include "NuoModelBase.h"
#include "NuoMaterial.h"

#include "NuoModelLoader/NuoModelTextured.h"
#include "NuoModelLoader/NuoModelMaterialedBasic.h"

#include <DirectXMath.h>


std::shared_ptr<NuoModelBase> CreateModel(const NuoMeshOptions& options, const NuoMaterial& material,
                                          const std::string& modelItemName)
{
    bool textured = options._textured && material.HasTextureDiffuse();

    if (!textured && !options._basicMaterialized)
    {
        auto model = std::make_shared<NuoModelSimple>();
        model->SetName(modelItemName);
        return model;
    }
    else if (textured && !options._basicMaterialized)
    {
        auto model = std::make_shared<NuoModelTextured>();
        model->SetName(modelItemName);
        model->SetCheckTransparency(options._textureEmbedMaterialTransparency);
        return model;
    }
    else if (textured && options._basicMaterialized)
    {
        /*if (material.HasTextureBump() && options._texturedBump)
        {
            auto model = std::make_shared<NuoModelMaterialedBumpedTextured>();
            model->SetName(modelItemName);
            model->SetCheckTransparency(true);
            return model;
        }
        else*/
        {
            auto model = std::make_shared<NuoModelMaterialedTextured>();
            model->SetName(modelItemName);
            model->SetCheckTransparency(true);
            return model;
        }
    }
    else if (options._basicMaterialized)
    {
        auto model = std::make_shared<NuoModelMaterialed>();
        model->SetName(modelItemName);
        return model;
    }
    /*
    else
    {
        auto model = std::shared_ptr<NuoModelBase>();
        return model;
    }*/
    return nullptr;
}


uint32_t* NuoModelBase::IndicesPtr()
{
    return _indices.data();
}

size_t NuoModelBase::IndicesLength()
{
    return _indices.size() * sizeof(uint32_t);
}


size_t NuoModelBase::IndicesCount()
{
    return _indices.size();
}


NuoBounds NuoModelBase::GetBoundingBox()
{
    float xMin = 1e9f, xMax = -1e9f;
    float yMin = 1e9f, yMax = -1e9f;
    float zMin = 1e9f, zMax = -1e9f;

    for (size_t i = 0; i < GetVerticesNumber(); ++i)
    {
        NuoVectorFloat4 position = GetPosition(i);

        xMin = std::min(xMin, position.x());
        xMax = std::max(xMax, position.x());
        yMin = std::min(yMin, position.y());
        yMax = std::max(yMax, position.y());
        zMin = std::min(zMin, position.z());
        zMax = std::max(zMax, position.z());
    }

    NuoVectorFloat3 center((xMax + xMin) / 2.0f, (yMax + yMin) / 2.0f, (zMax + zMin) / 2.0f);
    NuoVectorFloat3 span(xMax - xMin, yMax - yMin, zMax - zMin);

    NuoBounds result;
    result._center = center;
    result._span = span;

    return result;
}


NuoItemSimple::NuoItemSimple()
{
    memset(&_position, sizeof(_position), 0);
    memset(&_normal, sizeof(_normal), 0);
}


bool NuoItemSimple::operator == (const NuoItemSimple& i2)
{
    return DirectX::XMVector4Equal(_position, i2._position) &&
           DirectX::XMVector4Equal(_normal, i2._normal);
}

NuoModelSimple::NuoModelSimple()
{
}


void NuoModelSimple::AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer)
{
}



void NuoModelSimple::AddMaterial(const NuoMaterial& material)
{
}



void NuoModelSimple::GenerateTangents()
{
}


void NuoModelSimple::SetTexturePathDiffuse(const std::string texPath)
{
}



std::string NuoModelSimple::GetTexturePathDiffuse()
{
    return std::string();
}


void NuoModelSimple::SetTexturePathOpacity(const std::string texPath)
{
}


std::string NuoModelSimple::GetTexturePathOpacity()
{
    return std::string();
}


void NuoModelSimple::SetTexturePathBump(const std::string texPath)
{
}


std::string NuoModelSimple::GetTexturePathBump()
{
    return std::string();
}



NuoMaterial NuoModelSimple::GetMaterial(size_t primtiveIndex) const
{
    return NuoMaterial();
}



bool NuoModelSimple::HasTransparent()
{
    return false;
}



std::shared_ptr<NuoMaterial> NuoModelSimple::GetUnifiedMaterial()
{
    return nullptr;
}



void NuoModelSimple::UpdateBufferWithUnifiedMaterial()
{
}
