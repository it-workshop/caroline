// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <fstream>  // NOLINT
#include <string>

#include "base/json.h"
#include "base/values.h"
#include "core/config.h"

namespace core {

Config::Config()
  : dictionary_(new base::DictionaryValue()) {}

Config::~Config() {}

// static
std::shared_ptr<Config> Config::GetInstance() {
  static std::shared_ptr<Config> instance(new Config());
  return instance;
}

bool Config::LoadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open())
    return false;

  file.seekg(0, std::ios_base::end);
  size_t size = file.tellg();
  file.seekg(0, std::ios_base::beg);

  std::unique_ptr<char[]> buffer(new char[size]);
  file.read(buffer.get(), size);
  if (file.bad() || file.fail())
    return false;
  file.close();

  return LoadFromString(std::string(buffer.get()));
}

bool Config::LoadFromString(const std::string& json) {
  std::unique_ptr<base::Value> parsed(base::json::Parse(json));
  if (!parsed || !parsed->IsDictionary())
    return false;

  std::unique_ptr<base::DictionaryValue> dictionary(
      parsed.release()->AsDictionary());

  dictionary_.swap(dictionary);
  return true;
}

std::string Config::SaveToString() const {
  base::json::Generator generator;
  generator.set_tab_size(2);
  generator.set_space_after_colon(true);
  generator.set_wrap_mode(base::json::Generator::WRAP_IF_NOT_SINGLE);
  return generator.Generate(dictionary_.get());
}

bool Config::SaveToFile(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open())
    return false;

  const std::string& json = SaveToString();
  file.write(json.c_str(), json.size());
  if (file.fail() || file.bad())
    return false;

  return true;
}

}  // namespace core
