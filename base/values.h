// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_VALUES_H_
#define BASE_VALUES_H_

#include <utility>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <memory>

namespace base {

class IntegerValue;
class StringValue;
class FloatValue;
class BooleanValue;
class ListValue;
class DictionaryValue;

/// Base class for other values.
class Value {
 public:
  /// Destructor.
  virtual ~Value();

  /// Is this value integer.
  /// @returns true if can be casted to the IntegerValue.
  virtual bool IsInteger() const;
  /// Is this value string.
  /// @returns true if can be casted to the StringValue.
  virtual bool IsString() const;
  /// Is this value float.
  /// @returns true if can be casted to the FloatValue.
  virtual bool IsFloat() const;
  /// Is this value boolean.
  /// @returns true if can be casted to the BooleanValue.
  virtual bool IsBoolean() const;
  /// Is this value list.
  /// @returns true if can be casted to the ListValue.
  virtual bool IsList() const;
  /// Is this value dictionary.
  /// @returns true if can be casted to the DictionaryValue.
  virtual bool IsDictionary() const;

  /// Casts value to IntegerValue.
  /// @returns this if can be casted or nullptr.
  virtual IntegerValue* AsInteger();
  /// Casts value to StringValue.
  /// @returns this if can be casted or nullptr.
  virtual StringValue* AsString();
  /// Casts value to FloatValue.
  /// @returns this if can be casted or nullptr.
  virtual FloatValue* AsFloat();
  /// Casts value to BooleanValue.
  /// @returns this if can be casted or nullptr.
  virtual BooleanValue* AsBoolean();
  /// Casts value to ListValue.
  /// @returns this if can be casted or nullptr.
  virtual ListValue* AsList();
  /// Casts value to DictionaryValue.
  /// @returns this if can be casted or nullptr.
  virtual DictionaryValue* AsDictionary();

  /// Casts value to IntegerValue.
  /// @returns this if can be casted or nullptr.
  virtual const IntegerValue* AsInteger() const;
  /// Casts value to StringValue.
  /// @returns this if can be casted or nullptr.
  virtual const StringValue* AsString() const;
  /// Casts value to FloatValue.
  /// @returns this if can be casted or nullptr.
  virtual const FloatValue* AsFloat() const;
  /// Casts value to BooleanValue.
  /// @returns this if can be casted or nullptr.
  virtual const BooleanValue* AsBoolean() const;
  /// Casts value to ListValue.
  /// @returns this if can be casted or nullptr.
  virtual const ListValue* AsList() const;
  /// Casts value to DictinaryValue.
  /// @returns this if can be casted or nullptr.
  virtual const DictionaryValue* AsDictionary() const;

  /// Deep copy of the value.
  /// @returns Value
  virtual Value* DeepCopy() const = 0;
};

/// Value for integer numbers.
class IntegerValue : public Value {
 public:
  /// Constructor.
  /// @param[in] value Number to store.
  explicit IntegerValue(int64_t value);
  /// Default constructor.
  IntegerValue();
  /// Destructor.
  virtual ~IntegerValue();

  virtual bool IsInteger() const override;
  virtual IntegerValue* AsInteger() override;
  virtual const IntegerValue* AsInteger() const override;

  virtual Value* DeepCopy() const override;

  /// Set internal value.
  /// @param[in] value Integer number.
  void set_value(int64_t value) { value_ = value; }
  /// Get internal value.
  /// @returns integer number.
  int64_t value() const { return value_; }

 private:
  int64_t value_;

  /// Copy constructor is disallowed.
  IntegerValue(const IntegerValue&);
  /// Assign operator is disallowed.
  IntegerValue& operator=(const IntegerValue&);
};

/// Value for string numbers.
class StringValue : public Value {
 public:
  /// Constructor.
  /// @param[in] value String to store.
  explicit StringValue(const std::string& value);
  /// Default constructor.
  StringValue();
  /// Destructor.
  virtual ~StringValue();

  virtual bool IsString() const override;
  virtual StringValue* AsString() override;
  virtual const StringValue* AsString() const override;

  virtual Value* DeepCopy() const override;

  /// Set internal value.
  /// @param[in] value String.
  void set_value(const std::string& value) { value_ = value; }
  /// Get internal value.
  /// @returns string.
  std::string value() const { return value_; }

 private:
  std::string value_;

  /// Copy constructor is disallowed.
  StringValue(const StringValue&);
  /// Assign operator is disallowed.
  StringValue& operator=(const StringValue&);
};

/// Value for float numbers.
class FloatValue : public Value {
 public:
  /// Constructor.
  /// @param[in] value Float to store.
  explicit FloatValue(double value);
  /// Default constructor.
  FloatValue();
  /// Destructor.
  virtual ~FloatValue();

  virtual bool IsFloat() const override;
  virtual FloatValue* AsFloat() override;
  virtual const FloatValue* AsFloat() const override;

  virtual Value* DeepCopy() const override;

  /// Set internal value.
  /// @param[in] value Float number.
  void set_value(double value) { value_ = value; }
  /// Get internal value.
  /// @returns float number.
  double value() const { return value_; }

 private:
  double value_;

  /// Copy constructor is disallowed.
  FloatValue(const FloatValue&);
  /// Assign operator is disallowed.
  FloatValue& operator=(const FloatValue&);
};

/// Value for booleans.
class BooleanValue : public Value {
 public:
  /// Constructor.
  /// @param[in] value Boolean to store.
  explicit BooleanValue(bool value);
  /// Default constructor.
  BooleanValue();
  /// Destructor.
  virtual ~BooleanValue();

