// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/caroline.h"

#include "core/image_capture_manager.h"
#include "core/optical_flow_processor.h"
#include "core/position.h"
#include "core/return_codes.h"
#include "core/time_controller.h"
#include "opencv2/core/mat.hpp"

namespace core {

Caroline::Caroline(base::CommandLine* command_line, Config* config)
  : command_line_(command_line),
    config_(config) {
}

Caroline::~Caroline() {}

bool Caroline::Init() {
  image_capture_manager_ = ImageCaptureManager::Create(config_);
  optical_frow_processor_ = OpticalFlowProcessor::Create(config_);

  return image_capture_manager_ &&
      image_capture_manager_->GetCapturesCount() < 2 &&
      optical_frow_processor_;
}

int Caroline::Run() {
  while (image_capture_manager_->GetTimeController()->Grab()) {
    auto frameset = image_capture_manager_->GetNextFrameset();
    if (frameset.size() < 2)
      return RETURN_WRONG_FRAMES_COUNT;

    auto optical_flow = optical_frow_processor_->Process(
        frameset.at(0).first, frameset.at(1).first);
  }

  return RETURN_OK;
}

}  // namespace core
