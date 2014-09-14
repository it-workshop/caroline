//// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#include <sys/param.h>

#include "base/path_service.h"
#include "gtest/gtest.h"

TEST(PathServicePosixTest, RootPath) {
  base::PathService* instance = base::PathService::GetInstance();
  EXPECT_EQ("/", instance->root_path().spec());
  EXPECT_EQ("file:///", instance->root_path().url());
}

TEST(PathServicePosixTest, WorkingDirectory) {
  base::PathService* instance = base::PathService::GetInstance();
  char buffer[MAXPATHLEN + 7] = "file://";
  EXPECT_TRUE(getcwd(buffer + 7, MAXPATHLEN));
  EXPECT_EQ(buffer + 7, instance->GetWorkingDirectoryPath().spec());
  EXPECT_EQ(buffer, instance->GetWorkingDirectoryPath().url());
}

TEST(PathServicePosixTest, MakePath) {
  base::PathService* instance = base::PathService::GetInstance();
  base::Path path = instance->MakePath("/tmp/../../b/./a.out");
  EXPECT_EQ("/b/a.out", path.spec());
  EXPECT_EQ("file:///b/a.out", path.url());
}
