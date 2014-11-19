// Copyright (c) 2014 The Caroline authors. All right reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENCE file.
/// @author Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include "core/device_manager.h"

#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace {

const char kVideoDeviceFileNameMask[] = "/dev/video";
const int kMaxVideoDevices = 64;

}

namespace core {

class LinuxDeviceManager : public DeviceManager {
public:
  LinuxDeviceManager();
  virtual ~LinuxDeviceManager();

  virtual std::vector<DeviceManager::Device> GetDevices() const override;
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
  int device_index = 0, descriptor, check_value;
  std::string path;
  struct v4l2_capability params;
  while (device_index < kMaxVideoDevices) {
    path = kVideoDeviceFileNameMask + std::to_string(device_index);
    descriptor = open(path.data(), O_RDONLY);
    if (descriptor == -1) {
      ++device_index; 
      continue;
    }
    check_value = ioctl(descriptor, VIDIOC_QUERYCAP, &params);
    if (check_value == -1) {
      ++device_index;
      continue;
    }
    device.accessed_by_path = true;
    device.name.assign(reinterpret_cast<char*>(params.card));
    device.path = path;
    device.index = device_index;
    devices.push_back(device);
    ++device_index;
  }
  return devices;
}

} // core