  virtual bool IsBoolean() const override;
  virtual BooleanValue* AsBoolean() override;
  virtual const BooleanValue* AsBoolean() const override;

  virtual Value* DeepCopy() const override;

  /// Set internal value.
  /// @param[in] value Boolean.
  void set_value(bool value) { value_ = value; }
  /// Get internal value.
  /// @returns Boolean.
  bool value() const { return value_; }

 private:
  bool value_;

  /// Copy constructor is disallowed.
  BooleanValue(const BooleanValue&);
  /// Assign operator is disallowed.
  BooleanValue& operator=(const BooleanValue&);
};

/// Value for lists.
class ListValue : public Value {
 public:
  /// Constructor.
  ListValue();
  /// Destructor.
  virtual ~ListValue();

  virtual bool IsList() const override;
  virtual ListValue* AsList() override;
  virtual const ListValue* AsList() const override;

  virtual Value* DeepCopy() const override;

  /// Append value to the end of list.
  /// @param[in] value Value that must be appended to the list.
  void AddValue(Value* value);
  /// Size of list.
  /// @returns Size of list.
  size_t size() const { return vector_.size(); }
  /// Check that list is empty.
  /// @returns true if empty.
  bool empty() const { return vector_.empty(); }
  /// Get value at given index.
  /// @param[in] index Index of required value.
  /// @returns required value, if any.
  Value* GetValueAt(size_t index);
  /// Get constant value at given index.
  /// @param[in] index Index of required value.
  /// @returns constant required value, if any.
  const Value* GetValueAt(size_t index) const;
  /// Insert value at given index.
  /// @param[in] index Index where to insert.
  /// @param[in] value Value that must be inserted.
  void InsertValueAt(size_t index, Value* value);
  /// Removes value at given index.
  /// @param[in] index Index where to remove.
  void RemoveValueAt(size_t index);

 private:
  std::vector<Value*> vector_;

  /// Copy constructor is disallowed.
  ListValue(const ListValue&);
  /// Assign operator is disallowed.
  ListValue& operator=(const ListValue&);
};

/// Value for dictionary.
class DictionaryValue : public Value {
 public:
  /// Constructor.
  DictionaryValue();
  /// Destructor.
  virtual ~DictionaryValue();

  virtual bool IsDictionary() const override;
  virtual DictionaryValue* AsDictionary() override;
  virtual const DictionaryValue* AsDictionary() const override;

  virtual Value* DeepCopy() const override;

  /// Insert value at given key.
  /// @param[in] key   Key to insert at.
  /// @param[in] value Value to insert.
  void InsertValue(const std::string& key, Value* value);
  /// Remove value at given key.
  /// @param[in] key Key to remove at.
  void RemoveValue(const std::string& key);
  /// Get size of the dictionary.
  /// @returns Size of the dictionary.
  size_t size() const { return map_.size(); }
  /// Checks that dictionary is empty.
  /// @returns true if dictionary is empty.
  bool empty() const { return map_.empty(); }
  /// Get value at given key.
  /// @param[in] key Key of required value.
  /// @return Required value.
  Value* GetValue(const std::string& key);
  /// Get constant value at given key.
  /// @param[in] key Key of required value.
  /// @returns Constant required value.
  const Value* GetValue(const std::string& key) const;
  /// Get a pair of key/value at given index.
  /// @param[in] index Index of required pair.
  /// @returns Required pair of key/value.
  const std::pair<std::string, Value*> GetValueAt(size_t index) const;

 private:
  std::map<std::string, Value*> map_;

  /// Copy constructor is disallowed.
  DictionaryValue(const DictionaryValue&);
  /// Assign operator is disallowed.
  DictionaryValue& operator=(const DictionaryValue&);
};

/// Cast a value to the integer or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline IntegerValue* ToInteger(Value* value) {
  return value ? value->AsInteger() : nullptr;
}

/// Cast a value to the integer or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const IntegerValue* ToInteger(const Value* value) {
  return value ? value->AsInteger() : nullptr;
}

/// Cast a value to the boolean or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline BooleanValue* ToBoolean(Value* value) {
  return value ? value->AsBoolean() : nullptr;
}

/// Cast a value to the boolean or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const BooleanValue* ToBoolean(const Value* value) {
  return value ? value->AsBoolean() : nullptr;
}

/// Cast a value to the string or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline StringValue* ToString(Value* value) {
  return value ? value->AsString() : nullptr;
}

/// Cast a value to the string or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const StringValue* ToString(const Value* value) {
  return value ? value->AsString() : nullptr;
}

/// Cast a value to the float or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline FloatValue* ToFloat(Value* value) {
  return value ? value->AsFloat() : nullptr;
}

/// Cast a value to the float or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const FloatValue* ToFloat(const Value* value) {
  return value ? value->AsFloat() : nullptr;
}

/// Cast a value to the dictionary or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline DictionaryValue* ToDictionary(Value* value) {
  return value ? value->AsDictionary() : nullptr;
}

/// Cast a value to the dictionary or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const DictionaryValue* ToDictionary(const Value* value) {
  return value ? value->AsDictionary() : nullptr;
}

/// Cast a value to the list or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline ListValue* ToList(Value* value) {
  return value ? value->AsList() : nullptr;
}

/// Cast a value to the list or null.
/// @param[in] value Value that must be casted.
/// @returns a value or null.
inline const ListValue* ToList(const Value* value) {
  return value ? value->AsList() : nullptr;
}

}  // namespace base

#endif  // BASE_VALUES_H_
