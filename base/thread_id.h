// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_ID_H_
#define BASE_THREAD_ID_H_

#include <memory>

#include "base/macro.h"

namespace base {

#if defined(USE_THREAD_CXX11)
class ThreadIdImplCxx11;
#endif
#if defined(USE_THREAD_PTHREAD)
class ThreadIdImplPThread;
#endif
class ThreadIdImplNone;

/// Thread id is a class that incapsulates platform specific thread-id to
/// identify threads.
class ThreadId {
 public:
  /// Plotform-specific implementation of thread id.
  class Impl {
   public:
    /// Default constructor. implicit default constructor is scheduled by
    /// DISSALLOW_COPY_AND_ASSIGN macro.
    Impl();
    /// Destructor.
    virtual ~Impl();

    /// Compare for equality.
    /// @param[in] that object to compare with.
    /// @returns true if objects are equal.
    virtual bool IsEqualTo(const Impl* that) const = 0;
    /// Compare for sorting.
    /// @param[in] that object to compare with.
    /// @returns thrue if this object is less than that.
    virtual bool IsLessThan(const Impl* that) const = 0;
    /// Copy this object.
    virtual std::unique_ptr<Impl> Copy() const = 0;

#if defined(USE_THREAD_CXX11)
    /// Check type of this object.
    /// @returns true if this object is cxx11 thread id impl.
    virtual bool IsCxx11() const;
    /// Cast impl to child class.
    /// @returns pointer to this object or nullptr.
    virtual const ThreadIdImplCxx11* AsCxx11() const;
#endif
#if defined(USE_THREAD_PTHREAD)
    /// Check type of this object.
    /// @returns true if this object is pthread thread id impl.
    virtual bool IsPThread() const;
    /// Cast impl to child class.
    /// @returns pointer to this object or nullptr.
    virtual const ThreadIdImplPThread* AsPThread() const;
#endif
    // Check type of this object.
    /// @returns true if this object is dummy thread id impl.
    virtual bool IsNone() const;
    /// Cast impl to child class.
    /// @returns pointer to this object or nullptr.
    virtual const ThreadIdImplNone* AsNone() const;
  };

  /// Constructor.
  explicit ThreadId(std::unique_ptr<Impl>&& impl);
  /// Copy constructor.
  /// @param[in] that object to copy.
  ThreadId(const ThreadId& that);
  /// Move contsructor.
  /// @param[in] that object to move.
  ThreadId(ThreadId&& that);

  /// Less operator.
  /// @param[in] that ThreadId to compare with.
  /// @returns true if this less than that.
  bool operator<(const ThreadId& that) const;
  /// Comparation operator.
  /// @param[in] that ThreadId to compare with.
  /// @returns true if this is equal to that.
  bool operator==(const ThreadId& that) const;
  /// Comparation operator.
  /// @param[in] that ThreadId to compare with.
  /// @returns true if this is not equal to that.
  bool operator!=(const ThreadId& that) const;

  /// Get current thread id.
  /// @returns object for current thread.
  static ThreadId GetCurrent();

 private:
  std::unique_ptr<Impl> impl_;
};

}  // namespace base

#endif  // BASE_THREAD_ID_H_

