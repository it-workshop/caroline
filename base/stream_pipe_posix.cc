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

std::unique_ptr<Stream> Stream::OpenFile(
    const std::string& path, Stream::Mode mode) {
  int filedf = open(path.c_str(), O_RDWR);

  if (filedf == -1) {
    unlink(path.c_str());
    if (mkfifo(path.c_str(), O_RDWR) == -1) {
      LOG(WARNING) << "Pipe - can't create file.";
      return nullptr;
    }

    filedf = open(path.c_str(), O_RDWR);
  }

  return std::unique_ptr<Stream>(
        new Stream(std::move(std::unique_ptr<Stream::Impl>(
                               new StreamPipePOSIX(filedf)))));
}

StreamPipePOSIX::StreamPipePOSIX(int filedf)
  : filedf_(filedf) {
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

}  // namespace base
