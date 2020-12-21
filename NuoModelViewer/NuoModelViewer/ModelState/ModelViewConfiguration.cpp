//
//  ModelViewConfiguration.cpp
//  ModelViewer
//
//  Created by Dong Feng on 12/20/2020.
//  Copyright 2020 Dong Feng. All rights reserved.
//

#include "ModelViewConfiguration.h"

#include "NuoFile.h"
#include "NuoScripting/NuoLua.h"
#include "NuoDirect/NuoDevice.h"


ModelViewConfiguration::ModelViewConfiguration(const std::string& path)
    : _path(path)
{
    Load();

    if (_deviceName.empty())
    {
        InitDeviceName();
    }
    
    bool validDevice = false;

    auto devices = NuoDevice::Devices();
    for (auto device : devices)
    {
        _devices.insert(std::make_pair(device->Name(), device));
        if (device->Name() == _deviceName)
            validDevice = true;
    }

    // eGPU may be removed

    if (!validDevice)
    {
        InitDeviceName();
    }
}


PNuoDevice ModelViewConfiguration::Device()
{
    return _devices[_deviceName];
}


std::vector<std::string> ModelViewConfiguration::DeviceNames() const
{
    std::vector<std::string> result;

    for (auto device : _devices)
        result.push_back(device.first);

    return result;
}


void ModelViewConfiguration::Load()
{
    NuoFile file(_path);
    if (!file.Exists())
        return;

    NuoLua lua;

    lua.LoadFile(_path);

    _deviceName = lua.GetFieldAsString("device", -1);
}



void ModelViewConfiguration::InitDeviceName()
{
    auto devices = NuoDevice::Devices();
    for (auto device : devices)
    {
        if (device->SupportRayTracing())
            _deviceName = device->Name();
    }
}


