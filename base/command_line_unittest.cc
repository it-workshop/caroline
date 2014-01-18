// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include <algorithm>

#include "gtest/gtest.h"

#include "base/command_line.h"

namespace {

const char kArgOne[] = "--one";
const char kSwitchOne[] = "one";
const char kArgTwo[] = "--two";
const char kSwitchTwo[] = "two";
const char kArgThree[] = "--three";
const char kSwitchThree[] = "three";
const char kArgThreeData[] = "four";
const char kArgEmpty[] = "--";
const char kSwitchEmpty[] = "";
const char kArgData[] = "data";
const char kSwitchTest[] = "test";

}  // namespace

TEST(CommandLineTest, ArgvConstructorTest) {
  const char* argv[] = {
    kArgOne,
    kArgTwo,
    kArgThree, kArgThreeData,
    kArgEmpty, kArgData,
    nullptr
  };

  base::CommandLine command_line(argv);
  EXPECT_TRUE(command_line.HasSwitch(kSwitchOne));
  EXPECT_TRUE(command_line.HasSwitch(kSwitchTwo));
  EXPECT_TRUE(command_line.HasSwitch(kSwitchThree));
  EXPECT_EQ(kArgThreeData, command_line.GetSwitchData(kSwitchThree));
  EXPECT_TRUE(command_line.HasSwitch(kSwitchEmpty));
  EXPECT_EQ(kArgData, command_line.GetSwitchData(kSwitchEmpty));
}

TEST(CommandLineTest, AddRemoveSwitchTest) {
  base::CommandLine command_line;
  command_line.AddSwitch(kSwitchTest);
  EXPECT_TRUE(command_line.HasSwitch(kSwitchTest));
  command_line.RemoveSwitch(kSwitchTest);
  EXPECT_FALSE(command_line.HasSwitch(kSwitchTest));
}

TEST(CommandLineTest, SetDataTest) {
  base::CommandLine command_line;
  command_line.AddSwitchWithData(kSwitchTest, kArgData);
  EXPECT_TRUE(command_line.HasSwitch(kSwitchTest));
  EXPECT_EQ(kArgData, command_line.GetSwitchData(kSwitchTest));
}

TEST(CommandLineTest, CopyObjectTest) {
  base::CommandLine command_line;
  command_line.AddSwitch(kSwitchOne);
  command_line.AddSwitch(kSwitchTwo);
  command_line.AddSwitch(kSwitchThree);
  {
    base::CommandLine copy(command_line);
    EXPECT_TRUE(copy.HasSwitch(kSwitchOne));
    EXPECT_TRUE(copy.HasSwitch(kSwitchTwo));
    EXPECT_TRUE(copy.HasSwitch(kSwitchThree));
  }
  {
    base::CommandLine copy;
    copy = command_line;
    EXPECT_TRUE(command_line.HasSwitch(kSwitchOne));
    EXPECT_TRUE(command_line.HasSwitch(kSwitchTwo));
    EXPECT_TRUE(command_line.HasSwitch(kSwitchThree));
  }
}

TEST(CommandLineTest, CurrentProcessTest) {
  {
    // Fill command line object and release shared pointer.
    std::shared_ptr<base::CommandLine> command_line =
        base::CommandLine::GetForCurrentProcess();
    command_line->AddSwitch(kSwitchOne);
    command_line->AddSwitch(kSwitchTwo);
    command_line->AddSwitch(kSwitchThree);
  }
  {
    // Moke sure that there is the same command line object.
    std::shared_ptr<base::CommandLine> command_line =
        base::CommandLine::GetForCurrentProcess();
    EXPECT_TRUE(command_line->HasSwitch(kSwitchOne));
    EXPECT_TRUE(command_line->HasSwitch(kSwitchTwo));
    EXPECT_TRUE(command_line->HasSwitch(kSwitchThree));
  }
}
