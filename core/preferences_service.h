// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PREFERENCES_SERVICE_H_
#define CORE_PREFERENCES_SERVICE_H_

#include <memory>
#include <string>

#include "base/singleton.h"
#include "core/preferences.h"

namespace core {

/// Implementatio of preferences service.
/// You can register, set and get different properties from this.
class PrefService : public base::Singleton<PrefService>{
 public:
  /// All possible types of content.
  enum class PrefType {
    NOT_REGISTERED,
    BOOLEAN,
    INTEGER,
    FLOAT,
    STRING,
    LIST,
    DICTIONARY
  };

  PrefService();
  /// Destructor.
  virtual ~PrefService() {}

  static bool Init();

  /// Computes, is value registered.
  /// @param[in] name Value name.
  /// @returns true, if the value is registered.
  bool IsRegistered(const std::string& name);
  /// Computes the type of value.
  /// @param[i] name Value name.
  /// @returns The type of value.
  PrefType Type(const std::string& name);

  /// Try to register new boolean value in storage.
  /// @param[in] name Value name.
  /// @param[in] value Value to register.
  /// @returns true, if successfully registered.
  bool RegisterBool(const std::string& name, bool value);
  /// Overloaded version of the functions above to work with integer values.
  bool RegisterInt(const std::string& name, int value);
  /// Overloaded version of the functions above to work with double values.
  bool RegisterFloat(const std::string& name, double value);
  /// Overloaded version of the functions above to work with strings.
  bool RegisterString(const std::string& name, const std::string& value);
  /// Try to register dictionary value in storage.
  /// @param[in] name Value name.
  /// @returns true, if successfully registered.
  bool RegisterDict(const std::string& name);
  /// Try to register list value in storage.
  /// @param[in] name Value name.
  /// @returns true, if successfully registered.
  bool RegisterList(const std::string& name);
  /// Try to register new property, and sets the default value.
  /// This function can be used for registration of composite objects.
  /// @param[in] type The type of new value.
  /// @param[in] name Value name.
  /// @returns true, if successfully registered.
  bool Register(PrefType type, const std::string& name);

  /// Try to read boolean value from storage.
  /// It would be better to use after the IsRegistered() function.
  /// @param[in] name Value name.
  /// @returns value if it is found; otherwise returns default boolean value.
  bool GetBool(const std::string& name);
  /// Same as the function above, but returns int value.
  int GetInt(const std::string& name);
  /// Same as the function above, but returns double value.
  double GetFloat(const std::string& name);
  /// Same as the function above, but returns string.
  std::string GetString(const std::string& name);
  Json::Value *GetDict(const std::string& name);
  Json::Value *GetList(const std::string& name);

  /// Try to update registered value.
  /// @param[in] name Value name.
  /// @param[in] value New value.
  /// @returns true, if successfully updated; otherwise returns false.
  bool SetBool(const std::string& name, bool value);
  /// Same as the function above to work with int values.
  bool SetInt(const std::string& name, int value);
  /// Same as the function above to work with double values.
  bool SetFloat(const std::string& name, double value);
  /// Same as the function above to work with strings.
  bool SetString(const std::string& name, const std::string& value);

  /// Try to load properties from the file.
  /// Merges registered properties with the loaded.
  /// @param[in] filename Path to file.
  /// @returns true, if successfully loaded.
  bool LoadFromConfig(const std::string& filename);
  /// Try to save properties to file.
  /// @param[in] filename Path to file.
  /// @returns true, if successfully saved.
  bool WriteToConfig(const std::string& filename);

 protected:
  PrefType Type(const Json::Value& value);

 private:
  /// Storage of the properties.
  std::unique_ptr<Preferences> prefs_;
};

}  // namespace core

#endif  // CORE_PREFERENCES_SERVICE_H_

