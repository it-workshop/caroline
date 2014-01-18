// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "gtest/gtest.h"

#include "base/json.h"
#include "base/values.h"

base::Value* GenerateTestData() {
  std::unique_ptr<base::DictionaryValue> dict(new base::DictionaryValue());

  dict->InsertValue("a", new base::IntegerValue(42));
  dict->InsertValue("b", new base::StringValue("foo"));
  dict->InsertValue("c", new base::FloatValue(0.0));
  dict->InsertValue("d", new base::BooleanValue(true));
  dict->InsertValue("e", new base::ListValue());
  return dict.release();
}

void CheckData(base::Value* parsed) {
  EXPECT_NE(nullptr, parsed);
  EXPECT_TRUE(parsed->IsDictionary());
  EXPECT_EQ(5, parsed->AsDictionary()->size());
#define check_item(key, type_check, type_cast, val) \
    EXPECT_NE(nullptr, parsed->AsDictionary()->GetValue(key)); \
    EXPECT_TRUE(parsed->AsDictionary()->GetValue(key)->type_check()); \
    EXPECT_EQ(val, parsed->AsDictionary()->GetValue(key)->type_cast()->value())

  check_item("a", IsInteger, AsInteger, 42);
  check_item("b", IsString, AsString, "foo");
  check_item("c", IsFloat, AsFloat, 0.0);
  check_item("d", IsBoolean, AsBoolean, true);
#undef check_item

  EXPECT_NE(nullptr, parsed->AsDictionary()->GetValue("e"));
  EXPECT_TRUE(parsed->AsDictionary()->GetValue("e")->IsList());
  EXPECT_TRUE(parsed->AsDictionary()->GetValue("e")->AsList()->empty());
}

TEST(JSONTest, SimpleTest) {
  std::unique_ptr<base::Value> generated(GenerateTestData());
  std::string json = base::json::Generate(generated.get());
  std::unique_ptr<base::Value> parsed(base::json::Parse(json));
  CheckData(parsed.get());
}

TEST(JSONTest, SingleQuotesTest) {
  base::json::Generator generator;
  generator.set_single_quote(true);
  generator.set_wrap_mode(base::json::Generator::WRAP_IF_NOT_SINGLE);
  generator.set_tab_size(2);
  generator.set_space_after_colon(true);
  std::unique_ptr<base::Value> generated(GenerateTestData());
  std::string json = generator.Generate(generated.get());
  base::json::Parser parser;
  parser.set_allow_single_quote(true);
  std::unique_ptr<base::Value> parsed(parser.Parse(json));
  CheckData(parsed.get());
}
