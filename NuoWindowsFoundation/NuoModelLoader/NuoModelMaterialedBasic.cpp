//
//  NuoModelMaterialed.cpp
//  ModelViewer
//
//  Created by dfeng on 9/7/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#include "NuoModelMaterialedBasic.h"



template <>
bool ItemTexCoordEequal<NuoItemMaterialedBasic>(const NuoItemMaterialedBasic& i1, const NuoItemMaterialedBasic& i2)
{
    return true;
}



NuoItemMaterialedTexturedBasic::NuoItemMaterialedTexturedBasic() :
    _position(0), _normal(0), _texCoord(0),
    _ambient(0), _diffuse(0), _specular(0)
{
}


bool NuoItemMaterialedTexturedBasic::operator == (const NuoItemMaterialedTexturedBasic& i2)
{
    return
        (_position == i2._position) &&
        (_normal == i2._normal) &&
    
        (_ambient == i2._ambient) &&
        (_diffuse == i2._diffuse) &&
        (_specular == i2._specular) &&
    
        (_texCoord == i2._texCoord);
}


NuoItermMaterialedBumpedTextured::NuoItermMaterialedBumpedTextured() :
    _position(0), _normal(0), _tangent(0), _texCoord(0),
    _ambient(0), _diffuse(0), _specular(0)
{
}


bool NuoItermMaterialedBumpedTextured::operator == (const NuoItermMaterialedBumpedTextured& i2)
{
    return
        (_position == i2._position) &&
        (_normal == i2._normal) &&
        (_tangent == i2._tangent) &&
    
        (_ambient == i2._ambient) &&
        (_diffuse == i2._diffuse) &&
        (_specular.x == i2._specular.x) &&
        (_specular.y == i2._specular.y) &&
        (_specular.z == i2._specular.z) &&
        
        (_texCoord.x == i2._texCoord.x) &&
        (_texCoord.y == i2._texCoord.y);
}


NuoItemMaterialedBasic::NuoItemMaterialedBasic() :
    _position(0), _normal(0),
    _ambient(0), _diffuse(0), _specular(0)
{
}


bool NuoItemMaterialedBasic::operator == (const NuoItemMaterialedBasic& i2)
{
    return
        (_position == i2._position) &&
        (_normal == i2._normal) &&
        
        (_ambient == i2._ambient) &&
        (_diffuse == i2._diffuse) &&
        (_specular == i2._specular);
}



NuoModelMaterialedTextured::NuoModelMaterialedTextured()
{
}



void NuoModelMaterialedTextured::GenerateTangents()
{
}


void NuoModelMaterialedTextured::SetTexturePathBump(const std::string texPath)
{
}


std::string NuoModelMaterialedTextured::GetTexturePathBump()
{
    return std::string();
}



void NuoModelMaterialed::SetTexturePathDiffuse(const std::string texPath)
{
}



std::string NuoModelMaterialed::GetTexturePathDiffuse()
{
    return "";
}


void NuoModelMaterialed::SetTexturePathOpacity(const std::string texPath)
{
}


std::string NuoModelMaterialed::GetTexturePathOpacity()
{
    return std::string();
}


void NuoModelMaterialed::AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer)
{
}


void NuoModelMaterialed::GenerateTangents()
{
}


void NuoModelMaterialed::SetTexturePathBump(const std::string texPath)
{
}


std::string NuoModelMaterialed::GetTexturePathBump()
{
    return std::string();
}



