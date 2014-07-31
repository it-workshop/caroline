// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "base/stream_pipe_posix.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "base/logging.h"

namespace base {

std::unique_ptr<Stream> Stream::OpenPipe(
    const std::string& path, Stream::Mode mode) {
  int filed = StreamPipePOSIX::DescriptorSetMode(path, mode);

  if (filed == -1) {
    if (mkfifo(path.c_str(), 0777) == -1) {
      LOG(WARNING) << "Pipe - can't create file.";
      return nullptr;
    }

    filed = StreamPipePOSIX::DescriptorSetMode(path, mode);
  }

  return std::unique_ptr<Stream>(
        new Stream(std::move(std::unique_ptr<Stream::Impl>(
                               new StreamPipePOSIX(filed)))));
}

StreamPipePOSIX::StreamPipePOSIX(int filed)
  : filedf_(filed) {
}

size_t StreamPipePOSIX::Write(const char *buffer, size_t size) {
  int bytes_write = write(filedf(), buffer, size);
  if (bytes_write == -1) {
    LOG(WARNING) << "Pipe - data not write";
  }

  return bytes_write;
}

size_t StreamPipePOSIX::Read(char *buffer, size_t size) {
  int bytes_read = read(filedf(), buffer, size);
  if (bytes_read == -1) {
    LOG(WARNING) << "Pipe - data not read";
  }

  return bytes_read;
}

void StreamPipePOSIX::Close() {
  close(filedf());
}

int StreamPipePOSIX::DescriptorSetMode(
    const std::string& path, Stream::Mode mode) {
  int filed;

  switch (mode) {
  case Stream::kRead: {
    filed = open(path.c_str(), O_RDONLY | O_NONBLOCK);
    break;
  }
  case Stream::kWrite: {
    filed = open(path.c_str(), O_WRONLY | O_NONBLOCK);
    break;
  }
  case Stream::kAppend: {
    filed = open(path.c_str(), O_WRONLY | O_NONBLOCK);
    break;
  }
  case Stream::kReadWrite: {
    filed = open(path.c_str(), O_RDWR | O_NONBLOCK);
    break;
  }
  case Stream::kReadAppend: {
    filed = open(path.c_str(), O_RDWR | O_NONBLOCK);
    break;
  }
  default:
    filed = -1;
  }

  return filed;
}

}  // namespace base
