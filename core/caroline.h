// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_CAROLINE_H_
#define CORE_CAROLINE_H_

#include <memory>
 
#include "core/serialization.h"
#include "base/logging.h"

namespace base {

class CommandLine;

}  // namespace base

namespace core {

class Cameras;
class Config;
class ImageCaptureManager;
class OpticalFlowProcessor;

class Caroline {
 public:
  Caroline(base::CommandLine* command_line, Config* config);
  virtual ~Caroline();

  bool Init();

  int Run();

 private:
  base::CommandLine* command_line_;
  Config* config_;
  bool send_message_;
  std::unique_ptr<bitdata::GlobalMessage>message_;
  std::unique_ptr<ImageCaptureManager> image_capture_manager_;
  std::unique_ptr<OpticalFlowProcessor> optical_flow_processor_;
  std::unique_ptr<Cameras> cameras_properties_;
};

}  // namespace core

#endif  // CORE_CAROLINE_H_
