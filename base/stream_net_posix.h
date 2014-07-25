// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_NET_POSIX_H_
#define BASE_STREAM_NET_POSIX_H_

#include "base/stream.h"

namespace base {

class StreamNetPOSIX : public Stream::Impl {
 public:
  size_t Read(char* buffer, size_t size);
  size_t Write(const char* buffer, size_t size);

  bool Seek(ssize_t offset, Stream::SeekType type);
  size_t GetSize();

  void Close();
};

}  // namespace base

#endif  // BASE_STREAM_NET_POSIX_
