// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENCE file.
/// @author Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include "core/device_manager.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

namespace {

const std::string ROOT_VIDEO = "/dev/video";
const int MAX_DEVICES = 64;

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
  struct v4l2_capability params;
  while (device_index <= MAX_DEVICES) {
    path = ROOT_VIDEO + std::to_string(device_index);
    descriptor = open(path.data(), O_RDONLY);
    if (descriptor == -1) {
      ++device_index; 
      continue;
    }
    ioctl(descriptor, VIDIOC_QUERYCAP, &params);
    device.accessed_by_name = true;
    device.name.assign(reinterpret_cast<char*>(params.card));
    device.index = device_index;
    devices.push_back(device);
    ++device_index;
    close(descriptor);
  }
  return devices;
}

} // core
