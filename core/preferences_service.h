// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_SERVICE_H_
#define CORE_PREFERENCES_SERVICE_H_

#include <memory>
#include <string>

#include "base/macro.h"
#include "core/preferences.h"

namespace core {

class PrefService {
 public:
  enum class PrefType {
    BAD,
    BOOLEAN,
    INTEGER,
    FLOAT,
    STRING,
    LIST,
    DICTIONARY
  };

  PrefService() {}
  virtual ~PrefService() {}

  bool IsRegistered(const std::string& name);
  PrefType Type(const std::string& name);

  bool Register(PrefType type, const std::string& name, bool value);
  bool Register(PrefType type, const std::string& name, int value);
  bool Register(PrefType type, const std::string& name, double value);
  bool Register(PrefType type, const std::string& name,
    const std::string& value);
  bool Register(PrefType type, const std::string& name);

  bool GetBool(const std::string& name);
  int GetInt(const std::string& name);
  double GetFloat(const std::string& name);
  std::string GetString(const std::string& name);

  bool SetBool(const std::string& name, bool value);
  bool SetInt(const std::string& name, int value);
  bool SetFloat(const std::string& name, double value);
  bool SetString(const std::string& name, const std::string& value);

  bool LoadFromConfig(const std::string& filename);
  bool WriteToConfig(const std::string& filename);

 protected:
  PrefType Type(const Json::Value& value);

  std::string Path(const std::string& name);
  std::string Name(const std::string& name);

 private:
  Preferences prefs_;
};

}  // namespace core

#endif  // CORE_PREFERENCES_SERVICE_H_

