// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/json.h"

#include <cstdlib>
#include <cmath>

#include "base/values.h"

namespace base {

namespace json {

namespace {

void SkipSpaces(const std::string& json, off_t* offset) {
  while (isspace(json[*offset]) && *offset < static_cast<off_t>(json.size()))
    (*offset)++;
}

int64_t ParseInteger(const std::string& json, off_t* offset) {
  int64_t ret = 0;
  bool positive = true;
  if (*offset >= static_cast<off_t>(json.size()))
    return 0;
  if ('-' == json[*offset]) {
    (*offset)++;
    positive = false;
  }
  while (*offset < static_cast<off_t>(json.size()) && isdigit(json[*offset]))
    ret = ret * 10 + (json[(*offset)++] - '0');
  return positive ? ret : -ret;
}

long double ParseFractional(const std::string& json, off_t* offset, int* exp) {
  *exp = 0;
  if (*offset >= static_cast<off_t>(json.size()))
    return 0.0;
  if (json[*offset] != '.')
    return 0.0;
  (*offset)++;
  long double ret = 0.0;
  int denominator = 10;
  while (*offset < static_cast<off_t>(json.size()) &&
      isdigit(json[*offset])) {
    long double numerator = static_cast<long double>(json[(*offset)++] - '0');
    ret += numerator / denominator;
    denominator *= 10;
  }
  if (*offset < static_cast<off_t>(json.size()) &&
      (json[*offset] == 'e' || json[*offset] == 'E')) {
    ++(*offset);
    if (*offset >= static_cast<off_t>(json.size()))
      return ret;
    bool negative_exp = false;
    if (json[*offset] == '-')
      negative_exp = true;
    if (json[*offset] == '-' || json[*offset] == '+')
      ++(*offset);
    while (*offset < static_cast<off_t>(json.size()) &&
      isdigit(json[*offset])) {
      *exp *= 10;
      *exp += json[*offset] - '0';
      ++(*offset);
    }
    if (negative_exp)
      *exp = -*exp;
  }
  return ret;
}

BooleanValue* ParseBoolean(const std::string& json, off_t *offset) {
  if (json.find("true", *offset) == *offset) {
    (*offset) += 4;
    return new BooleanValue(true);
  } else if (json.find("false", *offset) == *offset) {
    (*offset) += 5;
    return new BooleanValue(false);
  }
  return nullptr;
}

}  // namespace

Generator::Generator()
  : tab_size_(0),
    wrap_mode_(WRAP_NONE),
    space_after_colon_(false),
    single_quote_(false),
    trailing_comma_(false) {}

Generator::~Generator() {}

std::string Generator::Generate(const Value* value) const {
  return GenerateImpl(value, 0);
}

std::string Generator::GenerateImpl(const Value* value,
    unsigned int level) const {
  const char* quote = single_quote_ ? "'" : "\"";
  const char* newline = wrap_mode_ != WRAP_NONE ? "\n" : "";
  const std::string indentation(
      wrap_mode_ != WRAP_NONE ? tab_size_ * level : 0, ' ');

  if (value->IsInteger())
    return std::to_string(value->AsInteger()->value());

  if (value->IsString())
    return std::string(quote).append(PrepareString(value->AsString()->value())).
        append(quote);

  if (value->IsFloat())
    return std::to_string(value->AsFloat()->value());

  if (value->IsBoolean())
    return value->AsBoolean()->value() ? "true" : "false";

  if (value->IsList()) {
    const ListValue* list = value->AsList();
    std::string ret("[");

    std::string separator(",");
    std::string last_separator(trailing_comma_  && !list->empty() ? "," : "");
    if ((wrap_mode_ == WRAP_ALL && !list->empty()) ||
        (wrap_mode_ == WRAP_IF_NOT_SINGLE && list->size() > 1 &&
            !list->GetValueAt(0)->IsList() &&
            !list->GetValueAt(0)->IsDictionary())) {
      separator.append(newline).append(tab_size_ * (level + 1), ' ');
      last_separator.append(newline).append(indentation);
    }

    for (unsigned int i = 0; i < list->size(); ++i)
      ret.append(i != 0 ? "," : "").append(separator).
          append(GenerateImpl(list->GetValueAt(i), level + 1));

    return ret.append(last_separator).append("]");
  }

  if (value->IsDictionary()) {
    const DictionaryValue* dict = value->AsDictionary();
    std::string ret("{");

    std::string separator(",");
    std::string last_separator(trailing_comma_ && !dict->empty() ? "," : "");
    if ((wrap_mode_ == WRAP_ALL && !dict->empty()) ||
        (wrap_mode_ == WRAP_IF_NOT_SINGLE && dict->size() > 1 &&
            !dict->GetValueAt(0).second->IsList() &&
            !dict->GetValueAt(0).second->IsDictionary())) {
      separator.append(newline).append(tab_size_ * (level + 1), ' ');
      last_separator.append(newline).append(indentation);
    }

    for (unsigned int i = 0; i < dict->size(); ++i) {
      auto pair = dict->GetValueAt(i);
      ret.append(i != 0 ? separator : "").
          append(quote).append(pair.first).append(quote).append(":").
          append(space_after_colon_ ? " " : "").
          append(GenerateImpl(pair.second, level + 1));
    }

    return ret.append(last_separator).append("}");
  }

  // Not reached.
  abort();
}


std::string Generator::PrepareString(const std::string& string) const {
  std::string ret;
  for (char c : string) {
    switch (c) {
      case '"':
        if (single_quote_)
          ret.push_back(c);
        else
          ret.append("\\\"");
        break;
      case '\'':
        if (single_quote_)
          ret.append("\\'");
        else
          ret.push_back(c);
        break;
      case '\\': ret.append("\\\\"); break;
      case '/': ret.append("\\/"); break;
      case '\b': ret.append("\\b"); break;
      case '\f': ret.append("\\f"); break;
      case '\n': ret.append("\\n"); break;
      case '\t': ret.append("\\t"); break;
      case '\r': ret.append("\\r"); break;
      default: ret.push_back(c); break;
    }
  }
  return ret;
}

std::string Generate(const Value* value) {
  return Generator().Generate(value);
}

// Parser
Parser::Parser()
  : allow_single_quote_(false),
    allow_trailing_comma_(false) {}

Parser::~Parser() {}

Value* Parser::Parse(const std::string& json) const {
  off_t offset = 0;
  return ParseImpl(json, &offset);
}

Parser::ValueType Parser::WhatIsStarts(char c) const {
  switch (c) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '-':
      return INTEGER;
    case '"':
      return STRING;
    case '\'':
      return allow_single_quote_ ? STRING : ERROR;
    case '.':
      return FLOAT;
    case 't':
    case 'f':
      return BOOLEAN;
    case '[':
      return LIST;
    case '{':
      return DICTIONARY;
  }
  return ERROR;
}

base::Value* Parser::ParseImpl(const std::string& json, off_t* offset) const {
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;

  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  std::unique_ptr<Value> value;
  switch (WhatIsStarts(json[*offset])) {
    case INTEGER: {
      int64_t integral = ParseInteger(json, offset);
      if (static_cast<off_t>(json.size()) > *offset && json[*offset] != '.') {
        value.reset(new IntegerValue(integral));
        break;
      }
      int exp = 0;
      long double fractional = ParseFractional(json, offset, &exp);
      fractional = pow(10, exp) *
          (integral < 0 ? integral - fractional : integral + fractional);
      value.reset(new FloatValue(integral < 0 ?
          integral - fractional : integral + fractional));
      break;
    }
    case FLOAT: {
      // WhatIsStarts() can return float only if it written as fractional part.
      int exp = 0;
      long double fractional = ParseFractional(json, offset, &exp);
      value.reset(new FloatValue(fractional * pow(10, exp)));
      break;
    }
    case STRING:
      value.reset(ParseString(json, offset));
      break;
    case BOOLEAN:
      value.reset(ParseBoolean(json, offset));
      break;
    case LIST:
      value.reset(ParseList(json, offset));
      break;
    case DICTIONARY:
      value.reset(ParseDictionary(json, offset));
      break;
    case ERROR:
      break;
  }
  return value.release();
}

StringValue* Parser::ParseString(const std::string& json, off_t* offset) const {
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  char quote = json[*offset];
  if (quote != '"' && allow_single_quote_ == (quote != '\''))
    return nullptr;
  (*offset)++;
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  std::string ret;
  bool escape = false;
  bool quote_meet = false;
  while (!quote_meet && static_cast<off_t>(json.size()) > *offset) {
    char c = json[*offset];
    if (escape) {
      switch (c) {
        case 'b':
          ret.push_back('\b');
          break;
        case 'f':
          ret.push_back('\f');
          break;
        case 'n':
          ret.push_back('\n');
          break;
        case 't':
          ret.push_back('\t');
          break;
        case 'r':
          ret.push_back('\r');
          break;
        default:
          ret.push_back(c);
          break;
      }
    } else {
      if ('\\' == c)
        escape = true;
      else if (c == quote)
        quote_meet = true;
      else
        ret.push_back(c);
    }
    (*offset)++;
  }
  if (!quote_meet)
    return nullptr;
  return new StringValue(ret);
}

ListValue* Parser::ParseList(const std::string& json, off_t* offset) const {
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] != '[')
    return nullptr;
  (*offset)++;

  std::unique_ptr<ListValue> list(new ListValue());
  std::unique_ptr<Value> item;

