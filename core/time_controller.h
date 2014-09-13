// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_TIME_CONTROLLER_H_
#define CORE_TIME_CONTROLLER_H_

#include <ctime>
#include <cinttypes>
#include <vector>

namespace core {

class ImageCaptureImpl;

class TimeController {
 public:
  virtual ~TimeController();

  // Running time in us.
  virtual uint64_t GetCurrentTime() const = 0;

  // Time from the UNIX epoch (01/01/1970 00:00:00 UTC) in seconds.
  virtual time_t GetGlobalTime() const;

  // Add Image Capture for tracking.
  virtual bool AddCapture(ImageCaptureImpl* capture) = 0;

  // Call Grab on all tracked captures. Increase current time if needed.
  virtual bool Grab();

 protected:
  std::vector<ImageCaptureImpl*> captures_;
};

}  // namespace core

#endif  // CORE_TIME_CONTROLLER_H_

