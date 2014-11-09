// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_H_
#define CORE_PREFERENCES_H_

#include <string>

#include "json/value.h"

namespace core {

class Preferences {
 public:
  Preferences();
  virtual ~Preferences();

  static const char kNameSeparator = '.';

  bool Add(const std::string& path,
    const std::string& name, const Json::Value& value);
  bool Add(const std::string& name, const Json::Value& value);

  Json::Value *Get(const std::string& name);

 private:
  Json::Value dictionary_;
};

}  // namespace core


#endif  // CORE_PREFERENCES_H_
