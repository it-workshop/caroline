// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "base/posix_stream.h"

#include <sys/types.h>
#include <sys/socket.h>

namespace base {

std::unique_ptr<Stream> Stream::OpenTCPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  int sokdf = socket(AF_INET, SOCK_STREAM, 0);

}

std::unique_ptr<Stream> Stream::BindTCPSocket(
    uint16_t port, Stream::Mode mode) {

}

std::unique_ptr<Stream> Stream::OpenUDPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {

}

std::unique_ptr<Stream> Stream::BindUDPSocket(
    uint16_t port, Stream::Mode mode) {

}

size_t StreamNetPOSIX::Read(char *buffer, size_t size) {

}

size_t StreamNetPOSIX::Write(const char *buffer, size_t size) {

}

bool StreamNetPOSIX::Seek(ssize_t offset, Stream::SeekType type) {

}

size_t StreamNetPOSIX::GetSize() {

}

void StreamNetPOSIX::Close() {

}


}  // namespace base
