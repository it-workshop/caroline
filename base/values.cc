// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <base/values.h>

namespace base {

// class Value
Value::~Value() {}

bool Value::IsInteger() const { return false; }
bool Value::IsString() const { return false; }
bool Value::IsFloat() const { return false; }
bool Value::IsBoolean() const { return false; }
bool Value::IsList() const { return false; }
bool Value::IsDictionary() const { return false; }

IntegerValue* Value::AsInteger() { return nullptr; }
StringValue* Value::AsString() { return nullptr; }
FloatValue* Value::AsFloat() { return nullptr; }
BooleanValue* Value::AsBoolean() { return nullptr; }
ListValue* Value::AsList() { return nullptr; }
DictionaryValue* Value::AsDictionary() { return nullptr; }

const IntegerValue* Value::AsInteger() const { return nullptr; }
const StringValue* Value::AsString() const { return nullptr; }
const FloatValue* Value::AsFloat() const { return nullptr; }
const BooleanValue* Value::AsBoolean() const { return nullptr; }
const ListValue* Value::AsList() const { return nullptr; }
const DictionaryValue* Value::AsDictionary() const { return nullptr; }

// class IntegerValue
IntegerValue::IntegerValue(int64_t value)
  : value_(value) {}

IntegerValue::IntegerValue()
  : value_(0) {}

IntegerValue::~IntegerValue() {}

bool IntegerValue::IsInteger() const { return true; }
IntegerValue* IntegerValue::AsInteger() { return this; }
const IntegerValue* IntegerValue::AsInteger() const { return this; }

Value* IntegerValue::DeepCopy() const { return new IntegerValue(value_); }

// class StringValue
StringValue::StringValue(const std::string& value)
  : value_(value) {}

StringValue::StringValue() {}

StringValue::~StringValue() {}

bool StringValue::IsString() const { return true; }
StringValue* StringValue::AsString() { return this; }
const StringValue* StringValue::AsString() const { return this; }

Value* StringValue::DeepCopy() const { return new StringValue(value_); }

// class FloatValue
FloatValue::FloatValue(double value)
  : value_(value) {}

FloatValue::FloatValue()
  : value_(0.0) {}

FloatValue::~FloatValue() {}

bool FloatValue::IsFloat() const { return true; }
FloatValue* FloatValue::AsFloat() { return this; }
const FloatValue* FloatValue::AsFloat() const { return this; }

Value* FloatValue::DeepCopy() const { return new FloatValue(value_); }

// class BooleanValue
BooleanValue::BooleanValue(bool value)
  : value_(value) {}

BooleanValue::BooleanValue()
  : value_(false) {}

BooleanValue::~BooleanValue() {}

bool BooleanValue::IsBoolean() const { return true; }
BooleanValue* BooleanValue::AsBoolean() { return this; }
const BooleanValue* BooleanValue::AsBoolean() const { return this; }

Value* BooleanValue::DeepCopy() const { return new BooleanValue(value_); }

// class ListValue
ListValue::ListValue() {}

ListValue::~ListValue() {
  decltype(vector_) vector;
  std::swap(vector, vector_);
  for (auto value : vector)
    delete value;
}

bool ListValue::IsList() const { return true; }
ListValue* ListValue::AsList() { return this; }
const ListValue* ListValue::AsList() const { return this; }

Value* ListValue::DeepCopy() const {
  std::unique_ptr<ListValue> list(new ListValue());
  if (!list)
    return nullptr;
  for (auto value : vector_)
    list->AddValue(value->DeepCopy());
  return list.release();
}

void ListValue::AddValue(Value* value) {
  vector_.push_back(value);
}

Value* ListValue::GetValueAt(size_t index) {
  return index < vector_.size() ? vector_.at(index) : nullptr;
}

const Value* ListValue::GetValueAt(size_t index) const {
  return index < vector_.size() ? vector_.at(index) : nullptr;
}

void ListValue::InsertValueAt(size_t index, Value* value) {
  vector_.insert(index < vector_.size() ?
      vector_.begin() + index : vector_.end(), value);
}

void ListValue::RemoveValueAt(size_t index) {
  if (index < vector_.size())
    vector_.erase(vector_.begin() + index);
}

// class DictionaryValue
DictionaryValue::DictionaryValue() {}

DictionaryValue::~DictionaryValue() {
  decltype(map_) map;
  std::swap(map, map_);
  for (auto pair : map)
    delete pair.second;
}

bool DictionaryValue::IsDictionary() const { return true; }
DictionaryValue* DictionaryValue::AsDictionary() { return this; }
const DictionaryValue* DictionaryValue::AsDictionary() const { return this; }

Value* DictionaryValue::DeepCopy() const {
  std::unique_ptr<DictionaryValue> dictionary(new DictionaryValue());
  if (!dictionary)
    return nullptr;
  for (auto pair : map_)
    dictionary->InsertValue(pair.first, pair.second->DeepCopy());
  return dictionary.release();
}

void DictionaryValue::InsertValue(const std::string& key, Value* value) {
  auto it = map_.find(key);
  if (it != map_.end()) {
    auto previous = it->second;
    it->second = value;
    delete previous;
  } else {
    map_.insert(std::make_pair(key, value));
  }
}

void DictionaryValue::RemoveValue(const std::string& key) {
  auto it = map_.find(key);
  if (it != map_.end()) {
    auto value = it->second;
    map_.erase(it);
    delete value;
  }
}

Value* DictionaryValue::GetValue(const std::string& key) {
  auto it = map_.find(key);
  if (it == map_.end())
    return nullptr;
  return it->second;
}

const Value* DictionaryValue::GetValue(const std::string& key) const {
  auto it = map_.find(key);
  if (it == map_.end())
    return nullptr;
  return it->second;
}

const std::pair<std::string, Value*>
    DictionaryValue::GetValueAt(size_t index) const {
  if (index > map_.size())
    return std::make_pair(std::string(), static_cast<Value*>(nullptr));
  auto it = map_.begin();
  while (index --> 0)
    it++;
  return *it;
}

}  // namespace base
