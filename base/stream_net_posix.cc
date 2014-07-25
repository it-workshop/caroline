// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "base/stream_net_posix.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include "base/logging.h"

namespace base {

const std::string kLocal = "127.0.0.1";

std::unique_ptr<Stream> Stream::OpenTCPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  int sockdf = socket(AF_INET, SOCK_STREAM, 0);
  std::unique_ptr<Impl> impl =
      StreamNetPOSIX::init(sockdf, host, port, mode, StreamNetPOSIX::kOpen);
  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::BindTCPSocket(
    uint16_t port, Stream::Mode mode) {
  int sockdf = socket(AF_INET, SOCK_STREAM, 0);
  std::unique_ptr<Impl> impl =
      StreamNetPOSIX::init(sockdf, kLocal, port, mode, StreamNetPOSIX::kBind);

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));

  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::OpenUDPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  int sockdf = socket(AF_INET, SOCK_DGRAM, 0);
  std::unique_ptr<Impl> impl =
      StreamNetPOSIX::init(sockdf, kLocal, port, mode, StreamNetPOSIX::kOpen);

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));

  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::BindUDPSocket(
    uint16_t port, Stream::Mode mode) {
  int sockdf = socket(AF_INET, SOCK_DGRAM, 0);
  std::unique_ptr<Impl> impl =
      StreamNetPOSIX::init(sockdf, kLocal, port, mode, StreamNetPOSIX::kBind);

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));

  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream::Impl> StreamNetPOSIX::init(
    int sockdf, const std::string &host, uint16_t port,
    Stream::Mode mode, ConnectionType cn_type) {
  if (sockdf == -1) {
    LOG(WARNING) << "Socket creation error";
    return nullptr;
  }

  switch (mode) {
  case Stream::kRead: {
    shutdown(sockdf, 1);
    break;
  }
  case Stream::kWrite: {
    shutdown(sockdf, 0);
    break;
  }
  case Stream::kAppend: {
    shutdown(sockdf, 0);
    break;
  }
  default: {
    break;
  }
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(host.c_str());

  if (kBind == cn_type) {
    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Can't bind socket.";
      return nullptr;
    }

    listen(sockdf, 1);
  } else if (kOpen == cn_type) {
    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Can't open socket.";
      return nullptr;
    }
  }

  return std::unique_ptr<Impl>(new StreamNetPOSIX(sockdf));
}

StreamNetPOSIX::StreamNetPOSIX(int sockdf) :
  socket_d_(sockdf) {
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
