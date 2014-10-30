// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_REAL_THREAD_IMPL_CXX11_H_
#define BASE_REAL_THREAD_IMPL_CXX11_H_

#include <thread>

#include "base/real_thread.h"

namespace base {

class RealThreadImplCxx11 : public RealThread::Impl {
 public:
  explicit RealThreadImplCxx11(const RealThread& thread);
  virtual ~RealThreadImplCxx11();

  virtual RealThread::id_t GetId() const override;

  static void EntryPoint(const RealThreadImplCxx11* self);

 private:
  std::thread thread_;
};

}  // namespace base

#endif  // BASE_REAL_THREAD_IMPL_CXX11_H_
