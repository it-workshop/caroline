// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Konstantin Bormotov <bormotovk@gmail.com>

#ifndef BASE_STREAM_NET_WIN_H_
#define BASE_STREAM_NET_WIN_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

#include "base/stream.h"

namespace base {
  class StreamNetWin : public Stream::Impl {
  public:
    /// Constructor
    /// @param[in] IPAddress ip address
    /// @param[in] PortNo port
    /// @param[in] mode mode
    StreamNetWin(const std::string& IPAddress, uint16_t PortNo, Stream::Mode mode);
    /// Default constructor
    StreamNetWin::StreamNetWin();
    /// Destructor
    virtual ~StreamNetWin();
    /// Open TCP socket and connect it to target
    /// @returns True if connection succeed of false if not
    bool OpenTCPSocket();
    /// Open UDP socket
    /// @returns True if succeed of false if not
    bool OpenUDPSocket();
    /// Bind TCP socket and start listening
    /// @returns True if succeed of false if not
    bool BindTCPSocket();    
    /// Open UDP socket and bind it
    /// @returns True if succeed of false if not
    bool BindUDPSocket();
    /// Read data from the stream.
    /// @param[out] buffer Memory buffer, to read into.
    /// @param[in]  size   Size of the buffer and expected data size.
    /// @returns size of readen data.
    virtual size_t Read(char* buffer, size_t size) override;
    /// Write data to the stream.
    /// @param[in] buffer Memory buffer, to write from.
    /// @param[in] size   Size of data to write.
    /// @returns size of written data.
    virtual size_t Write(const char* buffer, size_t size) override;
    /// Change position in the stream.
    /// @param[in] offset Offset to move position at.
    /// @param[in] type   Type of the seek operation.
    /// @returns true if seek was successful.
    virtual bool Seek(int offset, Stream::SeekType type) override;
    /// Retrive size of the file.
    /// @returns size of stream or 0.
    virtual size_t GetSize() override;
    /// Close the stream.
    virtual void Close() override;


    std::string  ip_addr() const {
      return ip_addr_;
    }
    uint16_t  port() const{
      return port_;
    }
    Stream::Mode mode() const{
      return mode_;
    }

  private:
    /// Socket type (tcp or udp)
    int socket_type_;
    /// Socket
    SOCKET sock_;
    /// WSA data structurestatic
    WSADATA wsadata_;
    /// Read or write or both mode
    Stream::Mode mode_;
    /// ip address
    std::string ip_addr_;
    /// Port
    int port_;
    /// Structure which contain base information about socket like used protocols
    SOCKADDR_IN addr_info_;
    /// Write ip and port in special structure
    /// param[in] ip_address ip address
    /// param[in] port Port 
    void StreamNetWin::SetSockAddr(int family,
      const std::string& ip_address,
      uint16_t port);
    /// Convert ip address
    /// @param[in] ip_address Ip address 
    /// @returns Converted address
    char* ConvertAddr(const std::string& ip_address);
    /// Set mode
    /// @param[in] sock socket
    /// @param[in] mode Mode of stream
    void StreamNetWin::SetMode(SOCKET sock, Stream::Mode mode);
    /// Initilisation of WSA data
    void InitWSA();
    bool TryAccept();
    ///
    SOCKET accept_socket_;
    bool accepted_;
    void AsyncThisShit();
  };
}  // namespace base

#endif  // BASE_STREAM_NET_WIN_H_
