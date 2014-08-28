// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/at_exit.h"
#include "base/path_service.h"
#include "gtest/gtest.h"

int main(int argc, char* argv[]) {
  base::AtExitManager at_exit_manager;
  base::PathService::Init(*argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
