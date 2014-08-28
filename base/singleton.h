// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_SINGLETON_H_
#define BASE_SINGLETON_H_

#include "base/at_exit.h"
#include "base/macro.h"

namespace base {

/// Singleton template class.
///
/// Usage:
/// my_class.h:
/// @code
/// #include "base/singleton.h"
///
/// class MyClass : public base::Singleton<MyClass> {
///  public:
///   MyClass();
///   virtual ~MyClass();
/// };
/// @endcode
/// my_class.cc:
/// @code
/// #include "my_class.h"
///
/// INSTANCE_SINGLETON(MyClass);
/// @endcode
template <class T>
class Singleton {
 public:
  /// Constructor.
  ///
  /// Registers remove callback in the AtExitManager.
  Singleton() {
    instance_ = static_cast<T*>(this);
    AtExitManager::GetInstance()->RegisterCallback(&Singleton<T>::Remove);
  }
  /// Destructor.
  virtual ~Singleton() {}

  /// Get instance of the singleton.
  /// @returns Singleton instance.
  static T* GetInstance();

 private:
  /// Remove instance.
  static void Remove() {
    delete instance_;
  }

  /// Pointer to the instance.
  static T* instance_;

  DISALLOW_COPY_AND_ASSIGN(Singleton);
};

#define INSTANCE_SINGLETON(Type) \
    template<> \
    Type* ::base::Singleton<Type>::instance_ = nullptr; \
    template<> \
    Type* ::base::Singleton<Type>::GetInstance() { \
      return instance_; \
    }

}  // namespace base

#endif  // BASE_SINGLETON_H_
