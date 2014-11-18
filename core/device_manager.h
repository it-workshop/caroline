// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_DEVICE_MANAGER_H_
#define CORE_DEVICE_MANAGER_H_

#include <memory>
#include <string>
#include <vector>

#include "base/macro.h"

namespace core {

/// Class that enumerates video capture devices connected to the computer.
class DeviceManager {
 public:
  DeviceManager();
  virtual ~DeviceManager();

  /// Fabric method. Returns platform-specific implementation.
  static std::unique_ptr<DeviceManager> Create();

  /// This struct will be returned by a manager for each device.
  struct Device {
    /// Must be set to true if device can be accessed by device file name
    /// such as /dev/video0
    bool accessed_by_path;
    /// Device name.
    std::string name;
    /// Device index.
    int index;
    /// Device file name
    std::string path; 
  };

  /// Enumerate devices and return them to caller.
  virtual std::vector<Device> GetDevices() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(DeviceManager);
};

}  // namespace core

#endif  // CORE_DEVICE_MANAGER_H_

