// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_NET_POSIX_H_
#define BASE_STREAM_NET_POSIX_H_

#include <string>
#include "base/stream.h"

namespace base {

class StreamNetPOSIXFactory {
 public:
  enum StreamType {
    kTCPOpen,
    kTCPBind,
    kUDPOpen,
    kUDPBind
  };

  static std::unique_ptr<Stream::Impl> createImpl(
      const std::string &host, uint16_t port,
      Stream::Mode mode, StreamType type);

  static void SetMode(int sockdf, Stream::Mode mode);
};

class StreamNetPOSIX : public Stream::Impl {
 public:
  enum ConnectionType {
    kOpen,
    kBind
  };

  virtual ~StreamNetPOSIX() {}

  size_t Read(char *buffer, size_t size) {}
  size_t Write(const char *buffer, size_t size) {}

  bool Seek(ssize_t offset, Stream::SeekType type) {}
  size_t GetSize() {}

  void Close();

 protected:
  int Sock() const { return socket_d_; }

  explicit StreamNetPOSIX(int sockdf);

  void SetMode(int sockdf, Stream::Mode mode);

 private:
  int socket_d_;
};

class TCPOpenPOSIX : public StreamNetPOSIX {
 public:
  explicit TCPOpenPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

  size_t Read(char* buffer, size_t size);
  size_t Write(const char* buffer, size_t size);
};

class TCPBindPOSIX: public StreamNetPOSIX {
 public:
  explicit TCPBindPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

  size_t Read(char *buffer, size_t size);
  size_t Write(const char *buffer, size_t size);
};

class UDPOpenPOSIX : public StreamNetPOSIX {
 public:
  explicit UDPOpenPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

  size_t Read(char *buffer, size_t size);
  size_t Write(const char *buffer, size_t size);
};

class UDPBindPOSIX : public StreamNetPOSIX {
 public:
  explicit UDPBindPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

  size_t Read(char *buffer, size_t size);
  size_t Write(const char *buffer, size_t size);
};

}  // namespace base

#endif  // BASE_STREAM_NET_POSIX_H_
