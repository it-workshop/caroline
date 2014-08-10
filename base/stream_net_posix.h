// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef BASE_STREAM_NET_POSIX_H_
#define BASE_STREAM_NET_POSIX_H_

#include <string>
#include "base/stream.h"

namespace base {

/// POSIX socket factory
/// Used to create different socket types
class StreamNetPOSIXFactory {
 public:
  /// A type of connection to create.
  enum StreamType {
    /// Bind socket of TCP schema and start to listen
    kTCPOpen,
    /// Connect socket of TCP schema
    kTCPBind,
    /// Bind socket of UDP schema
    kUDPOpen,
    /// Connect socket of UDP schema
    kUDPBind
  };

  /// Creates Impl for POSIX stream
  /// @param[in] host host to connect to.
  /// @param[in] port port to connect to.
  /// @param[in] mode Mode of stream.
  /// @param[in] type Type of stream.
  static std::unique_ptr<Stream::Impl> createImpl(
      const std::string &host, uint16_t port,
      Stream::Mode mode, StreamType type);

  /// Sets mode of socket to Mode
  /// @param[in] sockdf socket descriptor
  /// @param[in] mode Mode of stream
  static void SetMode(int sockdf, Stream::Mode mode);
};

/// Implementation of POSIX sockets
/// Inherits the different types of data sending schemas
/// UDP bind and open schemas
/// TCP bind and open schemas
class StreamNetPOSIX : public Stream::Impl {
 public:
  /// Destructor.
  virtual ~StreamNetPOSIX() {}

  /// Not needed for sockets, so empty impl
  virtual bool Seek(ssize_t offset, Stream::SeekType type) override {
    return false;
  }
  /// Empty impl
  virtual size_t GetSize() override {
    return 0;
  }

  /// Close the stream.
  void Close();

 protected:
  /// Return the socket descriptor.
  int socketdf() const { return socketdf_; }

  /// Constructor
  /// @param[in] sockdf Sets the socket descriptor to sockdf.
  explicit StreamNetPOSIX(int sockdf);

 private:
  /// Socket descriptor.
  int socketdf_;
};

/// Implementation of TCP open schema.
class TCPOpenPOSIX : public StreamNetPOSIX {
 public:
  explicit TCPOpenPOSIX(int sockdf)
    : StreamNetPOSIX(sockdf)
  {}

  virtual size_t Read(char* buffer, size_t size) override;
  virtual size_t Write(const char* buffer, size_t size) override;
};

/// Implementation of TCP bind schema.
class TCPBindPOSIX : public StreamNetPOSIX {
 public:
  explicit TCPBindPOSIX(int sockdf)
    : StreamNetPOSIX(sockdf)
  {}

  virtual size_t Read(char *buffer, size_t size) override;
  virtual size_t Write(const char *buffer, size_t size) override;
};

/// Implementation of UDP open schema.
class UDPOpenPOSIX : public StreamNetPOSIX {
 public:
  explicit UDPOpenPOSIX(int sockdf)
    : StreamNetPOSIX(sockdf)
  {}

  virtual size_t Read(char *buffer, size_t size) override;
  virtual size_t Write(const char *buffer, size_t size) override;
};

/// Impelentation of UDP bind schema.
class UDPBindPOSIX : public StreamNetPOSIX {
 public:
  explicit UDPBindPOSIX(int sockdf)
    : StreamNetPOSIX(sockdf)
  {}

  size_t Read(char *buffer, size_t size) override;
  size_t Write(const char *buffer, size_t size) override;
};

}  // namespace base

#endif  // BASE_STREAM_NET_POSIX_H_
