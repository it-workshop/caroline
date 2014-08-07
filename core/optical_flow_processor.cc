// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/optical_flow_processor.h"

#include <string>

#include "core/farneback_optical_flow_processor.h"
#include "core/config.h"
#include "core/lucas_kanade_optical_flow_processor.h"

namespace core {

namespace {

const char kOpticalFlowNode[] = "optical-flow";
const char kAlgorithmNameNode[] = "algorithm";
const char kLucasKanadeAlgorithmName[] = "lucas-kanade";
const char kFarnebackAlgorithmName[] = "farneback";

}  // namespace

// static
std::unique_ptr<OpticalFlowProcessor>
OpticalFlowProcessor::Create(const Config* config) {
  if (!config)
    return std::unique_ptr<OpticalFlowProcessor>();

  auto dictionary = config->dictionary();
  if (!dictionary)
    return std::unique_ptr<OpticalFlowProcessor>();

  if (dictionary->isMember(kOpticalFlowNode)) {
    const Json::Value& settings = (*dictionary)[kOpticalFlowNode];
    if (!settings.isObject() || !settings.isMember(kAlgorithmNameNode))
      return std::unique_ptr<OpticalFlowProcessor>();

    const Json::Value& algorithm_node = settings[kAlgorithmNameNode];
    if (!algorithm_node.isString())
      return std::unique_ptr<OpticalFlowProcessor>();

    const std::string& algorithm_name = algorithm_node.asString();

    if (kLucasKanadeAlgorithmName == algorithm_name) {
      return LucasKanadeOpticalFlowProcessor::Create(settings);
    }

    if (kFarnebackAlgorithmName == algorithm_name) {
      return FarnebackOpticalFlowProcessor::Create(settings);
    }
  }

  // There is no matched algorithm. Return empty pointer.
  return std::unique_ptr<OpticalFlowProcessor>();
}

}  // namespace core
