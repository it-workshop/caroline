// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "core/image_capture_manager.h"

#include "core/preferences_service.h"

namespace {

const char kTimeSettingsNode[] = "time";
const char kCapturesNode[] = "captures";

}  // namespace

namespace core {

ImageCaptureManager::~ImageCaptureManager() {}

bool ImageCaptureManager::RegisterPreferences() {
  PrefService* prefs = PrefService::GetInstance();

  if (!prefs)
    return false;

  if (!prefs->RegisterDict(kTimeSettingsNode))
    return false;

  if (!prefs->RegisterList(kCapturesNode))
    return false;

  return true;
}

}  // namespace core

