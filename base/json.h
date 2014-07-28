// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

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

/// Generates JSON for given base::Value*. Uses compact, valid mode by default.
/// Output can be customized.
class Generator {
 public:
  /// Method of the line wrapping
  enum WrapMode {
    /// Do not wrap lines.
    WRAP_NONE,
    /// Wrap lines if collection has more that one item.
    WRAP_IF_NOT_SINGLE,
    /// Wrap lines always.
    WRAP_ALL
  };

  /// Default constructor.
  Generator();
  /// Destructor.
  virtual ~Generator();

  // Next three options makes sense for style of generated json.
  /// Get indent size.
  ///
  /// 0 by default.
  /// @returns indentation size.
  size_t tab_size() const { return tab_size_; }
  /// Set indent size.
  /// @param[in] tab_size New indentation size value.
  void set_tab_size(size_t tab_size) { tab_size_ = tab_size; }

  /// Get wrapping mode.
  ///
  /// WRAP_NONE by default.
  /// @returns wrapping mode.
  WrapMode wrap_mode() const { return wrap_mode_; }
  /// Set wrapping node.
  /// @param[in] wrap_node New wrapping mode value.
  void set_wrap_mode(WrapMode wrap_mode) { wrap_mode_ = wrap_mode; }

  /// Is it must to write a space after colons in dictionaries.
  ///
  /// False by default.
  /// @returns true if must be a space after the colons.
  bool space_after_colon() const { return space_after_colon_; }
  /// Sets the space_after_colon flag.
  /// @param[in] value New value of the flag.
  void set_space_after_colon(bool value) { space_after_colon_ = value; }

  // Next two options makes JSON invalid, but sometimes are useful.
  /// Use '' instead of "".
  ///
  /// Makes JSON invalid but usable in some cases.
  /// False by default.
  /// @returns true if single quotes will be used.
  bool single_quote() const { return single_quote_; }
  /// Set single_quote flag.
  /// @param[in] value New value of the flag.
  void set_single_quote(bool value) { single_quote_ = value; }

  /// Write comma after last item in the collection.
  ///
  /// Makes JSON invalid but usable in some cases.
  /// False by default.
  /// @returns true if trailing_comma will be used.
  bool trailing_comma() const { return trailing_comma_; }
  /// Set trailing_comma flag.
  /// @param[in] value New value of the flag.
  void set_trailing_comma(bool value) { trailing_comma_ = value; }

  /// Generate json for value using current generator settings.
  /// @param[in] value Value to generate json for.
  /// @returns string with generated json.
  std::string Generate(const Value* value) const;

 private:
  /// Internal method to generate indented json for value.
  /// @param[in] value Value to generate json for.
  /// @param[in] level Indentation level.
  /// @returns string with generated json with given indentation level.
  std::string GenerateImpl(const Value* value, unsigned int level) const;

  /// Internal method to escape characters in the strings.
  /// Replaces " with \" or ' with \' in depend of single_quote flag.
  /// @param[in] string String to escape.
  /// @returns escaped string.
  std::string PrepareString(const std::string& string) const;

  size_t tab_size_;
  WrapMode wrap_mode_;
  bool space_after_colon_;
  bool single_quote_;
  bool trailing_comma_;
};

/// Generate JSON with default generator settings.
/// @param[in] value Value to generate JSON for.
/// @returns string with json.
std::string Generate(const Value* value);

/// Parses json to the base::Value*. Can parse invalid json with custom options
/// enabled.
class Parser {
 public:
  /// Default constructor.
  Parser();
  /// Destructor.
  virtual ~Parser();

  // Next two options makes parser able to parse invalid JSON.
  /// Allow '' instead of "".
  ///
  /// Allows to parse invalid JSON. Usable in some cases.
  /// False by default.
  /// @returns true if allows single quotes.
  bool allow_single_quote() const { return allow_single_quote_; }
  /// Set allow_single_quote flag.
  /// @param[in] value New value of the flag.
  void set_allow_single_quote(bool value) { allow_single_quote_ = value; }

  /// Allow a comma after last item in the collection.
  ///
  /// Allows to parse invalid JSON. Usable in some cases.
  /// False by default.
  /// @returns true if trailing comma is allowed.
  bool allow_trailing_comma() const { return allow_trailing_comma_; }
  /// Set allow_trailing_comma flag.
  /// @param[in] value New value of the flag.
  void set_allow_trailing_comma(bool value) { allow_trailing_comma_ = value; }

  /// Parse given json to the base::Value*.
  /// @param[in] json String with json to parse.
  /// @returns parsed value. Can return nullptr on parsing error.
  base::Value* Parse(const std::string& json) const;

 private:
  /// Type of the parsed value.
  enum ValueType {
    /// base::IntegerValue
    INTEGER,
    /// base::StringValue
    STRING,
    /// base::FloatValue
    FLOAT,
    /// base::BooleanValue
    BOOLEAN,
    /// base::ListValue
    LIST,
    /// base::DictionaryValue
    DICTIONARY,
    /// Parsing error.
    ERROR
  };

  /// Returns type of value by first character. Returns INTEGER for floats if
  /// can't differentiate it by first character.
  /// @param[in] c Character to check.
  /// @returns type of the value.
  ValueType WhatIsStarts(char c) const;

  /// Implementation of the parse method. Parses json from offset.
  ///
  /// Changes offset after parsing.
  /// @param[in]     json   String to parse.
  /// @param[in,out] offset Pointer to the offset.
  /// @returns parsed value.
  Value* ParseImpl(const std::string& json, off_t* offset) const;
  /// Parses string from json at offset.
  ///
  /// Changes offset after parsing.
  /// @param[in]     json   String to parse.
  /// @param[in,out] offset Pointer to the offset.
  /// @returns parsed string value.
  StringValue* ParseString(const std::string& json, off_t* offset) const;
  /// Parses list from json at offset.
  ///
  /// Changes offset after parsing.
  /// @param[in]     json   String to parse.
  /// @param[in,out] offset Pointer to the offset.
  /// @returns parsed list.
  ListValue* ParseList(const std::string& json, off_t* offset) const;
  /// Parse dictionary from json at offset.
  ///
  /// Changes offset after parsing.
  /// @param[in]     json   String to parse.
  /// @param[in,out] offset pointer to the offset.
  /// @returns parsed dictionary.
  DictionaryValue* ParseDictionary(const std::string& json,
      off_t* offset) const;

  bool allow_single_quote_;
  bool allow_trailing_comma_;
};

/// Parse JSON with default parser settings.
/// @param[in] json String to parse.
/// @returns parsed value.
base::Value* Parse(const std::string& json);

}  // namespace json

}  // namespace base

#endif  // BASE_JSON_H_
