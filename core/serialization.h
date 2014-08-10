// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#ifndef CORE_SERIALIZATION_H_
#define CORE_SERIALIZATION_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/stream.h"
#include "base/logging.h"
#include "core/depth_map.h"
#include "core/image_capture_impl.h"
#include "core/mesh.h"
#include "core/point3d.h"
#include "core/position.h"
#include "core/scene3d.h"

namespace bitdata {

class GlobalMessage {
 public:
  virtual ~GlobalMessage() {}

  std::string stream_name() const {
    return stream_name_;
  }

  void SetStream(const std::string& stream);

  /// Seriallize Depth map with protobuf.
  /// @param[in] depth_map Depth map to seriallize.
  void GenDMap(const core::DepthMap& depth_map);

  /// Seriallize Optical flow with protobuf.
  /// @param[in] seriallize Optical flow to seriallize.
  void GenOptFlow(const core::OpticalFlow& optical_flow);

  /// Seriallize Model with protobuf.
  /// @param[in] scene Scene to se
  void GenModel(const core::Scene3D& scene);

  // Seriallize images from cameras.
  /// @param[in] frameset Collection of images.
  void GenPic(const std::vector<std::pair<cv::Mat, core::Position>>& frameset);

  /// Seriallize log message.
  /// @param[in] message Message to seriallize.
  void GenLog(const std::string& message);

 private:
  std::string stream_name_;
  std::unique_ptr<base::Stream>stream_;
};

}  // namespace bitdata

#endif  // CORE_SERIALIZATION_H_
