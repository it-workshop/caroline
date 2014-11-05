// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <memory>
#include <string>

#include "base/path.h"
#include "json/reader.h"
#include "json/writer.h"

namespace core {

class Config {
 public:
  Config();
  virtual ~Config();

  static std::shared_ptr<Config> GetInstance();

  Json::Value* dictionary() { return &dictionary_; }
  const Json::Value* dictionary() const {
    return &dictionary_;
  }

  /// Path to used config file. Default is config.json in the work directory.
  /// @returns Path to config.
  base::Path path() const { return path_; }
  /// Set path to config file.
  /// @param [in] path Path to the config.json
  void set_path(const base::Path& path) { path_ = path; }

  // Returns false on error.
  bool LoadFromFile(const std::string& filename);
  bool LoadFromString(const std::string& json);

  /// Load config from file saved into filename field.
  /// @returns true on success.
  bool Load();

  std::string SaveToString() const;
  bool SaveToFile(const std::string& filename) const;
  /// Save config to file saved into filename field.
  /// @returns true on success.
  bool Save() const;

 private:
  Json::Reader parser_;
  mutable Json::StyledWriter generator_;
  Json::Value dictionary_;
  base::Path path_;
};

}  // namespace core

#endif  // CORE_CONFIG_H_
