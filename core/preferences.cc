// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/preferences.h"

#include <sstream>
#include <string>
#include <vector>

#include "base/logging.h"

namespace core {

Preferences::Preferences() {}

Preferences::~Preferences() {}

bool Preferences::Add(const std::string& path,
  const std::string& name, const Json::Value& value) {
  Json::Value *curr = &dictionary_;

  if (!path.empty()) {
    std::istringstream f(path);
    std::string s;
    while (getline(f, s, kNameSeparator)) {
      if (curr->isMember(s)) {
        curr = &((*curr)[s]);
      } else {
        LOG(WARNING) << "Can't add value. Wrong path.";
        return false;
      }
    }
  }

  if (!name.empty()) {
    if (!curr->isMember(name)) {
      (*curr)[name] = value;
    } else {
      LOG(WARNING) << "Can't add value. That name already exists.";
      return false;
    }
  } else {
    LOG(WARNING) << "Can't add value. Empty name.";
    return false;
  }
  return true;
}

bool Preferences::Add(const std::string& name, const Json::Value& value) {
  return Add(std::string(), name, value);
}

Json::Value *Preferences::Get(const std::string& name) {
  std::vector<std::string> strings;
  std::istringstream f(name);
  std::string s;
  while (getline(f, s, kNameSeparator)) {
    strings.push_back(s);
  }

  Json::Value *curr = &dictionary_;
  for (size_t i = 0; i < strings.size() - 1; ++i) {
    if (curr->isMember(strings[i])) {
      curr = &((*curr)[strings[i]]);
    } else {
      LOG(WARNING) << "Can't get value. Wrong path.";
      return nullptr;
    }
  }

  if (curr->isMember(strings.back())) {
    return &((*curr)[strings.back()]);
  } else {
    LOG(WARNING) << "Can't get value. Wrong name.";
    return nullptr;
  }
}

}  // namespace core
