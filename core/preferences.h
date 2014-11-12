// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_H_
#define CORE_PREFERENCES_H_

#include <string>

#include "json/value.h"

namespace core {

/// Implementation of preferences storage.
class Preferences {
 public:
  Preferences();
  explicit Preferences(const Json::Value& dictionary);
  /// Destructor.
  virtual ~Preferences() {}

  /// Separator for the names of hierarchical structures.
  static const char kNameSeparator = '.';

  /// Adds new value to the storage.
  /// Can use hierarchical structures.
  /// @param[in] path Path to value. For example - root.config.params.
  /// @param[in] name Value name.
  /// @param[in] value Json value.
  /// @returns true, if successfully added; otherwise returns false.
  bool Add(const std::string& path,
    const std::string& name, const Json::Value& value);
  /// Adds new value to the storage.
  /// The same as the method above, with empty path.
  /// @param[in] name Value name.
  /// @param[in] value Json value.
  /// @returns true, if successfully added; otherwise returns false.
  bool Add(const std::string& name, const Json::Value& value);

  /// Returns the value from storage.
  /// @param[in] name Value name.
  /// @returns Json::Value*, if it finds; otherwise returns nullptr.
  Json::Value *Get(const std::string& name);

  /// Finds all atomic elements of the storage.
  /// @returns The list of atomic elements.
  Json::Value::Members AtomicMembers() const;

 private:
  /// Storage for parameters of the Json object type.
  Json::Value dictionary_;

  void AtomicMembers(const std::string& name,
    const Json::Value& value, Json::Value::Members* members) const;
};

}  // namespace core


#endif  // CORE_PREFERENCES_H_

