// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Konstantin Bormotov <bormotovk@gmail.com

#ifndef BASE_STREAM_NET_WIN_H_
#define BASE_STREAM_NET_WIN_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>

#include "base/stream.h"

namespace base {
  class StreamNetWin : public Stream::Impl {
   public:
    StreamNetWin(const std::string& IPAddress, uint16_t PortNo, Stream::Mode mode);
    StreamNetWin::StreamNetWin();
    virtual ~StreamNetWin();
    bool OpenTCPSocket();
    bool OpenUDPSocket();
    bool BindTCPSocket();
    bool BindUDPSocket();

    virtual size_t Read(char* buffer, size_t size) override;
    virtual size_t Write(const char* buffer, size_t size) override;
    virtual bool Seek(int offset, Stream::SeekType type) override;
    virtual size_t GetSize() override;
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
    int socket_type_;
    SOCKET sock_;
    WSADATA wsadata_;
    Stream::Mode mode_;
    std::string ip_addr_;
    int port_;
    SOCKADDR_IN addr_info;
    void StreamNetWin::SetSockAddr(int family,
                                   const std::string& ip_address,
                                   uint16_t port_no);
    char* ConvertAddr(const std::string& ip_Address);
    void StreamNetWin::SetMode(SOCKET sock, Stream::Mode mode);
  };
}  // namespace base

#endif  // BASE_STREAM_NET_WIN_H_
