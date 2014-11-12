// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <cstdio>
#include <string>

#include "gtest/gtest.h"

#include "core/preferences_service.h"

TEST(PrefServiceTest, InitTest) {
  core::PrefService pref;

  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "width", 640));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "height", 480));
  EXPECT_EQ(pref.GetInt("width"), 640);
}

TEST(PrefServiceTest, SetAndUpdateTest) {
  core::PrefService pref;

  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "width", 640));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "height", 480));
  EXPECT_EQ(pref.GetInt("width"), 640);
  EXPECT_TRUE(pref.SetInt("width", 1024));
  EXPECT_EQ(pref.GetInt("width"), 1024);
}

TEST(PrefServiceTest, DictTest) {
  core::PrefService pref;

  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "params.width", 640));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "params.height", 480));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "other_params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::FLOAT, "other_params.type", 33.3));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::STRING,
    "other_params.algoname", std::string("coolss")));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::BOOLEAN, "other_params.isok", true));
  EXPECT_FALSE(pref.Register(
    core::PrefService::PrefType::FLOAT, "foofoo.barbar", 33.3));
  EXPECT_TRUE(pref.IsRegistered("params"));
  EXPECT_EQ(pref.GetInt("params.width"), 640);
  EXPECT_TRUE(pref.SetInt("params.width", 1024));
  EXPECT_EQ(pref.GetInt("params.width"), 1024);
}

TEST(PrefServiceTest, WriteTest) {
  core::PrefService pref;

  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "params.width", 640));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "params.height", 480));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "other_params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::FLOAT, "other_params.type", 33.3));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::STRING,
    "other_params.algoname", std::string("goodalgo")));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::BOOLEAN, "other_params.isok", true));
  EXPECT_FALSE(pref.Register(
    core::PrefService::PrefType::FLOAT, "foofoo.barbar", 33.3));
  EXPECT_TRUE(pref.IsRegistered("params"));
  EXPECT_EQ(pref.GetInt("params.width"), 640);
  EXPECT_TRUE(pref.SetInt("params.width", 1024));
  EXPECT_EQ(pref.GetInt("params.width"), 1024);

  EXPECT_TRUE(pref.WriteToConfig("test.config"));
}

TEST(PrefServiceTest, LoadTest) {
  core::PrefService pref;

  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::FLOAT, "params.height"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::DICTIONARY, "other_params"));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::STRING,
    "other_params.algoname", std::string("badalgo")));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::BOOLEAN, "other_params.isok", false));
  EXPECT_TRUE(pref.Register(
    core::PrefService::PrefType::INTEGER, "length", 13));

  EXPECT_TRUE(pref.LoadFromConfig("test.config"));
  EXPECT_EQ(pref.GetString("other_params.algoname"), "goodalgo");
  EXPECT_EQ(pref.GetBool("other_params.isok"), true);

  EXPECT_EQ(std::remove("test.config"), 0);
}

