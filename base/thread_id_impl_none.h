// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_ID_IMPL_NONE_H_
#define BASE_THREAD_ID_IMPL_NONE_H_

#include "base/thread_id.h"

namespace base {

class ThreadIdImplNone : public ThreadId::Impl {
 public:
  explicit ThreadIdImplNone(int id);
  virtual ~ThreadIdImplNone();

  virtual bool IsEqualTo(const ThreadId::Impl* that) const override;
  virtual bool IsLessThan(const ThreadId::Impl* that) const override;
  virtual std::unique_ptr<ThreadId::Impl> Copy() const override;

  virtual bool IsNone() const override;
  virtual const ThreadIdImplNone* AsNone() const override;

 private:
  int id_;
};

}  // namespace base

#endif  // BASE_THREAD_ID_IMPL_NONE_H_

