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
#include "NuoUtilites/NuoTableExporter.h"


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


ModelViewConfiguration::~ModelViewConfiguration()
{
    Save();
}


PNuoDevice ModelViewConfiguration::Device()
{
    return _devices[_deviceName];
}


std::vector<std::string> ModelViewConfiguration::DeviceNames() const
{
    std::vector<std::string> result;

    for (auto device : _devices)
    {
        if (device.second->SupportRayTracing())
            result.push_back(device.first);
    }

    return result;
}


void ModelViewConfiguration::SelectDevice(const std::string& device)
{
    _deviceName = device;
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


void ModelViewConfiguration::Save()
{
    NuoTableExporter exporter;

    exporter.StartTable();

    exporter.StartEntry("device");
    exporter.SetEntryValueString(_deviceName);
    exporter.EndEntry(false);

    exporter.EndTable();

    const std::string& content = exporter.GetResult();

    NuoFile file(_path);
    file.Write((void*)content.c_str(), content.length());
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


