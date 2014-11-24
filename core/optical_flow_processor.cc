// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/optical_flow_processor.h"

#include <string>

#include "core/cameras.h"
#include "core/farneback_optical_flow_processor.h"
#include "core/preferences_service.h"
#include "core/lucas_kanade_optical_flow_processor.h"

namespace core {

namespace {

const char kNameSeparator = '.';

const std::string kOpticalFlowNode = "optical-flow";
const std::string kAlgorithmNameNode = "algorithm";
const std::string kLucasKanadeAlgorithmName = "lucas-kanade";
const std::string kFarnebackAlgorithmName = "farneback";

}  // namespace

// static
std::unique_ptr<OpticalFlowProcessor>
OpticalFlowProcessor::Create() {
  PrefService* prefs = PrefService::GetInstance();
  if (!prefs)
    return std::unique_ptr<OpticalFlowProcessor>();

  const std::string& algorithm_name = prefs->GetString(kOpticalFlowNode
    + kNameSeparator + kAlgorithmNameNode);

  if (kLucasKanadeAlgorithmName == algorithm_name) {
    return LucasKanadeOpticalFlowProcessor::Create(
      *prefs->GetDict(kOpticalFlowNode));
  }

  if (kFarnebackAlgorithmName == algorithm_name) {
    return FarnebackOpticalFlowProcessor::Create(
      *prefs->GetDict(kOpticalFlowNode));
  }

  // There is no matched algorithm. Return empty pointer.
  return std::unique_ptr<OpticalFlowProcessor>();
}

bool OpticalFlowProcessor::RegisterPreferences() {
  core::PrefService* pref = core::PrefService::GetInstance();

  if (!pref->RegisterDict(kOpticalFlowNode))
    return false;

  if (!pref->RegisterString(kOpticalFlowNode
    + kNameSeparator + kAlgorithmNameNode, kLucasKanadeAlgorithmName))
    return false;

  return Cameras::RegisterPreferences();
}

}  // namespace core
