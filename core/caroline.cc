// Copyright (c) 2014 The Caroline authors.  All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/caroline.h"

#include <string>
#include <vector>

#include "core/cameras.h"
#include "core/config.h"
#include "core/depth_map.h"
#include "core/depth_mesh.h"
#include "core/image_capture_manager.h"
#include "core/metric_factory.h"
#include "core/optical_flow_processor.h"
#include "core/position.h"
#include "core/return_codes.h"
#include "core/scene3d.h"
#include "core/serialization.h"
#include "core/time_controller.h"
#include "opencv2/core/core.hpp"

const std::string kStreamConfigFieldName = "connection";
const std::string kMetricsConfigFieldName = "metrics";

namespace core {

Caroline::Caroline(base::CommandLine* command_line, Config* config)
  : command_line_(command_line),
    config_(config),
    cameras_properties_(new Cameras),
    message_(new bitdata::GlobalMessage),
    send_message_(false)  {
}

Caroline::~Caroline() {}

bool Caroline::Init() {
  image_capture_manager_ = ImageCaptureManager::Create(config_);
  optical_flow_processor_ = OpticalFlowProcessor::Create(config_);
  const Json::Value* dictionary = config_->dictionary();
  if (dictionary && dictionary->isMember(kStreamConfigFieldName)) {
    const Json::Value& address_node = (*dictionary)[kStreamConfigFieldName];
    if (address_node.isString()) {
      const std::string& address = address_node.asString();
      message_->SetStream(address);
      base::Logger::GetInstance()->AddObserver(message_.get());
      send_message_ = true;
    }
  }

  if (dictionary && dictionary->isMember(kMetricsConfigFieldName)) {
    const Json::Value* metric_names = &(*dictionary)[kMetricsConfigFieldName];
    if (metric_names->isArray()) {
      for (size_t i = 0; i < metric_names->size(); i++) {
        const Json::Value& name = metric_names[i];
        if (name.isString()) {
          std::unique_ptr<stat::Metric> metric =
              stat::MetricFactory::Create(name.asString());
          if (metric) {
            metrics_.push_back(std::move(metric));
          }
        }
      }
    }
  }

  return image_capture_manager_ &&
      image_capture_manager_->GetCapturesCount() < 2 &&
      optical_flow_processor_;
}

int Caroline::Run() {
  while (image_capture_manager_->GetTimeController()->Grab()) {
    auto frameset = image_capture_manager_->GetNextFrameset();
    if (frameset.size() < 2)
      return RETURN_WRONG_FRAMES_COUNT;

    if (send_message_)
      message_->GenPic(frameset);

    auto optical_flow = optical_flow_processor_->Process(
        frameset.at(0).first, frameset.at(1).first);

    if (send_message_) message_->GenOptFlow(optical_flow);

    int w = frameset.at(0).first.size().width;
    int h = frameset.at(0).first.size().height;

    auto imdata = image_capture_manager_->GetCaptures();
    cameras_properties_->set_K1(Cameras::CameraMatrix(
                                  imdata.at(0)->GetDpm(),
                                  imdata.at(0)->GetFocusLength(),
                                  w, h));

    w = frameset.at(1).first.size().width;
    h = frameset.at(1).first.size().height;

    cameras_properties_->set_K2(Cameras::CameraMatrix(
                                  imdata.at(1)->GetDpm(),
                                  imdata.at(1)->GetFocusLength(),
                                  w, h));

    cameras_properties_->set_P1(Cameras::ProjectiveMatrix(
                                  frameset.at(0).second.rotation(),
                                  frameset.at(0).second.location()));

    cameras_properties_->set_P2(Cameras::ProjectiveMatrix(
                                  frameset.at(1).second.rotation(),
                                  frameset.at(1).second.location()));

    auto depth_map = DepthMap::BuildMap(
          optical_flow, *cameras_properties_, w, h);

    if (!metrics_.empty()) {
      std::vector<cv::Mat> src;
      src.push_back(depth_map->AsCVMat());
      for (size_t i = 0; i < metrics_.size(); i++) {
        metrics_[i]->compute(src);
      }
    }

    if (send_message_) message_->GenDMap(*depth_map.get());

    std::unique_ptr<Mesh> mesh(new DepthMesh(*depth_map, 0, INT_MAX));
    std::unique_ptr<Scene3D> scene(new Scene3D);

    std::unique_ptr<SceneElement> element(new SceneElement(mesh.get()));

    scene->AddElement(element.get());

    if (send_message_)
      message_->GenModel(*scene);
  }

  return RETURN_OK;
}

}  // namespace core
