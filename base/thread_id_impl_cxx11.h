// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_ID_IMPL_CXX11_H_
#define BASE_THREAD_ID_IMPL_CXX11_H_

#include <memory>
#include <thread>

#include "base/thread_id.h"

namespace base {

class ThreadIdImplCxx11 : public ThreadId::Impl {
 public:
  explicit ThreadIdImplCxx11(std::thread::id id);
  virtual ~ThreadIdImplCxx11();

  virtual bool IsEqualTo(const ThreadId::Impl* that) const override;
  virtual bool IsLessThan(const ThreadId::Impl* that) const override;
  virtual std::unique_ptr<ThreadId::Impl> Copy() const override;

  static std::unique_ptr<ThreadId::Impl> GetForCurrentThread();

  virtual bool IsCxx11() const override;
  virtual const ThreadIdImplCxx11* AsCxx11() const override;

 private:
  std::thread::id id_;
};

}  // namespace base

#endif  // BASE_THREAD_ID_IMPL_CXX11_H_

