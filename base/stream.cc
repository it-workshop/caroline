// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <cstdlib>

#include "base/stream.h"

namespace base {

const char Stream::kFileSchema[] = "file";
const char Stream::kPipeSchema[] = "pipe";
const char Stream::kTCPSchema[] = "tcp";
const char Stream::kTCPBindSchema[] = "tcp-bind";
const char Stream::kUDPSchema[] = "udp";
const char Stream::kUDPBindSchema[] = "udp-bind";

// static
std::unique_ptr<Stream> Stream::Open(
    const std::string& url, Mode mode) {
  const std::string::size_type schema_offset = url.find("://");
  if (std::string::npos == schema_offset) {
    // Not url. Use just file stream.
    // return OpenFile(url, mode);
  }
  const std::string schema = url.substr(0, schema_offset);

  if (kFileSchema == schema) {
    const std::string& path = url.substr(schema_offset + 3);
    // return OpenFile(path, mode);
  }

  if (kPipeSchema == schema) {
    const std::string& path = url.substr(schema_offset + 3);
    // return OpenPipe(path, mode);
  }

  // Network protocol. Find port number.
  const std::string::size_type port_offset = url.rfind(":");
  if (std::string::npos == port_offset)
    // No port. Return nullptr.
    return std::unique_ptr<Stream>();

  const std::string& host =
      url.substr(schema_offset + 3, port_offset);
  const std::string& port_str = url.substr(port_offset + 1);
  const uint16_t port = atoi(port_str.c_str());

  if (kTCPSchema == schema) {
    // return OpenTCPSocket(host, port, mode);
  }

  if (kTCPBindSchema == schema) {
    // return BindTCPSocket(port, mode);
  }

  if (kUDPSchema == schema) {
    // return OpenUDPSocket(host, port, mode);
  }

  if (kUDPBindSchema == schema) {
    // return BindUDPSocket(port, mode);
  }

  return std::unique_ptr<Stream>();
}

}  // namespace base
