// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_PIPE_POSIX_H_
#define BASE_STREAM_PIPE_POSIX_H_

#include <string>
#include "base/stream.h"

namespace base {

class StreamPipePOSIX : public Stream::Impl {
 public:
  explicit StreamPipePOSIX(int filedf);

  virtual ~StreamPipePOSIX() {}

  virtual bool Seek(ssize_t offset, Stream::SeekType type) override {}
  virtual size_t GetSize() override {}

  virtual size_t Write(const char *buffer, size_t size) override;
  virtual size_t Read(char *buffer, size_t size) override;

  virtual void Close() override;

 protected:
  int filedf() const { return filedf_; }

 private:
  std::string path_;
  int filedf_;
};

}  // namespace base

#endif  // BASE_STREAM_PIPE_POSIX_H_
