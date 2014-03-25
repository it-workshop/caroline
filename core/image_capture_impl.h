// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com>

#ifndef CORE_IMAGE_CAPTURE_IMPL_H_
#define CORE_IMAGE_CAPTURE_IMPL_H_

#include <string>

#include "core/image_capture.h"

namespace cv {

class VideoCapture;

}  // namespace cv

namespace core {

  // For capturing images from camera or file
class ImageCaptureImpl : public ImageCapture {
 public:
  ImageCaptureImpl(std::unique_ptr<PositionController>&& position_controller,
      const std::string& source_name);
  ImageCaptureImpl(std::unique_ptr<PositionController>&& position_controller,
      int camera_id);
  virtual ~ImageCaptureImpl();

  // Can return empty matrix.
  virtual cv::Mat GetNextImage() override;
  bool GrabNextImage() override;

  cv::VideoCapture* capture() const { return capture_.get(); }

 private:
  std::unique_ptr<cv::VideoCapture> capture_;
};

}  // namespace core

#endif  // CORE_IMAGE_CAPTURE_IMPL_H_
