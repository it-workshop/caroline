// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/preferences_service.h"

#include <algorithm>
#include <memory>
#include <string>

#include "base/crash_helper.h"
#include "base/logging.h"

INSTANCE_SINGLETON(core::PrefService);

namespace {

const bool kDefaultBool = false;
const int kDefaultInt = 0;
const double kDefaultFloat = 0.0;
const std::string kDefaultString = "";

}

namespace core {

PrefService::PrefService()
  : prefs_(std::unique_ptr<Preferences>(new Preferences)) {}

bool PrefService::Init() {
  return new PrefService();
}

bool PrefService::IsRegistered(const std::string& name) {
  return (prefs_->Get(name) != nullptr);
}

PrefService::PrefType PrefService::Type(const std::string& name) {
  if (prefs_->Get(name)) {
    return Type(*prefs_->Get(name));
  } else {
    return PrefType::NOT_REGISTERED;
  }
}

PrefService::PrefType PrefService::Type(const Json::Value& value) {
  if (value.isBool())
    return PrefType::BOOLEAN;
  if (value.isInt())
    return PrefType::INTEGER;
  if (value.isDouble())
    return PrefType::FLOAT;
  if (value.isString())
    return PrefType::STRING;
  if (value.isArray())
    return PrefType::LIST;
  if (value.isObject())
    return PrefType::DICTIONARY;
  return PrefType::NOT_REGISTERED;
}

bool PrefService::RegisterBool(const std::string& name, bool value) {
  return prefs_->Add(name, Json::Value(value));
}

bool PrefService::RegisterInt(const std::string& name, int value) {
  return prefs_->Add(name, Json::Value(value));
}

bool PrefService::RegisterFloat(const std::string& name, double value) {
  return prefs_->Add(name, Json::Value(value));
}

bool PrefService::RegisterString(
  const std::string& name, const std::string& value) {
  return prefs_->Add(name, Json::Value(std::string(value)));
}

bool PrefService::RegisterDict(const std::string& name) {
  return prefs_->Add(name, Json::Value(Json::objectValue));
}

bool PrefService::Register(PrefType type, const std::string& name) {
  switch (type) {
  case PrefType::NOT_REGISTERED:
    return false;
  case PrefType::BOOLEAN:
    return prefs_->Add(name,
      Json::Value(kDefaultBool));
  case PrefType::INTEGER:
    return prefs_->Add(name,
      Json::Value(kDefaultInt));
  case PrefType::FLOAT:
    return prefs_->Add(name,
      Json::Value(kDefaultFloat));
  case PrefType::STRING:
    return prefs_->Add(name,
      Json::Value(kDefaultString));
  case PrefType::LIST:
    return prefs_->Add(name,
      Json::Value(Json::arrayValue));
  case PrefType::DICTIONARY:
    return prefs_->Add(name,
      Json::Value(Json::objectValue));
  default:
    return false;
  }
}

bool PrefService::GetBool(const std::string& name) {
  if (Type(name) == PrefType::BOOLEAN)
    return prefs_->Get(name)->asBool();
  CRASH() << "Not registered value!";
}

int PrefService::GetInt(const std::string& name) {
  if (Type(name) == PrefType::INTEGER)
    return prefs_->Get(name)->asInt();
  CRASH() << "Not registered value!";
}

double PrefService::GetFloat(const std::string& name) {
  if (Type(name) == PrefType::FLOAT)
    return prefs_->Get(name)->asDouble();
  CRASH() << "Not registered value!";
}

std::string PrefService::GetString(const std::string& name) {
  if (Type(name) == PrefType::STRING)
    return prefs_->Get(name)->asString();
  CRASH() << "Not registered value!";
}

Json::Value* PrefService::GetDict(const std::string& name) {
  if (Type(name) == PrefType::DICTIONARY)
    return prefs_->Get(name);
  CRASH() << "Not registered value!";
}

Json::Value* PrefService::GetList(const std::string& name) {
  if (Type(name) == PrefType::LIST)
    return prefs_->Get(name);
  CRASH() << "Not registered value!";
}

bool PrefService::SetBool(const std::string& name, bool value) {
  if (Type(name) == PrefType::BOOLEAN) {
    *(prefs_->Get(name)) = Json::Value(value);
    return (GetBool(name) == value);
  }
  return false;
}

bool PrefService::SetInt(const std::string& name, int value) {
  if (Type(name) == PrefType::INTEGER) {
    *(prefs_->Get(name)) = Json::Value(value);
    return (GetInt(name) == value);
  }
  return false;
}

bool PrefService::SetFloat(const std::string& name, double value) {
  if (Type(name) == PrefType::FLOAT) {
    *(prefs_->Get(name)) = Json::Value(value);
    return (GetFloat(name) == value);
  }
  return false;
}

bool PrefService::SetString(const std::string& name, const std::string& value) {
  if (Type(name) == PrefType::STRING) {
    *(prefs_->Get(name)) = Json::Value(value);
    return (GetString(name) == value);
  }
  return false;
}

bool PrefService::LoadFromConfig(const std::string& filename) {
  Preferences pref_config;
  if (!pref_config.LoadFromFile(filename)) {
    LOG(WARNING) << "Can't load config.";
    return false;
  }

  Json::Value::Members pref_members = prefs_->AtomicMembers();
  Json::Value::Members conf_members = pref_config.AtomicMembers();
  Json::Value::Members both_members;

  std::sort(pref_members.begin(), pref_members.end());
  std::sort(conf_members.begin(), conf_members.end());
  std::set_intersection(pref_members.begin(), pref_members.end(),
    conf_members.begin(), conf_members.end(), std::back_inserter(both_members));

  for (size_t i = 0; i < both_members.size(); ++i) {
    std::string curr_name = both_members.at(i);
    switch (Type(curr_name)) {
    case PrefType::BOOLEAN: {
      if (pref_config.Get(curr_name)->isBool()) {
        SetBool(both_members.at(i),
          pref_config.Get(curr_name)->asBool());
      } else {
        LOG(INFO) << "Can't update value. Mismatched types.";
      }
      break;
    }
    case PrefType::INTEGER: {
      if (pref_config.Get(curr_name)->isInt()) {
        SetInt(both_members.at(i),
          pref_config.Get(curr_name)->asInt());
      } else {
        LOG(INFO) << "Can't update value. Mismatched types.";
      }
      break;
    }
    case PrefType::FLOAT: {
      if (pref_config.Get(curr_name)->isDouble()) {
        SetFloat(both_members.at(i),
          pref_config.Get(curr_name)->asDouble());
      } else {
        LOG(INFO) << "Can't update value. Mismatched types.";
      }
      break;
    }
    case PrefType::STRING: {
      if (pref_config.Get(curr_name)->isString()) {
        SetString(both_members.at(i),
          pref_config.Get(curr_name)->asString());
      } else {
        LOG(INFO) << "Can't update value. Mismatched types.";
      }
      break;
    }
    default:
      LOG(INFO) << "Can't update value. Not atomic type.";
    }
  }

  return true;
}

bool PrefService::WriteToConfig(const std::string& filename) {
  return prefs_->SaveToFile(filename);
}

}  // namespace core

