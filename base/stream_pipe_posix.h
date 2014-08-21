// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_PIPE_POSIX_H_
#define BASE_STREAM_PIPE_POSIX_H_

#include <string>
#include "base/stream.h"

namespace base {

/// Implementation of POSIX pipe
/// Can be used to read and write to pipe
class StreamPipePOSIX : public Stream::Impl {
 public:
  /// Constructor
  /// @param[in] filedf Sets the filedf_ to filedf;
  explicit StreamPipePOSIX(int filed);

  /// Destructor
  virtual ~StreamPipePOSIX() {}

  /// Used only for File schema
  virtual bool Seek(ssize_t offset, Stream::SeekType type) override {
    return false;
  }
  virtual size_t GetSize() override {
    return 0;
  }

  virtual size_t Write(const char *buffer, size_t size) override;
  virtual size_t Read(char *buffer, size_t size) override;

  virtual void Close() override;

  static int DescriptorSetMode(
      const std::string& path, Stream::Mode mode);

 protected:
  /// Returns the file descriptor.
  int filedf() const { return filedf_; }

 private:
  std::string path_;
  int filedf_;
};

}  // namespace base

#endif  // BASE_STREAM_PIPE_POSIX_H_
