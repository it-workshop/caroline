// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_MANAGER_IMPL_H_
#define CORE_IMAGE_CAPTURE_MANAGER_IMPL_H_

#include <vector>
#include <utility>

#include "core/image_capture_manager.h"

namespace core {

class ImageCaptureManagerImpl : public ImageCaptureManager {
 public:
  ImageCaptureManagerImpl(std::unique_ptr<TimeController>&& time_controller,
      std::vector<std::unique_ptr<ImageCapture>>&& captures,
      ImageCapture::Type type);
  virtual ~ImageCaptureManagerImpl();

  virtual ImageCapture::Type GetType() const override;
  virtual size_t GetCapturesCount() const override;
  virtual TimeController* GetTimeController() const override;
  virtual std::vector<std::pair<cv::Mat, Position>> GetNextFrameset() override;

 private:
  std::unique_ptr<TimeController> time_controller_;
  std::vector<std::unique_ptr<ImageCapture>> captures_;
  ImageCapture::Type type_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_MANAGER_IMPL_H_
