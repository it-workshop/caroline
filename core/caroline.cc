// Copyright (c) 2014 The Caroline authors.  All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/caroline.h"

#include <string>
#include <utility>
#include <vector>

#include "base/message_loop.h"
#include "core/cameras.h"
#include "core/preferences_service.h"
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
#include "core/time_utils.h"
#include "opencv2/core/core.hpp"
#include "core/time_performance_field_names.h"

namespace core {

Caroline::Caroline(base::CommandLine* command_line)
  : command_line_(command_line),
    cameras_properties_(new Cameras),
    message_(new bitdata::GlobalMessage),
    send_message_(false),
    receive_message_(false),
    thread_bundle_(new base::ThreadBundle())  {
}

Caroline::~Caroline() {}

bool Caroline::Init() {
  image_capture_manager_ = ImageCaptureManager::Create();
  optical_flow_processor_ = OpticalFlowProcessor::Create();
  send_message_ = message_->SetOStream();
  if (send_message_)
    base::Logger::GetInstance()->AddObserver(message_.get());
  receive_message_ = message_->SetIStream();

  metrics_ = stat::MetricFactory::CreateFromPreferences();

  cameras_properties_->LoadFromConfig();

  base::MessageLoop::GetCurrent()->PostTask(FROM_HERE,
      std::bind(std::mem_fn(&Caroline::Grab), this));

  return image_capture_manager_ &&
      image_capture_manager_->GetCapturesCount() >= 2 &&
      optical_flow_processor_;
}

int Caroline::Run() {
  base::MessageLoop::GetCurrent()->Run();
  return error_code_;
}

void Caroline::Grab() {
  Clock GrabClock(kGrabPerformance);
  if (!image_capture_manager_->GetTimeController()->Grab()) {
    error_code_ = RETURN_OK;
    base::MessageLoop::GetCurrent()->Quit();
  }

  auto frameset = image_capture_manager_->GetNextFrameset();
  if (frameset.size() < 2) {
    error_code_ = RETURN_WRONG_FRAMES_COUNT;
    base::MessageLoop::GetCurrent()->Quit();
  }

  if (send_message_)
    message_->GenPic(frameset);

  base::MessageLoop::GetCurrent()->PostTask(FROM_HERE,
      std::bind(std::mem_fn(&Caroline::CalculateOpticalFlow), this, frameset));
}

void Caroline::CalculateOpticalFlow(
    std::vector<std::pair<cv::Mat, Position>> frameset) {
  Clock FlowClock(kFlowPerformance);
  auto optical_flow = optical_flow_processor_->Process(
      frameset.at(0).first, frameset.at(1).first);

  if (send_message_)
    message_->GenOptFlow(optical_flow);

  base::MessageLoop::GetCurrent()->PostTask(FROM_HERE,
      std::bind(std::mem_fn(&Caroline::CalculateDepthMap),
                this, frameset, optical_flow));
}

void Caroline::CalculateDepthMap(
    std::vector<std::pair<cv::Mat, Position>> frameset,
    std::vector<std::pair<cv::Point2d, cv::Point2d>> optical_flow) {
  Clock MapCalcClock(kMapCalculationPerformance);
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

  if (send_message_)
    message_->GenDMap(*depth_map.get());
  std::shared_ptr<DepthMap> depth_map_shared(depth_map.release());

  base::MessageLoop::GetCurrent()->PostTask(FROM_HERE,
      std::bind(std::mem_fn(&Caroline::BuildScene), this, depth_map_shared));
}

void Caroline::BuildScene(std::shared_ptr<DepthMap> depth_map) {
  Clock BuildSceneClock(kSceneBuildingPerformance);

  std::unique_ptr<Mesh> mesh(new DepthMesh(*depth_map, 0, INT_MAX));
  std::unique_ptr<Scene3D> scene(new Scene3D);

  std::unique_ptr<SceneElement> element(new SceneElement(mesh.get()));

  scene->AddElement(element.get());

  if (send_message_)
    message_->GenModel(*scene);

  base::MessageLoop::GetCurrent()->PostTask(FROM_HERE,
      std::bind(std::mem_fn(&Caroline::Grab), this));
}

}  // namespace core
