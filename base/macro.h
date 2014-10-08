// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_MACRO_H_
#define BASE_MACRO_H_

/// Macro for disallowing copying of class.
///
/// Usage example:
/// @code
/// class MyClass {
///  public:
///   MyClass();
///   ~MyClass();
///
///  private:
///   DISALLOW_COPY(MyClass);
/// };
#define DISALLOW_COPY(class_name) \
    class_name(const class_name& object) = delete

/// Macro for disallowing assigning of class.
///
/// Usage example:
/// @code
/// class MyClass {
///  public:
///   MyClass();
///   ~MyClass();
///
///  private:
///   DISALLOW_ASSIGN(MyClass);
/// };
#define DISALLOW_ASSIGN(class_name) \
    class_name& operator=(const class_name& object) = delete

/// Macro for disallowing copying and assigning of class.
///
/// Usage example:
/// @code
/// class MyClass {
///  public:
///   MyClass();
///   ~MyClass();
///
///  private:
///   DISALLOW_COPY_AND_ASSIGN(MyClass);
/// };
#define DISALLOW_COPY_AND_ASSIGN(class_name) \
    DISALLOW_COPY(class_name); \
    DISALLOW_ASSIGN(class_name)

#endif  // BASE_MACRO_H_
