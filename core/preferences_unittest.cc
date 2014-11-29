// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "core/preferences.h"

TEST(PreferencesTest, InitTest) {
  core::Preferences prefs;

  EXPECT_TRUE(prefs.Add("length", Json::Value(13)));
  EXPECT_EQ(prefs.Get("length")->asInt(), 13);
}

TEST(PreferencesTest, AddsTest) {
  core::Preferences prefs;

  EXPECT_TRUE(prefs.Add("lengths", Json::Value(Json::objectValue)));
  EXPECT_TRUE(prefs.Add("lengths", "another", Json::Value(Json::objectValue)));
  EXPECT_TRUE(prefs.Add("lengths.another", "l1", Json::Value(14)));
  EXPECT_TRUE(prefs.Add("lengths.another", "l2", Json::Value(12)));
  EXPECT_TRUE(prefs.Get("lengths.another.l1"));
  EXPECT_EQ(prefs.Get("lengths.another.l1")->asInt(), 14);
}

TEST(PreferencesTest, MembersTest) {
  core::Preferences prefs;

  EXPECT_TRUE(prefs.Add("lengths", Json::Value(Json::objectValue)));
  EXPECT_TRUE(prefs.Add("lengths", "another", Json::Value(Json::objectValue)));
  EXPECT_TRUE(prefs.Add("lengths.another", "l1", Json::Value(14)));
  EXPECT_TRUE(prefs.Add("lengths.another", "l2", Json::Value(12)));
  EXPECT_TRUE(prefs.Add("lengths", "l3", Json::Value(42)));
  EXPECT_TRUE(prefs.Add("", "l4", Json::Value(22)));
  EXPECT_TRUE(prefs.Get("lengths.another.l1"));
  EXPECT_TRUE(prefs.Get("lengths.l3"));
  EXPECT_EQ(prefs.Get("lengths.another.l1")->asInt(), 14);
  EXPECT_EQ(prefs.Get("lengths.l3")->asInt(), 42);

  Json::Value::Members members = prefs.AtomicMembers();
  EXPECT_EQ(members.size(), 4);
  EXPECT_TRUE(std::find(members.begin(),
    members.end(), "lengths.l3") != members.end());
  EXPECT_TRUE(std::find(members.begin(), members.end(), "l4") != members.end());
}

