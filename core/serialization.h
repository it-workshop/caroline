// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author: Vladislav Krikunov <vlad.krikunoff@yandex.ru>

#include <memory>
#include "core/depth_map.h"
#include "core/mesh.h"
#include "core/scene3d.h"
#include "core/point3d.h"
#include "core/image_capture_impl.h"
#include "protocol.pb.h"

namespace bitdata
{

inline void GenDMap (core::DepthMap& defaultM);

void GenOptFlow (core::OpticalFlow defaultFlow);

void GenModel(core::Scene3D defaultScene);

void GenPic(core::ImageCapture* default_pic1,
            core::ImageCapture* default_pic2 );

} // namespace bitdata
