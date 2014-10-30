// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef BASE_THREADSAFE_QUEUE_H_
#define BASE_THREADSAFE_QUEUE_H_

#include <memory>
#include <utility>

#include "base/lock.h"
#include "base/macro.h"

namespace base {

/// Template class for thread-safe queue.
///
/// Uses C++11 atomic based spin locks for synchronisation.
template <class T>
class ThreadSafeQueue {
 private:
  struct Node;
  struct BaseNode {
    BaseNode()
      : next(nullptr) {}
    ~BaseNode() {
      delete next;
    }

    virtual Node* AsNode() { return nullptr; }

    BaseNode* next;
  };

  /// Element of the queue with a data.
  struct Node : BaseNode {
    /// Template constructor for support both copy and move semantics.
    /// @param[in] data Data of this element.
    template <class TArg>
    explicit Node(TArg&& data)
      : data(std::forward<TArg>(data)) {}

    /// Destructor.
    virtual ~Node() {
    }


    virtual Node* AsNode() override { return this; }

    /// Data of the current element.
    T data;
  };

  std::unique_ptr<BaseNode> head_;
  base::Lock head_lock_;
  BaseNode* tail_;
  base::Lock tail_lock_;

 public:
  /// Default constructor. Creates a queue with only one nil element.
  ThreadSafeQueue()
    : head_(new BaseNode()) {
    tail_ = head_.get();
  }

  /// Destructor. Deletes all elements.
  ~ThreadSafeQueue() {}

  /// Store data to the queue. Template for both copy and move semantics
  /// support.
  /// @param[in] data Data to store.
  template <class TArg>
  void Enqueue(TArg&& data) {
    BaseNode* node = new Node(std::forward<TArg>(data));
    base::ScopedLock scoped(tail_lock_);
    tail_->next = node;
    tail_ = node;
  }

  /// Get data from the queue. Template for both copy and move semantics
  /// support.
  /// @param[in] data pointer to the object which will store loaded data.
  /// @returns false if there is no data loaded.
  template <class TArg>
  bool Dequeue(TArg* data) {
    base::ScopedLock scoped(head_lock_);
    BaseNode* next = head_->next;
    if (!next)
      return false;
    *data = std::move(next->AsNode()->data);
    head_->next = nullptr;
    head_.reset(next);
    return true;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ThreadSafeQueue);
};

}  // namespace base

#endif  // BASE_THREADSAFE_QUEUE_H_
