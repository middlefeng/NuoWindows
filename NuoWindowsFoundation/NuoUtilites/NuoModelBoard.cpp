//
//  NuoModelCube.cpp
//  ModelViewer
//
//  Created by middleware on 5/22/17.
//  Copyright © 2017 middleware. All rights reserved.
//

#include "NuoModelBoard.h"
#include "NuoModelLoader/NuoMaterial.h"




NuoModelBoard::NuoModelBoard(float width, float height, float thickness)
    : NuoModelBoardBase<NuoItemSimple>(width, height, thickness),
      _diffuse(0.15f, 0.15f, 0.15f), _specular(0.f, 0.f, 0.f),
      _specularPower(1)
{
}


void NuoModelBoard::AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer)
{
}



void NuoModelBoard::AddMaterial(const NuoMaterial& material)
{
}



void NuoModelBoard::GenerateTangents()
{
}


void NuoModelBoard::SetTexturePathDiffuse(const std::string texPath)
{
}



std::string NuoModelBoard::GetTexturePathDiffuse()
{
    return std::string();
}


void NuoModelBoard::SetTexturePathOpacity(const std::string texPath)
{
}


std::string NuoModelBoard::GetTexturePathOpacity()
{
    return std::string();
}


void NuoModelBoard::SetTexturePathBump(const std::string texPath)
{
}


std::string NuoModelBoard::GetTexturePathBump()
{
    return std::string();
}



NuoMaterial NuoModelBoard::GetMaterial(size_t primtiveIndex) const
{
    return NuoMaterial();
}



bool NuoModelBoard::HasTransparent()
{
    return false;
}



std::shared_ptr<NuoMaterial> NuoModelBoard::GetUnifiedMaterial()
{
    return nullptr;
}



void NuoModelBoard::UpdateBufferWithUnifiedMaterial()
{
}


