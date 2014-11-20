// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_CRASH_HELPER_H_
#define BASE_CRASH_HELPER_H_

#include <memory>

#include "base/logging.h"

namespace base {

class CrashHelper {
 public:
  CrashHelper()
    : message_(Logger::GetInstance()->AddMessage(Logger::LOG_ERROR)) {}

  ~CrashHelper() {
    message_.Flush();
    int* ptr = nullptr;
    *ptr = 0;
  }

  std::ostream& stream() { return message_.stream(); }

 private:
  Logger::Message message_;
};

}  // namespace base

#define CRASH() ::base::CrashHelper().stream() \
  << __LOG_PREFIX(ERROR) "Crashed: "

#endif  // BASE_CRASH_HELPER_H_
