// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "core/preferences.h"

TEST(PreferencesTest, InitTest) {
  core::Preferences prefs;

  ASSERT_TRUE(prefs.Add("length", Json::Value(13)));
  ASSERT_EQ(prefs.Get("length")->asInt(), 13);
}

TEST(PreferencesTest, AddsTest) {
  core::Preferences prefs;

  ASSERT_TRUE(prefs.Add("lengths", Json::Value(Json::objectValue)));
  ASSERT_TRUE(prefs.Add("lengths", "another", Json::Value(Json::objectValue)));
  ASSERT_TRUE(prefs.Add("lengths.another", "l1", Json::Value(14)));
  ASSERT_TRUE(prefs.Add("lengths.another", "l2", Json::Value(12)));
  ASSERT_TRUE(prefs.Get("lengths.another.l1"));
  ASSERT_EQ(prefs.Get("lengths.another.l1")->asInt(), 14);
}
