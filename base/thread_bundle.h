// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREAD_BUNDLE_H_
#define BASE_THREAD_BUNDLE_H_

#include <map>

#include "base/singleton.h"
#include "base/thread.h"

namespace base {

/// Thread bundle is a singleton with collection of all running threads.
class ThreadBundle : public Singleton<ThreadBundle> {
 public:
  /// Constructor.
  ThreadBundle();
  /// Destructor. Calls PostQuitTasks to quit from all message loops before
  /// joining threads.
  virtual ~ThreadBundle();

  /// Create thread bundle.
  /// @returns pointer to created object.
  static ThreadBundle* Create();

  /// Create all threads.
  void Start();

  /// Get thread by type.
  /// @param[in] type Type of required thread.
  /// @returns thread by given type.
  const Thread* Get(Thread::Type type) const;

  /// Get current running thread.
  /// @returns current thread.
  const Thread* GetCurrentThread() const;

  //// Check that current thread is thread with given type.
  /// @param[in] type Type of required thread.
  /// @returns if current thread is required.
  bool IsOnThread(Thread::Type type) const;
  /// Check that current thread is main thread.
  /// @returns true if runs on main thread.
  bool IsOnMainThread() const { return IsOnThread(Thread::kMainThread); }
  /// Check that current thread is io thread.
  /// @returns true if runs on io thread.
  bool IsOnIoThread() const { return IsOnThread(Thread::kIoThread); }
  /// Check that current thread is worker thread.
  /// @returns true if runs on worker thread.
  bool IsOnWorkerThread() const { return IsOnThread(Thread::kWorkerThread); }

  /// Post quit tasks to all running threads.
  void PostQuitTasks() const;

 protected:
  /// Get id of curent thread.
  /// @returns current thread id.
  virtual ThreadId GetCurrentThreadId() const = 0;

  /// Create main thread.
  /// @returns smart pointer to main thread object.
  virtual std::unique_ptr<Thread> CreateMainThread() const = 0;
  /// Create io thread.
  /// @returns smart pointer to io thread object.
  virtual std::unique_ptr<Thread> CreateIoThread() const = 0;
  /// Create worker thread.
  /// @returns smart pointer to worker thread object.
  virtual std::unique_ptr<Thread> CreateWorkerThread() const = 0;

 private:
  std::unique_ptr<Thread> main_thread_;
  std::unique_ptr<Thread> io_thread_;
  std::unique_ptr<Thread> worker_thread_;

  std::map<ThreadId, Thread*> thread_by_id_;
};

}  // namespace base

#endif  // BASE_THREAD_BUNDLE_H_
