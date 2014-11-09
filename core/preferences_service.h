// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_SERVICE_H_
#define CORE_PREFERENCES_SERVICE_H_

#include <memory>
#include <string>

#include "base/singleton.h"

namespace core {

class Preferences;

class PrefService : public base::Singleton<PrefService> {
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

  PrefService();
  virtual ~PrefService() {}

  bool IsRegistered(const std::string& name) const;
  PrefType Type(const std::string& name) const;

  bool Register(PrefType type, const std::string& name, bool value);
  bool Register(PrefType type, const std::string& name, int value);
  bool Register(PrefType type, const std::string& name, double value);
  bool Register(PrefType type, const std::string& name,
    const std::string& value);
  bool Register(PrefType type, const std::string& name);

  bool GetBool(const std::string& name) const;
  int GetInt(const std::string& name) const;
  double GetFloat(const std::string& name) const;
  std::string GetString(const std::string& name) const;

  template<typename T>
  T GetValue(const std::string& name) const;

 protected:
  std::string Path(const std::string& name);
  std::string Name(const std::string& name);

 private:
  std::unique_ptr<Preferences> prefs_;
};

template<>
bool PrefService::GetValue<bool>(const std::string& name) const {
  return GetBool(name);
}

template<>
int PrefService::GetValue<int>(const std::string& name) const {
  return GetInt(name);
}

template<>
double PrefService::GetValue<double>(const std::string& name) const {
  return GetFloat(name);
}

template<>
std::string PrefService::GetValue<std::string>(const std::string& name) const {
  return GetString(name);
}

}  // namespace core

#endif  // CORE_PREFERENCES_SERVICE_H_
