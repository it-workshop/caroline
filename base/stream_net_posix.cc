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

std::unique_ptr<Stream::Impl> StreamNetPOSIXFactory::createImpl(
    const std::string& host, uint16_t port,
    Stream::Mode mode, StreamType type) {
  int sockdf = 0;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(host.c_str());

  if (type == kTCPOpen || type == kTCPBind) {
    sockdf = socket(AF_INET, SOCK_STREAM, 0);
  } else if (type == kUDPOpen ||  type == kUDPBind) {
    sockdf = socket(AF_INET, SOCK_DGRAM, 0);
  }

  if (sockdf == -1) {
    return nullptr;
  }

  SetMode(sockdf, mode);

  switch (type) {
  case kTCPOpen: {
    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't bind";
      return nullptr;
    }
    listen(sockdf, 1);

    return std::unique_ptr<Stream::Impl>(new TCPOpenPOSIX(sockdf));
    break;
  }
  case kTCPBind: {
    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't open";
      return nullptr;
    }

    return std::unique_ptr<Stream::Impl>(new TCPBindPOSIX(sockdf));
    break;
  }
  case kUDPOpen: {
    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't bind";
      return nullptr;
    }

    return std::unique_ptr<Stream::Impl>(new UDPOpenPOSIX(sockdf));
    break;
  }
  case kUDPBind: {
    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't open";
      return nullptr;
    }

    return std::unique_ptr<Stream::Impl>(new UDPBindPOSIX(sockdf));
    break;
  }
  default:
    return nullptr;
  }
}

void StreamNetPOSIXFactory::SetMode(int sockdf, Stream::Mode mode) {
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
}

std::unique_ptr<Stream> Stream::OpenTCPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  std::unique_ptr<Impl> impl =
      StreamNetPOSIXFactory::createImpl(
        host, port, mode, StreamNetPOSIXFactory::kTCPOpen);
  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::BindTCPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  std::unique_ptr<Impl> impl =
      StreamNetPOSIXFactory::createImpl(
        host, port, mode, StreamNetPOSIXFactory::kTCPBind);
  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::OpenUDPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  std::unique_ptr<Impl> impl =
      StreamNetPOSIXFactory::createImpl(
        host, port, mode, StreamNetPOSIXFactory::kUDPOpen);
  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

std::unique_ptr<Stream> Stream::BindUDPSocket(
    const std::string& host, uint16_t port, Stream::Mode mode) {
  std::unique_ptr<Impl> impl =
      StreamNetPOSIXFactory::createImpl(
        host, port, mode, StreamNetPOSIXFactory::kUDPBind);
  if (!impl) {
    return nullptr;
  }

  return std::unique_ptr<Stream>(new Stream(std::move(impl)));
}

void StreamNetPOSIX::SetMode(int sockdf, Stream::Mode mode) {
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
}

StreamNetPOSIX::StreamNetPOSIX(int sockdf) :
  socket_d_(sockdf) {
}

void StreamNetPOSIX::Close() {
  close(socket_d_);
}

size_t TCPOpenPOSIX::Read(char *buffer, size_t size) {
  int sock;

  sock = accept(Sock(), NULL, NULL);
  if (sock < 0) {
    LOG(WARNING) << "Socket - accept error";
    return -1;
  }

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read";
  }

  close(sock);
  return bytes_read;
}

size_t TCPOpenPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = accept(Sock(), NULL, NULL);

  if (sock < 0) {
    LOG(WARNING) << "Socket - accept error";
    return -1;
  }

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write";
  }

  close(sock);
  return bytes_write;
}

size_t TCPBindPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read";
  }

  return bytes_read;
}

size_t TCPBindPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write";
  }

  return bytes_write;
}

size_t UDPOpenPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_read = recvfrom(sock, buffer, size, 0, NULL, NULL);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read";
  }

  return bytes_read;
}

size_t UDPOpenPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_write = sendto(sock, buffer, size, 0, NULL, NULL);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write";
  }

  return bytes_write;
}

size_t UDPBindPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read";
  }

  return bytes_read;
}

size_t UDPBindPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = Sock();

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write";
  }

  return bytes_write;
}

}  // namespace base
