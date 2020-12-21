//
//  ModelViewConfiguration.h
//  ModelViewer
//
//  Created by Dong on 12/20/2020.
//  Copyright 2020 Dong FEng. All rights reserved.
//

#ifndef __MODEL_VIEW_CONFIGURATION_H__
#define __MODEL_VIEW_CONFIGURATION_H__


#include <string>
#include <memory>
#include <vector>
#include <map>


class NuoDevice;
typedef std::shared_ptr<NuoDevice> PNuoDevice;


class ModelViewConfiguration
{

	std::string _deviceName;
	std::string _path;

	std::map<std::string, PNuoDevice> _devices;

public:

	ModelViewConfiguration(const std::string& path);

	void Save();

	PNuoDevice Device();
	std::vector<std::string> DeviceNames() const;

private:

	void Load();
	void InitDeviceName();

};


#endif