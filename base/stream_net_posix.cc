// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "base/stream_net_posix.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
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
}

std::unique_ptr<Stream> Stream::BindUDPSocket(
    uint16_t port, Stream::Mode mode) {
}

std::unique_ptr<Stream::Impl> StreamNetPOSIX::init(
    int sockdf, const std::string &host, uint16_t port,
    Stream::Mode mode, ConnectionType cn_type) {
  if (sockdf == -1) {
    LOG(WARNING) << "Socket - creation error";
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

  int type = 1;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = port;

  if (kBind == cn_type) {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't bind";
      return nullptr;
    }

    type = 1;
    listen(sockdf, 1);

  } else if (kOpen == cn_type) {
    addr.sin_addr.s_addr = inet_addr(host.c_str());
    type = 2;

    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't open";
      return nullptr;
    }
  }

  return std::unique_ptr<Impl>(new StreamNetPOSIX(sockdf, type));
}

StreamNetPOSIX::StreamNetPOSIX(int sockdf, int type) :
  socket_d_(sockdf),
  type_(type) {
}

size_t StreamNetPOSIX::Read(char *buffer, size_t size) {
  int sock;

  if (type_ == 1) {
    sock = accept(socket_d_, NULL, NULL);
    if (sock < 0) {
      LOG(WARNING) << "Socket - accept error";
      return -1;
    } else if (type_ == 2) {
      sock = socket_d_;
    }
  }

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read";
  }

  if (type_ == 1) {
    close(sock);
  }
  return bytes_read;
}

size_t StreamNetPOSIX::Write(const char *buffer, size_t size) {
  int sock;

  if (type_ == 1) {
    sock = accept(socket_d_, NULL, NULL);
    if (sock < 0) {
      LOG(WARNING) << "Socket - accept error";
      return -1;
    } else if (type_ == 2) {
      sock = socket_d_;
    }
  }

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write";
  }

  if (type_ == 1) {
    close(sock);
  }
  return bytes_write;
}

bool StreamNetPOSIX::Seek(ssize_t offset, Stream::SeekType type) {
}

size_t StreamNetPOSIX::GetSize() {
}

void StreamNetPOSIX::Close() {
  close(socket_d_);
}

}  // namespace base
