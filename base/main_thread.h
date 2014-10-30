// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_MAIN_THREAD_H_
#define BASE_MAIN_THREAD_H_

namespace base {

class MainThread : public Thread {
 public:
  explicit MainThread(std::unique_ptr<MessageLoop>&& message_loop)
    : Thread(kMainThread, std::move(message_loop)) {}
  virtual ~MainThread() {}
};

}  // namespace base

#endif  // BASE_MAIN_THREAD_H_
