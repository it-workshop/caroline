// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/image_capture_manager.h"

#include "core/preferences_service.h"

namespace {

const char kTimeSettingsNode[] = "time";
const char kTimeSettingsTypeNode[] = "time.controller_type";
const char kTimeSettingsTypeDefault[] = "video";
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

  if (!prefs->RegisterString(kTimeSettingsTypeNode, kTimeSettingsTypeDefault))
    return false;

  if (!prefs->RegisterList(kCapturesNode))
    return false;

  return true;
}

}  // namespace core

