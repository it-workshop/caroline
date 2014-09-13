// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <memory>
#include <string>

#include "json/reader.h"
#include "json/writer.h"

namespace core {

class Config {
 public:
  Config();
  virtual ~Config();

  static std::shared_ptr<Config> GetInstance();

  Json::Value* dictionary() { return loaded_ ? &dictionary_ : nullptr; }
  const Json::Value* dictionary() const {
    return loaded_ ? &dictionary_ : nullptr;
  }

  // Returns false on error.
  bool LoadFromFile(const std::string& filename);
  bool LoadFromString(const std::string& json);

  std::string SaveToString() const;
  bool SaveToFile(const std::string& filename) const;

 private:
  Json::Reader parser_;
  mutable Json::StyledWriter generator_;
  bool loaded_;
  Json::Value dictionary_;
};

}  // namespace core

#endif  // CORE_CONFIG_H_