check_end:
  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] == ']') {
    (*offset)++;
    return list.release();
  }

parse_item:
  SkipSpaces(json, offset);
  item.reset(ParseImpl(json, offset));
  if (!item)
    return nullptr;
  list->AddValue(item.release());

  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] == ',') {
    (*offset)++;
    if (allow_trailing_comma_)
      goto check_end;
    else
      goto parse_item;
  }
  if (json[*offset] != ']')
    return nullptr;
  return list.release();
}

DictionaryValue* Parser::ParseDictionary(const std::string& json,
    off_t* offset) const {
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] != '{')
    return nullptr;
  (*offset)++;

  std::unique_ptr<DictionaryValue> dict(new DictionaryValue());
  std::unique_ptr<StringValue> key;
  std::unique_ptr<Value> item;

check_end:
  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] == '}') {
    (*offset)++;
    return dict.release();
  }

parse_item:
  SkipSpaces(json, offset);
  key.reset(ParseString(json, offset));
  if (!key)
    return nullptr;
  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (':' != json[*offset])
    return nullptr;
  (*offset)++;

  SkipSpaces(json, offset);
  item.reset(ParseImpl(json, offset));
  if (!item)
    return nullptr;
  dict->InsertValue(key->value(), item.release());

  SkipSpaces(json, offset);
  if (static_cast<off_t>(json.size()) < *offset)
    return nullptr;
  if (json[*offset] == ',') {
    (*offset)++;
    if (allow_trailing_comma_)
      goto check_end;
    else
      goto parse_item;
  }
  if (json[*offset] != '}')
    return nullptr;
  (*offset)++;
  return dict.release();
}

base::Value* Parse(const std::string& json) {
  return Parser().Parse(json);
}

}  // namespace json

}  // namespace base
