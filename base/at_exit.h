// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_AT_EXIT_H_
#define BASE_AT_EXIT_H_

#include <functional>
#include <stack>

#include "base/macro.h"

namespace base {

/// Manager for singletones lifetime.
///
/// Must be created at the start of main.
/// Each singleton must write deletion callback to the
/// this object. Calls all callbacks in the destructor.
class AtExitManager {
 public:
  typedef std::function<void()> AtExitCallback;

  /// Constructor.
  AtExitManager();
  /// Destructor. Calls all callbacks.
  ~AtExitManager();

  /// Returns pointer to the object.
  static AtExitManager* GetInstance();
  /// Append new callback.
  /// @param callback Callback to call at exit.
  void RegisterCallback(AtExitCallback callback);

 private:
  /// Pointer to the instance.
  static AtExitManager* instance_;

  /// Callbacks to call at exit.
  std::stack<AtExitCallback> callbacks_;

  /// Allow to stack managers. Keep pointer to the previous instance.
  AtExitManager* previous_instance_;

  DISALLOW_COPY_AND_ASSIGN(AtExitManager);
};

}  // namespace base

#endif  // BASE_AT_EXIT_H_
