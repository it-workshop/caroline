// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include <fstream>  // NOLINT
#include <string>
#include <vector>

#include "base/logging.h"
#include "core/config.h"

namespace core {

namespace {

const char kDefaultConfigName[] = "config.json";

Json::Features ConfigJsonFeatures() {
  Json::Features features;
  features.allowComments_ = true;
  features.strictRoot_ = true;
  features.allowDroppedNullPlaceholders_ = false;
  features.allowNumericKeys_ = false;
  return features;
}

}  // namespace

Config::Config()
  : parser_(ConfigJsonFeatures()),
    dictionary_(Json::objectValue),
    path_(kDefaultConfigName) {}

Config::Config(const Json::Value& dictionary)
  : parser_(ConfigJsonFeatures()),
    dictionary_(dictionary),
    loaded_(true) {}

Config::~Config() {}

// static
std::shared_ptr<Config> Config::GetInstance() {
  static std::shared_ptr<Config> instance(new Config());
  return instance;
}

bool Config::LoadFromFile(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }

  file.seekg(0, std::ios_base::end);
  size_t size = (size_t) file.tellg();
  file.seekg(0, std::ios_base::beg);

  std::unique_ptr<char[]> buffer(new char[size]);
  file.read(buffer.get(), size);
  if (file.bad() || file.fail())
    return false;
  file.close();

  return LoadFromString(std::string(buffer.get()));
}

bool Config::LoadFromString(const std::string& json) {
  Json::Value root;
  if (!parser_.parse(json, root, true)) {
    std::vector<Json::Reader::StructuredError> errors =
        parser_.getStructuredErrors();
    for (auto error : errors) {
      LOG(ERROR) << '[' << error.offset_start << ".." << error.offset_limit
          << "] " << error.message;
    }
    return false;
  }

  if (!root.isObject()) {
    LOG(ERROR) << "Config root node must be a dictionary!";
    return false;
  }

  dictionary_ = root;
  return true;
}

std::string Config::SaveToString() const {
  return generator_.write(dictionary_);
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

bool Config::Save() const {
  return SaveToFile(path_.spec());
}

}  // namespace core
