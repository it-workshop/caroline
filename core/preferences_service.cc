// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/preferences_service.h"

#include <string>

#include "base/logging.h"
#include "core/preferences.h"

namespace {

const bool kDefaultBool = false;
const int kDefaultInt = 0;
const double kDefaultFloat = 0;
const std::string kDefaultString = "";
}

namespace core {

PrefService::PrefService()
  : prefs_(std::unique_ptr<Preferences>(new Preferences)) {}

bool PrefService::IsRegistered(const std::string& name) const {
  return (prefs_->Get(name) != nullptr);
}

PrefService::PrefType PrefService::Type(const std::string& name) const {
  Json::Value *curr = prefs_->Get(name);

  if (!curr)
    return PrefType::BAD;
  if (curr->isBool())
    return PrefType::BOOLEAN;
  if (curr->isInt())
    return PrefType::INTEGER;
  if (curr->isDouble())
    return PrefType::FLOAT;
  if (curr->isArray())
    return PrefType::LIST;
  if (curr->isObject())
    return PrefType::DICTIONARY;
}

bool PrefService::Register(PrefType type,
  const std::string& name, bool value) {
  if (type == PrefType::BOOLEAN) {
    return prefs_->Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, int value) {
  if (type == PrefType::INTEGER) {
    return prefs_->Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, double value) {
  if (type == PrefType::FLOAT) {
    return prefs_->Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, const std::string& value) {
  if (type == PrefType::STRING) {
    return prefs_->Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type, const std::string& name) {
  switch (type) {
  case PrefType::BAD:
    return false;
  case PrefType::BOOLEAN:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(kDefaultBool));
  case PrefType::INTEGER:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(kDefaultInt));
  case PrefType::FLOAT:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(kDefaultFloat));
  case PrefType::STRING:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(kDefaultString));
  case PrefType::LIST:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(Json::arrayValue));
  case PrefType::DICTIONARY:
    return prefs_->Add(Path(name), Name(name),
      Json::Value(Json::objectValue));
  default:
    return false;
  }
}

std::string PrefService::Path(const std::string& name) {
  int found = name.find_last_of(Preferences::kNameSeparator);
  return found == -1 ? "" : name.substr(0, found);
}

std::string PrefService::Name(const std::string& name) {
  return name.substr(name.find_last_of(Preferences::kNameSeparator) + 1);
}

bool PrefService::GetBool(const std::string& name) const {
  if (Type(name) == PrefType::BOOLEAN)
    return prefs_->Get(name)->asBool();
  return kDefaultBool;
}

int PrefService::GetInt(const std::string& name) const {
  if (Type(name) == PrefType::INTEGER)
    return prefs_->Get(name)->asInt();
  return kDefaultInt;
}

double PrefService::GetFloat(const std::string& name) const {
  if (Type(name) == PrefType::FLOAT)
    return prefs_->Get(name)->asDouble();
  return kDefaultFloat;
}

std::string PrefService::GetString(const std::string& name) const {
  if (Type(name) == PrefType::STRING)
    return prefs_->Get(name)->asString();
  return kDefaultString;
}

}  // namespace core
