// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_STREAM_H_
#define BASE_STREAM_H_

#include <memory>
#include <string>

namespace base {

class Stream {
 public:
  enum Mode {
    kRead,
    kWrite,
    kAppend,
    kReadWrite,
    kReadAppend
  };

  enum SeekType {
    kBegin,
    kCurrent,
    kEnd
  };

  static const char kFileSchema[];
  static const char kPipeSchema[];
  static const char kTCPSchema[];
  static const char kTCPBindSchema[];
  static const char kUDPSchema[];
  static const char kUDPBindSchema[];

  class Impl {
   public:
    virtual ~Impl() {}

    virtual size_t Read(char* buffer, size_t size) = 0;
    virtual size_t Write(const char* buffer, size_t size) = 0;

    virtual bool Seek(ssize_t offset, SeekType type) = 0;
    virtual size_t GetSize() = 0;

    virtual void Close() = 0;

   protected:
    Impl() {}

   private:
    Impl(const Impl& impl);
    Impl& operator=(const Impl& impl);
  };

  static std::unique_ptr<Stream>
  Open(const std::string& url, Mode mode);

  virtual ~Stream() {
    Close();
  }

  size_t Read(char* buffer, size_t size) {
    return impl_->Read(buffer, size);
  }
  size_t Write(const char* buffer, size_t size) {
    return impl_->Write(buffer, size);
  }

  bool Seek(ssize_t offset, SeekType type) {
    return impl_->Seek(offset, type);
  }
  size_t GetSize() const {
    return impl_->GetSize();
  }

  void Close() {
    return impl_->Close();
  }

 protected:
  template <class ImplT>
  explicit Stream(std::unique_ptr<ImplT> impl)
    : impl_(impl) {}

 private:
  std::unique_ptr<Impl> impl_;

  static std::unique_ptr<Stream>
  OpenFile(const std::string& path, Mode mode);
  static std::unique_ptr<Stream>
  OpenPipe(const std::string& path, Mode mode);
  static std::unique_ptr<Stream>
  OpenTCPSocket(const std::string& host, uint16_t port, Mode mode);
  static std::unique_ptr<Stream>
  BindTCPSocket(uint16_t port, Mode mode);
  static std::unique_ptr<Stream>
  OpenTCPSocket(const std::string& host);
  static std::unique_ptr<Stream>
  OpenUDPSocket(const std::string& host, uint16_t port, Mode mode);
  static std::unique_ptr<Stream>
  BindUDPSocket(uint16_t port, Mode mode);

  Stream(const Stream& stream);
  Stream& operator=(const Stream& stream);
};

}  // namespace base

#endif  // BASE_STREAM_H_
