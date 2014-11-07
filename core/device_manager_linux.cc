// Copyright (c) 2014 The Caroline authors. All right reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENCE file.
/// @author Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include "core/device_manager.h"

#include <fcntl.h>
#include <unistd.h>

namespace {

const std::string ROOT_VIDEO = "/dev/video";

}

namespace core {

class LinuxDeviceManager : public DeviceManager {
public:
  LinuxDeviceManager();
  ~LinuxDeviceManager();

  std::vector<DeviceManager::Device> GetDevices() const;
};


LinuxDeviceManager::LinuxDeviceManager() {};
LinuxDeviceManager::~LinuxDeviceManager() {};

std::unique_ptr<DeviceManager> DeviceManager::Create() {
  std::unique_ptr<DeviceManager> manager (new LinuxDeviceManager());
  return manager;
}

std::vector<DeviceManager::Device> LinuxDeviceManager::GetDevices() const {
  std::vector<DeviceManager::Device> devices;
  DeviceManager::Device device;
  int device_index = 0, descriptor;
  std::string path;
  for (; ;) {
    path = ROOT_VIDEO + std::to_string(device_index);
    descriptor = open(path.data(), O_RDONLY);
    if (descriptor == -1) 
      break;
    device.accessed_by_name = true;
    device.name = path;
    device.index = device_index;
    devices.push_back(device);
    ++device_index;
    close(path);
  }
  return devices;
}

} // core
