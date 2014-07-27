// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_NET_POSIX_H_
#define BASE_STREAM_NET_POSIX_H_

#include <string>
#include "base/stream.h"

// NEED TO CHANGE DATAGRAM METHODS

namespace base {

class StreamNetPOSIX : public Stream::Impl {
 public:
  enum ConnectionType {
    kOpen,
    kBind
  };

  virtual ~StreamNetPOSIX() {}
  static std::unique_ptr<Stream::Impl> init(
      int sockdf, const std::string& host, uint16_t port,
      Stream::Mode mode, ConnectionType cn_type);

  size_t Read(char *buffer, size_t size) {}
  size_t Write(const char *buffer, size_t size) {}

  bool Seek(ssize_t offset, Stream::SeekType type) {}
  size_t GetSize() {}

  void Close();

  int Sock() const { return socket_d_; }

 protected:
  explicit StreamNetPOSIX(int sockdf);

 private:
  int socket_d_;
};

class TCPOpenPOSIX : public StreamNetPOSIX {
 public:
  TCPOpenPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

//  ~TCPOpenPOSIX() {}

  size_t Read(char* buffer, size_t size);
  size_t Write(const char* buffer, size_t size);
};

class TCPBindPOSIX: public StreamNetPOSIX {
 public:
  TCPBindPOSIX(int sockdf) :
    StreamNetPOSIX(sockdf)
  {}

//  ~TCPBindPOSIX() {}

  size_t Read(char *buffer, size_t size);
  size_t Write(const char *buffer, size_t size);
};

}  // namespace base

#endif  // BASE_STREAM_NET_POSIX_H_
