//
//  NuoModelTextured.cpp
//  ModelViewer
//
//  Created by middleware on 9/5/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#include "NuoModelTextured.h"
#include "NuoMaterial.h"




NuoItemTextured::NuoItemTextured() :
    _position(0), _normal(0), _texCoord(0)
{
}


bool NuoItemTextured::operator == (const NuoItemTextured& i2)
{
    return
        (_position == i2._position) &&
        (_normal == i2._normal) &&
        (_texCoord == i2._texCoord);
}



void NuoModelTextured::SetTexturePathOpacity(const std::string texPath)
{
}



std::string NuoModelTextured::GetTexturePathOpacity()
{
    return std::string();
}



bool NuoModelTextured::HasTransparent()
{
    return false;
}


void NuoModelTextured::GenerateTangents()
{
}


void NuoModelTextured::SetTexturePathBump(const std::string texPath)
{
}


std::string NuoModelTextured::GetTexturePathBump()
{
    return std::string();
}




void NuoModelTextured::AddMaterial(const NuoMaterial& material)
{
}



std::shared_ptr<NuoMaterial> NuoModelTextured::GetUnifiedMaterial()
{
    return nullptr;
}


void NuoModelTextured::UpdateBufferWithUnifiedMaterial()
{
}



NuoMaterial NuoModelTextured::GetMaterial(size_t primtiveIndex) const
{
    return NuoMaterial();
}


