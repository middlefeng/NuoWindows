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



NuoItemMaterialedTexturedBasic::NuoItemMaterialedTexturedBasic()
{
    _position = {};
    _normal = {};
    _texCoord = {};
    _ambient = {};
    _diffuse = {};
    _specular = {};
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



NuoItemMaterialedBasic::NuoItemMaterialedBasic()
{
    _position = {};
    _normal = {};
    _ambient = {};
    _diffuse = {};
    _specular = {};
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


void NuoModelMaterialedTextured::AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer)
{
    NuoModelTextureBase<NuoItemMaterialedTexturedBasic>::AddTexCoord(sourceIndex, texCoordBuffer);
}


void NuoModelMaterialedTextured::AddMaterial(const NuoMaterial& material)
{
    NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>::AddMaterial(material);
}


NuoMaterial NuoModelMaterialedTextured::GetMaterial(size_t primtiveIndex) const
{
    return NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>::GetMaterial(primtiveIndex);
}


bool NuoModelMaterialedTextured::HasTransparent()
{
    return NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>::HasTransparent();
}


std::shared_ptr<NuoMaterial> NuoModelMaterialedTextured::GetUnifiedMaterial()
{
    return NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>::GetUnifiedMaterial();
}


void NuoModelMaterialedTextured::UpdateBufferWithUnifiedMaterial()
{
    return NuoModelMaterialedBasicBase<NuoItemMaterialedTexturedBasic>::UpdateBufferWithUnifiedMaterial();
}


void NuoModelMaterialedTextured::SetTexturePathDiffuse(const std::string texPath)
{
    NuoModelTextureBase<NuoItemMaterialedTexturedBasic>::SetTexturePathDiffuse(texPath);
}


std::string NuoModelMaterialedTextured::GetTexturePathDiffuse()
{
    return NuoModelTextureBase<NuoItemMaterialedTexturedBasic>::GetTexturePathDiffuse();
}


void NuoModelMaterialedTextured::SetTexturePathOpacity(const std::string texPath)
{
    NuoModelTextureBase<NuoItemMaterialedTexturedBasic>::SetTexturePathOpacity(texPath);
}


std::string NuoModelMaterialedTextured::GetTexturePathOpacity()
{
    return NuoModelTextureBase<NuoItemMaterialedTexturedBasic>::GetTexturePathOpacity();
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



