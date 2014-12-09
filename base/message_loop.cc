// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/message_loop.h"

#include "base/thread_bundle.h"

namespace base {

MessageLoop::MessageLoop() {}

MessageLoop::~MessageLoop() {}

// static
MessageLoop* MessageLoop::GetCurrent() {
  return ThreadBundle::GetInstance()->GetCurrentThread()->message_loop();
}

}  // namespace base
