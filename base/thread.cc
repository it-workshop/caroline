// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "base/thread.h"

#include <utility>

#include "base/message_loop.h"

namespace base {

Thread::Thread(Type type, std::unique_ptr<MessageLoop>&& message_loop)
  : type_(type),
    message_loop_is_owned_(true),
    message_loop_(std::move(message_loop)) {}

Thread::~Thread() {
  if (!message_loop_is_owned_)
    message_loop_.release();
}

}  // namespace base
