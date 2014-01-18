// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_VALUES_H_
#define BASE_VALUES_H_

#include <utility>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace base {

class IntegerValue;
class StringValue;
class FloatValue;
class BooleanValue;
class ListValue;
class DictionaryValue;

class Value {
 public:
  virtual ~Value();

  virtual bool IsInteger() const;
  virtual bool IsString() const;
  virtual bool IsFloat() const;
  virtual bool IsBoolean() const;
  virtual bool IsList() const;
  virtual bool IsDictionary() const;

  virtual IntegerValue* AsInteger();
  virtual StringValue* AsString();
  virtual FloatValue* AsFloat();
  virtual BooleanValue* AsBoolean();
  virtual ListValue* AsList();
  virtual DictionaryValue* AsDictionary();

  virtual const IntegerValue* AsInteger() const;
  virtual const StringValue* AsString() const;
  virtual const FloatValue* AsFloat() const;
  virtual const BooleanValue* AsBoolean() const;
  virtual const ListValue* AsList() const;
  virtual const DictionaryValue* AsDictionary() const;

  virtual Value* DeepCopy() const = 0;
};

class IntegerValue : public Value {
 public:
  explicit IntegerValue(int64_t value);
  IntegerValue();
  virtual ~IntegerValue();

  virtual bool IsInteger() const override;
  virtual IntegerValue* AsInteger() override;
  virtual const IntegerValue* AsInteger() const override;

  virtual Value* DeepCopy() const override;

  void set_value(int64_t value) { value_ = value; }
  int64_t value() const { return value_; }

 private:
  int64_t value_;

  IntegerValue(const IntegerValue&);
  IntegerValue& operator=(const IntegerValue&);
};

class StringValue : public Value {
 public:
  explicit StringValue(const std::string& value);
  StringValue();
  virtual ~StringValue();

  virtual bool IsString() const override;
  virtual StringValue* AsString() override;
  virtual const StringValue* AsString() const override;

  virtual Value* DeepCopy() const override;

  void set_value(const std::string& value) { value_ = value; }
  std::string value() const { return value_; }

 private:
  std::string value_;

  StringValue(const StringValue&);
  StringValue& operator=(const StringValue&);
};

class FloatValue : public Value {
 public:
  explicit FloatValue(double value);
  FloatValue();
  virtual ~FloatValue();

  virtual bool IsFloat() const override;
  virtual FloatValue* AsFloat() override;
  virtual const FloatValue* AsFloat() const override;

  virtual Value* DeepCopy() const override;

  void set_value(double value) { value_ = value; }
  double value() const { return value_; }

 private:
  double value_;

  FloatValue(const FloatValue&);
  FloatValue& operator=(const FloatValue&);
};

class BooleanValue : public Value {
 public:
  explicit BooleanValue(bool value);
  BooleanValue();
  virtual ~BooleanValue();

  virtual bool IsBoolean() const override;
  virtual BooleanValue* AsBoolean() override;
  virtual const BooleanValue* AsBoolean() const override;

  virtual Value* DeepCopy() const override;

  void set_value(bool value) { value_ = value; }
  bool value() const { return value_; }

 private:
  bool value_;

  BooleanValue(const BooleanValue&);
  BooleanValue& operator=(const BooleanValue&);
};

class ListValue : public Value {
 public:
  ListValue();
  virtual ~ListValue();

  virtual bool IsList() const override;
  virtual ListValue* AsList() override;
  virtual const ListValue* AsList() const override;

  virtual Value* DeepCopy() const override;

  void AddValue(Value* value);
  size_t size() const { return vector_.size(); }
  bool empty() const { return vector_.empty(); }
  Value* GetValueAt(size_t index);
  const Value* GetValueAt(size_t index) const;
  void InsertValueAt(size_t index, Value* value);
  void RemoveValueAt(size_t index);

 private:
  std::vector<Value*> vector_;

  ListValue(const ListValue&);
  ListValue& operator=(const ListValue&);
};

class DictionaryValue : public Value {
 public:
  DictionaryValue();
  virtual ~DictionaryValue();

  virtual bool IsDictionary() const override;
  virtual DictionaryValue* AsDictionary() override;
  virtual const DictionaryValue* AsDictionary() const override;

  virtual Value* DeepCopy() const override;

  void InsertValue(const std::string& key, Value* value);
  void RemoveValue(const std::string& key);
  size_t size() const { return map_.size(); }
  bool empty() const { return map_.empty(); }
  Value* GetValue(const std::string& key);
  const Value* GetValue(const std::string& key) const;
  const std::pair<std::string, Value*> GetValueAt(size_t index) const;

 private:
  std::map<std::string, Value*> map_;

  DictionaryValue(const DictionaryValue&);
  DictionaryValue& operator=(const DictionaryValue&);
};

}  // namespace base

#endif  // BASE_VALUES_H_
