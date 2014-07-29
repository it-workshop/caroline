// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "base/stream_net_posix.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include "base/logging.h"

namespace base {

const int kSocketNotCreated = -1;
const int kWriteOnly = 0;
const int kReadOnly = 1;

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

  if (sockdf == kSocketNotCreated) {
    return nullptr;
  }

  SetMode(sockdf, mode);

  switch (type) {
  case kTCPOpen: {
    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't bind TCP";
      return nullptr;
    }
    listen(sockdf, 1);

    return std::unique_ptr<Stream::Impl>(new TCPOpenPOSIX(sockdf));
    break;
  }
  case kTCPBind: {
    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't open TCP";
      return nullptr;
    }

    return std::unique_ptr<Stream::Impl>(new TCPBindPOSIX(sockdf));
    break;
  }
  case kUDPOpen: {
    if (bind(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't bind UDP";
      return nullptr;
    }

    return std::unique_ptr<Stream::Impl>(new UDPOpenPOSIX(sockdf));
    break;
  }
  case kUDPBind: {
    if (connect(sockdf, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      LOG(WARNING) << "Socket - can't open UDP";
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
    shutdown(sockdf, kReadOnly);
    break;
  }
  case Stream::kWrite: {
    shutdown(sockdf, kWriteOnly);
    break;
  }
  case Stream::kAppend: {
    shutdown(sockdf, kWriteOnly);
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

StreamNetPOSIX::StreamNetPOSIX(int sockdf) :
  socketdf_(sockdf) {
}

void StreamNetPOSIX::Close() {
  close(socketdf_);
}

size_t TCPOpenPOSIX::Read(char *buffer, size_t size) {
  int sock;

  sock = accept(socketdf(), NULL, NULL);
  if (sock < 0) {
    LOG(WARNING) << "Socket - accept error TCP";
    return -1;
  }

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read TCP";
  }

  close(sock);
  return bytes_read;
}

size_t TCPOpenPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = accept(socketdf(), NULL, NULL);

  if (sock < 0) {
    LOG(WARNING) << "Socket - accept error TCP";
    return -1;
  }

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write TCP";
  }

  close(sock);
  return bytes_write;
}

size_t TCPBindPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read TCP";
  }

  return bytes_read;
}

size_t TCPBindPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write TCP";
  }

  return bytes_write;
}

size_t UDPOpenPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_read = recvfrom(sock, buffer, size, 0, NULL, NULL);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read UDP";
  }

  return bytes_read;
}

size_t UDPOpenPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_write = sendto(sock, buffer, size, 0, NULL, NULL);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write UDP";
  }

  return bytes_write;
}

size_t UDPBindPOSIX::Read(char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_read = recv(sock, buffer, size, 0);
  if (bytes_read < 0) {
    LOG(INFO) << "Socket - data not read UDP";
  }

  return bytes_read;
}

size_t UDPBindPOSIX::Write(const char *buffer, size_t size) {
  int sock;
  sock = socketdf();

  int bytes_write = send(sock, buffer, size, 0);
  if (bytes_write < 0) {
    LOG(INFO) << "Socket - data not write UDP";
  }

  return bytes_write;
}

}  // namespace base
