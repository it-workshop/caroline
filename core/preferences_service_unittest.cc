// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <cstdio>
#include <string>

#include "gtest/gtest.h"

#include "core/preferences_service.h"

TEST(PrefServiceTest, InitTest) {
  base::AtExitManager at_exit_manager;
  core::PrefService::Init();
  core::PrefService* pref = core::PrefService::GetInstance();

  EXPECT_TRUE(pref->RegisterInt("width", 640));
  EXPECT_TRUE(pref->RegisterInt("height", 480));
  EXPECT_EQ(pref->GetInt("width"), 640);
}

TEST(PrefServiceTest, SetAndUpdateTest) {
  base::AtExitManager at_exit_manager;
  core::PrefService::Init();
  core::PrefService* pref = core::PrefService::GetInstance();

  EXPECT_TRUE(pref->RegisterInt("width", 640));
  EXPECT_TRUE(pref->RegisterInt("height", 480));
  EXPECT_EQ(pref->GetInt("width"), 640);
  EXPECT_TRUE(pref->SetInt("width", 1024));
  EXPECT_EQ(pref->GetInt("width"), 1024);
}

TEST(PrefServiceTest, DictTest) {
  base::AtExitManager at_exit_manager;
  core::PrefService::Init();
  core::PrefService* pref = core::PrefService::GetInstance();

  EXPECT_TRUE(pref->RegisterDict("params"));
  EXPECT_TRUE(pref->RegisterInt("params.width", 640));
  EXPECT_TRUE(pref->RegisterInt("params.height", 480));
  EXPECT_TRUE(pref->RegisterDict("other_params"));
  EXPECT_TRUE(pref->RegisterFloat("other_params.type", 33.3));
  EXPECT_TRUE(pref->RegisterString(
    "other_params.algoname", std::string("coolss")));
  EXPECT_TRUE(pref->RegisterBool("other_params.isok", true));
  EXPECT_FALSE(pref->RegisterFloat("foofoo.barbar", 33.3));
  EXPECT_TRUE(pref->IsRegistered("params"));
  EXPECT_EQ(pref->GetInt("params.width"), 640);
  EXPECT_TRUE(pref->SetInt("params.width", 1024));
  EXPECT_EQ(pref->GetInt("params.width"), 1024);
}

TEST(PrefServiceTest, WriteTest) {
  base::AtExitManager at_exit_manager;
  core::PrefService::Init();
  core::PrefService* pref = core::PrefService::GetInstance();

  EXPECT_TRUE(pref->RegisterDict("params"));
  EXPECT_TRUE(pref->RegisterInt("params.width", 640));
  EXPECT_TRUE(pref->RegisterInt("params.height", 480));
  EXPECT_TRUE(pref->RegisterDict("other_params"));
  EXPECT_TRUE(pref->RegisterFloat("other_params.type", 33.3));
  EXPECT_TRUE(pref->RegisterString(
    "other_params.algoname", std::string("goodalgo")));
  EXPECT_TRUE(pref->RegisterBool("other_params.isok", true));
  EXPECT_FALSE(pref->RegisterFloat("foofoo.barbar", 33.3));
  EXPECT_TRUE(pref->IsRegistered("params"));
  EXPECT_EQ(pref->GetInt("params.width"), 640);
  EXPECT_TRUE(pref->SetInt("params.width", 1024));
  EXPECT_EQ(pref->GetInt("params.width"), 1024);

  EXPECT_TRUE(pref->WriteToConfig("test.config"));
}

TEST(PrefServiceTest, LoadTest) {
  base::AtExitManager at_exit_manager;
  core::PrefService::Init();
  core::PrefService* pref = core::PrefService::GetInstance();

  EXPECT_TRUE(pref->RegisterDict("params"));
  EXPECT_TRUE(pref->Register(
    core::PrefService::PrefType::FLOAT, "params.height"));
  EXPECT_TRUE(pref->RegisterDict("other_params"));
  EXPECT_TRUE(pref->RegisterString(
    "other_params.algoname", std::string("badalgo")));
  EXPECT_TRUE(pref->RegisterBool("other_params.isok", false));
  EXPECT_TRUE(pref->RegisterInt("length", 13));

  EXPECT_TRUE(pref->LoadFromConfig("test.config"));
  EXPECT_EQ(pref->GetString("other_params.algoname"), "goodalgo");
  EXPECT_EQ(pref->GetBool("other_params.isok"), true);

  EXPECT_EQ(std::remove("test.config"), 0);
}

