// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/at_exit.h"

namespace base {

AtExitManager* AtExitManager::instance_ = nullptr;

AtExitManager::AtExitManager()
  : previous_instance_(instance_) {
  instance_ = this;
}

AtExitManager::~AtExitManager() {
  for (; !callbacks_.empty(); callbacks_.pop())
    callbacks_.top()();
  instance_ = previous_instance_;
}

// static
AtExitManager* AtExitManager::GetInstance() {
  return instance_;
}

void AtExitManager::RegisterCallback(AtExitCallback callback) {
  callbacks_.push(callback);
}

}  // namespace base
