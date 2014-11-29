// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#ifndef CORE_TIME_PERFORMANCE_FIELD_NAMES_H_
#define CORE_TIME_PERFORMANCE_FIELD_NAMES_H_

#include <string>

namespace core {

/// Here you may add names of clocks that will be assessed
/// and logged via time utils.
const std::string kGrabPerformance = "Grab";
const std::string kFlowPerformance = "Flow";
const std::string kMapCalculationPerformance = "Map calculation";
const std::string kSceneBuildingPerformance = "Scene building";

}  // namespace core

#endif  // CORE_TIME_PERFORMANCE_FIELD_NAMES_H_
