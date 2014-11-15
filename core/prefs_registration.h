// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFS_REGISTRATION_H_
#define CORE_PREFS_REGISTRATION_H_

#include <string>

#include "core/preferences_service.h"

namespace {

const std::string kOpticalFlowNode = "optical-flow";
const std::string kAlgorithmNameNode = "algorithm";
const std::string kLucasKanadeAlgorithmName = "lucas-kanade";
const std::string kFarnebackAlgorithmName = "farneback";

}

namespace core {

bool RegisterOpticalFlowAlgoType(const std::string& value) {
  core::PrefService* pref = core::PrefService::GetInstance();

  return pref->RegisterString(kOpticalFlowNode + kAlgorithmNameNode, value);
}

bool RegisterOpticalFlow() {
  core::PrefService* pref = core::PrefService::GetInstance();

  if (!pref->RegisterDict(kOpticalFlowNode))
    return false;

  if (!RegisterOpticalFlowAlgoType(kLucasKanadeAlgorithmName))
    return false;

  return true;
}

}  // namespace core

#endif  // CORE_PREFS_REGISTRATION_H_
