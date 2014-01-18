// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_JSON_H_
#define BASE_JSON_H_

#include <cstddef>
#include <string>
#include <memory>

namespace base {

class Value;
class StringValue;
class ListValue;
class DictionaryValue;

namespace json {

// Generates JSON for given base::Value*. Uses compact, valid mode by default.
// Output can be customized.
class Generator {
 public:
  enum WrapMode {
    // Do not wrap lines.
    WRAP_NONE,
    // Wrap lines if collection has more that one item.
    WRAP_IF_NOT_SINGLE,
    // Wrap lines always.
    WRAP_ALL
  };

  Generator();
  virtual ~Generator();

  // Next three options makes sense for style of generated json.
  // Indent size. 0 by default.
  size_t tab_size() const { return tab_size_; }
  void set_tab_size(size_t tab_size) { tab_size_ = tab_size; }

  // Wrapping mode. WRAP_NONE by default.
  WrapMode wrap_mode() const { return wrap_mode_; }
  void set_wrap_mode(WrapMode wrap_mode) { wrap_mode_ = wrap_mode; }

  // Write a space after colons in dictionaries. False by default.
  bool space_after_colon() const { return space_after_colon_; }
  void set_space_after_colon(bool value) { space_after_colon_ = value; }

  // Next two options makes JSON invalid, but sometimes are useful.
  // Use '' instead of "". False by default.
  bool single_quote() const { return single_quote_; }
  void set_single_quote(bool value) { single_quote_ = value; }

  // Write comma after last item in the collection.
  bool trailing_comma() const { return trailing_comma_; }
  void set_trailing_comma(bool value) { trailing_comma_ = value; }

  // Generate json for value using current generator settings.
  std::string Generate(const Value* value) const;

 private:
  std::string GenerateImpl(const Value* value, unsigned int level) const;

  std::string PrepareString(const std::string& string) const;

  size_t tab_size_;
  WrapMode wrap_mode_;
  bool space_after_colon_;
  bool single_quote_;
  bool trailing_comma_;
};

// Generate JSON with default generator settings.
std::string Generate(const Value* value);

// Parses json to the base::Value*. Can parse invalid json with custom options
// enabled.
class Parser {
 public:
  Parser();
  virtual ~Parser();

  // Next two options makes parser able to parse invalid JSON.
  // Allow '' instead of "". False by default.
  bool allow_single_quote() const { return allow_single_quote_; }
  void set_allow_single_quote(bool value) { allow_single_quote_ = value; }

  // Allow a comma after last item in the collection. False by default.
  bool allow_trailing_comma() const { return allow_trailing_comma_; }
  void set_allow_trailing_comma(bool value) { allow_trailing_comma_ = value; }

  // Parse given json to the base::Value*. Can return nullptr on parsing error.
  base::Value* Parse(const std::string& json) const;

 private:
  enum ValueType {
    INTEGER,
    STRING,
    FLOAT,
    BOOLEAN,
    LIST,
    DICTIONARY,
    ERROR
  };

  // Returns type of value by first character. Returns INTEGER for floats if
  // can't differentiate it by first character.
  ValueType WhatIsStarts(char c) const;

  Value* ParseImpl(const std::string& json, off_t* offset) const;
  StringValue* ParseString(const std::string& json, off_t* offset) const;
  ListValue* ParseList(const std::string& json, off_t* offset) const;
  DictionaryValue* ParseDictionary(const std::string& json,
      off_t* offset) const;

  bool allow_single_quote_;
  bool allow_trailing_comma_;
};

// Parse JSON with default parser settings.
base::Value* Parse(const std::string& json);

}  // namespace json

}  // namespace base

#endif  // BASE_JSON_H_
