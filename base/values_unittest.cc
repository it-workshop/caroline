// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#include <algorithm>
#include <memory>
#include <utility>

#include "gtest/gtest.h"

#include "base/values.h"

TEST(ValuesTest, IntegerValueTest) {
  std::unique_ptr<base::Value> value(new base::IntegerValue(42));
  EXPECT_TRUE(value->IsInteger());
  EXPECT_EQ(42, value->AsInteger()->value());
}

TEST(ValuesTest, StringValueTest) {
  std::unique_ptr<base::Value> value(new base::StringValue("abc"));
  EXPECT_TRUE(value->IsString());
  EXPECT_EQ("abc", value->AsString()->value());
}

TEST(ValuesTest, FloatValueTest) {
  std::unique_ptr<base::Value> value(new base::FloatValue(0.0));
  EXPECT_TRUE(value->IsFloat());
  EXPECT_EQ(0.0, value->AsFloat()->value());
}

TEST(ValueTest, BooleanValueTest) {
  std::unique_ptr<base::Value> value(new base::BooleanValue(true));
  EXPECT_TRUE(value->IsBoolean());
  EXPECT_TRUE(value->AsBoolean()->value());
  value->AsBoolean()->set_value(false);
  EXPECT_FALSE(value->AsBoolean()->value());
}

TEST(ValuesTest, ListValueTest) {
  std::unique_ptr<base::Value> list(new base::ListValue());
  EXPECT_TRUE(list->IsList());
  EXPECT_EQ(0, list->AsList()->size());
  for (int i = 0; i < 10; ++i)
    list->AsList()->AddValue(new base::IntegerValue(i));
  EXPECT_EQ(10, list->AsList()->size());
  for (int i = 0; i < 10; ++i) {
    EXPECT_NE(nullptr, list->AsList()->GetValueAt(i));
    EXPECT_TRUE(list->AsList()->GetValueAt(i)->IsInteger());
    EXPECT_EQ(i, list->AsList()->GetValueAt(i)->AsInteger()->value());
  }
}

TEST(ValuesTest, DictionaryValueTest) {
  std::unique_ptr<base::Value> dictionary(new base::DictionaryValue());
  EXPECT_TRUE(dictionary->IsDictionary());
  EXPECT_EQ(0, dictionary->AsDictionary()->size());
  for (int i = 0; i < 10; ++i) {
    std::ostringstream stream;
    stream << i;
    dictionary->AsDictionary()->InsertValue(stream.str(),
        new base::IntegerValue(i));
  }
  EXPECT_EQ(10, dictionary->AsDictionary()->size());
  for (int i = 0; i < 10; ++i) {
    std::ostringstream stream;
    stream << i;
    EXPECT_NE(nullptr, dictionary->AsDictionary()->GetValue(stream.str()));
    EXPECT_TRUE(dictionary->AsDictionary()->GetValue(stream.str())->
        IsInteger());
    EXPECT_EQ(i, dictionary->AsDictionary()->GetValue(stream.str())->
        AsInteger()->value());
    std::pair<std::string, base::Value*> pair =
        dictionary->AsDictionary()->GetValueAt(i);
    EXPECT_EQ(stream.str(), pair.first);
    EXPECT_NE(nullptr, pair.second);
    EXPECT_TRUE(pair.second->IsInteger());
    EXPECT_EQ(i, pair.second->AsInteger()->value());
  }
}

TEST(ValuesTest, DeepCopyTest) {
  std::unique_ptr<base::Value> value;
  {
    value.reset(new base::DictionaryValue());
    base::DictionaryValue* dict = value->AsDictionary();
    dict->InsertValue("int", new base::IntegerValue(42));
    dict->InsertValue("string", new base::StringValue("abc"));
    dict->InsertValue("float", new base::FloatValue(0.0));
    {
      base::ListValue* list = new base::ListValue();
      for (int i = 0; i < 10; ++i)
        list->AddValue(new base::IntegerValue(i));
      dict->InsertValue("list", list);
    }
  }
  std::unique_ptr<base::Value> copy(value->DeepCopy());
  {
    EXPECT_TRUE(value->IsDictionary());
    base::DictionaryValue* dict = value->AsDictionary();
    EXPECT_EQ(4, dict->size());

#define check_item(key, type_check, type_cast, val) \
    EXPECT_NE(nullptr, dict->GetValue(key)); \
    EXPECT_TRUE(dict->GetValue(key)->type_check()); \
    EXPECT_EQ(val, dict->GetValue(key)->type_cast()->value())

    check_item("int", IsInteger, AsInteger, 42);
    check_item("string", IsString, AsString, "abc");
    check_item("float", IsFloat, AsFloat, 0.0);
#undef check_item

    EXPECT_NE(nullptr, dict->GetValue("list"));
    EXPECT_TRUE(dict->GetValue("list")->IsList());
    {
      base::ListValue* list = dict->GetValue("list")->AsList();
      EXPECT_EQ(10, list->size());
      for (int i = 0; i < 10; ++i) {
        EXPECT_NE(nullptr, list->GetValueAt(i));
        EXPECT_TRUE(list->GetValueAt(i)->IsInteger());
        EXPECT_EQ(i, list->GetValueAt(i)->AsInteger()->value());
      }
    }
  }
}
