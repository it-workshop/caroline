// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_REGISTRATION_H_
#define CORE_PREFERENCES_REGISTRATION_H_

#include <string>

#include "core/preferences_service.h"

namespace core {

bool RegisterOpticalFlowAlgoType(const std::string& value);
bool RegisterOpticalFlow();

}  // namespace core

#endif  // CORE_PREFERENCES_REGISTRATION_H_
