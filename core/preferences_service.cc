// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/preferences_service.h"

#include <algorithm>
#include <string>

#include "base/logging.h"
#include "core/config.h"

namespace {

const bool kDefaultBool = false;
const int kDefaultInt = 0;
const double kDefaultFloat = 0.0;
const std::string kDefaultString = "";

}

namespace core {

bool PrefService::IsRegistered(const std::string& name) {
  return (prefs_.Get(name) != nullptr);
}

PrefService::PrefType PrefService::Type(const std::string& name) {
  return Type(*prefs_.Get(name));
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
  return PrefType::BAD;
}

bool PrefService::Register(PrefType type,
  const std::string& name, bool value) {
  if (type == PrefType::BOOLEAN) {
    return prefs_.Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, int value) {
  if (type == PrefType::INTEGER) {
    return prefs_.Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, double value) {
  if (type == PrefType::FLOAT) {
    return prefs_.Add(Path(name), Name(name), Json::Value(value));
  } else {
    return false;
  }
}

bool PrefService::Register(PrefType type,
  const std::string& name, const std::string& value) {
  if (type == PrefType::STRING) {
    return prefs_.Add(Path(name), Name(name), Json::Value(std::string(value)));
  } else {
    return false;
  }
}


bool PrefService::Register(PrefType type, const std::string& name) {
  switch (type) {
  case PrefType::BAD:
    return false;
  case PrefType::BOOLEAN:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(kDefaultBool));
  case PrefType::INTEGER:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(kDefaultInt));
  case PrefType::FLOAT:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(kDefaultFloat));
  case PrefType::STRING:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(kDefaultString));
  case PrefType::LIST:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(Json::arrayValue));
  case PrefType::DICTIONARY:
    return prefs_.Add(Path(name), Name(name),
      Json::Value(Json::objectValue));
  default:
    return false;
  }
}

bool PrefService::GetBool(const std::string& name) {
  if (Type(name) == PrefType::BOOLEAN)
    return prefs_.Get(name)->asBool();
  return kDefaultBool;
}

int PrefService::GetInt(const std::string& name) {
  if (Type(name) == PrefType::INTEGER)
    return prefs_.Get(name)->asInt();
  return kDefaultInt;
}

double PrefService::GetFloat(const std::string& name) {
  if (Type(name) == PrefType::FLOAT)
    return prefs_.Get(name)->asDouble();
  return kDefaultFloat;
}

std::string PrefService::GetString(const std::string& name) {
  if (Type(name) == PrefType::STRING)
    return prefs_.Get(name)->asString();
  return kDefaultString;
}

bool PrefService::SetBool(const std::string& name, bool value) {
  if (Type(name) == PrefType::BOOLEAN) {
    *(prefs_.Get(name)) = Json::Value(value);
    return (GetBool(name) == value);
  }
  return false;
}

bool PrefService::SetInt(const std::string& name, int value) {
  if (Type(name) == PrefType::INTEGER) {
    *(prefs_.Get(name)) = Json::Value(value);
    return (GetInt(name) == value);
  }
  return false;
}

bool PrefService::SetFloat(const std::string& name, double value) {
  if (Type(name) == PrefType::FLOAT) {
    *(prefs_.Get(name)) = Json::Value(value);
    return (GetFloat(name) == value);
  }
  return false;
}

bool PrefService::SetString(const std::string& name, const std::string& value) {
  if (Type(name) == PrefType::STRING) {
    *(prefs_.Get(name)) = Json::Value(value);
    return (GetString(name) == value);
  }
  return false;
}

bool PrefService::LoadFromConfig(const std::string& filename) {
  Config config;
  if (!config.LoadFromFile(filename)) {
    LOG(WARNING) << "Can't load config.";
    return false;
  }

  if (!config.dictionary()) {
    LOG(WARNING) << "Config without dictionary.";
    return false;
  }

  Preferences pref_config(*config.dictionary());
  Json::Value::Members pref_members = prefs_.AtomicMembers();
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
  Config config(*prefs_.Get(std::string()));
  return config.SaveToFile(filename);
}

std::string PrefService::Path(const std::string& name) {
  int found = name.find_last_of(Preferences::kNameSeparator);
  return found == -1 ? "" : name.substr(0, found);
}

std::string PrefService::Name(const std::string& name) {
  return name.substr(name.find_last_of(Preferences::kNameSeparator) + 1);
}

}  // namespace core

