// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <memory>
#include <string>

namespace base {

class DictionaryValue;

}  // namespace base

namespace core {

class Config {
 public:
  Config();
  virtual ~Config();

  static std::shared_ptr<Config> GetInstance();

  base::DictionaryValue* dictionary() { return dictionary_.get(); }
  const base::DictionaryValue* dictionary() const { return dictionary_.get(); }

  // Returns false on error.
  bool LoadFromFile(const std::string& filename);
  bool LoadFromString(const std::string& json);

  std::string SaveToString() const;
  bool SaveToFile(const std::string& filename) const;

 private:
  std::unique_ptr<base::DictionaryValue> dictionary_;
};

}  // namespace core

#endif  // CORE_CONFIG_H_
