// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#include "core/preferences.h"

#include <fstream>  // NOLINT
#include <sstream>
#include <string>
#include <vector>

#include "base/logging.h"

namespace {

const char kNameSeparator = '.';

Json::Features ConfigJsonFeatures() {
  Json::Features features;
  features.allowComments_ = true;
  features.strictRoot_ = true;
  features.allowDroppedNullPlaceholders_ = false;
  features.allowNumericKeys_ = false;
  return features;
}

}  // namespace

namespace core {

Preferences::Preferences()
  : parser_(ConfigJsonFeatures()),
    dictionary_(Json::Value(Json::objectValue)) {}

Preferences::Preferences(const Json::Value& dictionary)
  : dictionary_(dictionary) {}

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
  return Add(Path(name), Name(name), value);
}

Json::Value *Preferences::Get(const std::string& name) {
  if (name.empty()) {
    return &dictionary_;
  }

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

Json::Value::Members Preferences::AtomicMembers() const {
  Json::Value::Members members;
  Json::Value::Members curr_members = dictionary_.getMemberNames();

  for (size_t i = 0; i < curr_members.size(); ++i) {
    std::string curr_name = curr_members.at(i);
    if (dictionary_[curr_name].isObject()) {
      AtomicMembers(curr_name, dictionary_[curr_name], &members);
    } else {
      members.push_back(curr_name);
    }
  }

  return members;
}

void Preferences::AtomicMembers(const std::string& name,
  const Json::Value& value, Json::Value::Members* members) const {
  Json::Value::Members curr_members = value.getMemberNames();

  for (size_t i = 0; i < curr_members.size(); ++i) {
    std::string curr_name = curr_members.at(i);
    if (value[curr_name].isObject()) {
      AtomicMembers(name + kNameSeparator + curr_name,
        value[curr_name], members);
    } else {
      members->push_back(name + kNameSeparator + curr_name);
    }
  }
}

bool Preferences::LoadFromFile(const std::string& filename) {
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

bool Preferences::LoadFromString(const std::string& json) {
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

std::string Preferences::SaveToString() const {
  return generator_.write(dictionary_);
}

bool Preferences::SaveToFile(const std::string& filename) const {
  std::ofstream file(filename);
  if (!file.is_open())
    return false;

  const std::string& json = SaveToString();
  file.write(json.c_str(), json.size());
  if (file.fail() || file.bad())
    return false;

  return true;
}

std::string Preferences::Path(const std::string& name) {
  int found = name.find_last_of(kNameSeparator);
  return found == -1 ? "" : name.substr(0, found);
}

std::string Preferences::Name(const std::string& name) {
  return name.substr(name.find_last_of(kNameSeparator) + 1);
}

}  // namespace core

