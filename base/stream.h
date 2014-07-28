// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_STREAM_H_
#define BASE_STREAM_H_

#include <memory>
#include <string>

namespace base {

/// File, pipe and socket interface.
///
/// Create an object of this class using Stream::Open.
/// Use simple url-like strings to specifi what you want:
///
///     file:///path/to/the/file
///     pipe:///path/to/the/pipe
///     tcp://host:port
///     udp://host:port
///     tcp-bind://host:port
///     udp-bind://host:port
class Stream {
 public:
  /// A mode that will be used.
  enum Mode {
    /// Read-only mode.
    kRead,
    /// Write-only mode.
    kWrite,
    /// Append-only mode.
    kAppend,
    /// Read-Write mode.
    kReadWrite,
    /// Read-Append mode.
    kReadAppend
  };

  /// Type of seek operation.
  enum SeekType {
    /// Seek from start of file.
    kBegin,
    /// Seek from the current position.
    kCurrent,
    /// Seek from end of file.
    kEnd
  };

  /// file:// schema
  static const char kFileSchema[];
  /// pipe:// schema
  static const char kPipeSchema[];
  /// tcp:// schema
  static const char kTCPSchema[];
  /// tcp-bind:// schema
  static const char kTCPBindSchema[];
  /// udp:// schema
  static const char kUDPSchema[];
  /// udp-bind:// schema
  static const char kUDPBindSchema[];

  /// Implementation interface.
  class Impl {
   public:
    /// Destructor.
    virtual ~Impl() {}

    /// Read data from the stream.
    /// @param[out] buffer Memory buffer, to read into.
    /// @param[in]  size   Size of the buffer and expected data size.
    /// @returns size of readen data.
    virtual size_t Read(char* buffer, size_t size) = 0;
    /// Write data to the stream.
    /// @param[in] buffer Memory buffer, to write from.
    /// @param[in] size   Size of data to write.
    /// @returns size of written data.
    virtual size_t Write(const char* buffer, size_t size) = 0;

    /// Change position in the stream.
    /// @param[in] offset Offset to move position at.
    /// @param[in] type   Type of the seek operation.
    /// @returns true if seek was successful.
    virtual bool Seek(ssize_t offset, SeekType type) = 0;
    /// Retrive size of the file.
    /// @returns size of stream or 0.
    virtual size_t GetSize() = 0;

    /// Close the stream.
    virtual void Close() = 0;

   protected:
    /// Default constructor must be used only from implementations.
    Impl() {}

   private:
    /// Copy constructor is disallowed.
    Impl(const Impl& impl);
    /// Assign operatorn is disallowed.
    Impl& operator=(const Impl& impl);
  };

  /// Create Stream object. Selects an implementation by the schema of url.
  ///
  /// Can return empty pointer on error.
  /// @param[in] url  Url-like string with arguments of open.
  /// @param[in] mode Mode of stream.
  /// @returns smart pointer to the created stream.
  static std::unique_ptr<Stream>
  Open(const std::string& url, Mode mode);

  /// Destructor. Calls Close on impl.
  virtual ~Stream() {
    Close();
  }

  /// Read data from the stream.
  /// @param[out] buffer Memory buffer, to read into.
  /// @param[in]  size   Size of the buffer and expected data size.
  /// @returns size of readen data.
  size_t Read(char* buffer, size_t size) {
    return impl_->Read(buffer, size);
  }
  /// Write data to the stream.
  /// @param[in] buffer Memory buffer, to write from.
  /// @param[in] size   Size of data to write.
  /// @returns size of written data.
  size_t Write(const char* buffer, size_t size) {
    return impl_->Write(buffer, size);
  }

  /// Change position in the stream.
  /// @param[in] offset Offset to move position at.
  /// @param[in] type   Type of the seek operation.
  /// @returns true if seek was successful.
  bool Seek(ssize_t offset, SeekType type) {
    return impl_->Seek(offset, type);
  }
  /// Change position in the stream.
  /// @param[in] offset Offset to move position at.
  /// @param[in] type   Type of the seek operation.
  /// @returns true if seek was successful.
  size_t GetSize() const {
    return impl_->GetSize();
  }

  /// Close the stream.
  void Close() {
    return impl_->Close();
  }

 protected:
  /// Constructor.
  ///
  /// Creates an wrapper around the given Impl.
  /// @param[in] impl Implementation.
  template <class ImplT>
  explicit Stream(std::unique_ptr<ImplT> impl)
    : impl_(impl) {}

 private:
  std::unique_ptr<Impl> impl_;

  /// Will be called when file:// schema is used.
  /// @param[in] path Path to the file.
  /// @param[in] mode Mode of file stream.
  /// @returns Stream for this file.
  static std::unique_ptr<Stream>
  OpenFile(const std::string& path, Mode mode);
  /// Will be called when pipe:// schema is used.
  /// @param[in] path Path to the pipe.
  /// @param[in] mode Mode of file stream.
  /// @returns Stream for this pipe.
  static std::unique_ptr<Stream>
  OpenPipe(const std::string& path, Mode mode);
  /// Will be called when tcp:// schema is used.
  /// @param[in] host host to connect to.
  /// @param[in] port port to connect to.
  /// @param[in] mode Mode of stream.
  /// @returns Stream for this connection.
  static std::unique_ptr<Stream>
  OpenTCPSocket(const std::string& host, uint16_t port, Mode mode);
  /// Will be called when tcp-bind:// schema is used.
  /// @param[in] host host to listen on.
  /// @param[in] port port to connect on.
  /// @param[in] mode Mode of stream.
  /// @returns Stream for this connection.
  static std::unique_ptr<Stream>
  BindTCPSocket(const std::string& host, uint16_t port, Mode mode);
  /// Will be called when udp:// schema is used.
  /// @param[in] host host to connect to.
  /// @param[in] port port to connect to.
  /// @param[in] mode Mode of stream.
  /// @returns Stream for this connection.
  static std::unique_ptr<Stream>
  OpenUDPSocket(const std::string& host, uint16_t port, Mode mode);
  /// Will be called when udp-bind:// schema is used.
  /// @param[in] host host to listen on.
  /// @param[in] port port to connect on.
  /// @param[in] mode Mode of stream.
  /// @returns Stream for this connection.
  static std::unique_ptr<Stream>
  BindUDPSocket(const std::string& host, uint16_t port, Mode mode);

  /// Copy constructor is disabled.
  Stream(const Stream& stream);
  /// Assign operator is disabled.
  Stream& operator=(const Stream& stream);
};

}  // namespace base

#endif  // BASE_STREAM_H_
