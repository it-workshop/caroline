// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "gtest/gtest.h"

#include "base/stream.h"

TEST(StreamTest, Init) {
  std::unique_ptr<base::Stream> str =
      base::Stream::Open("127.0.0.1:3456", base::Stream::Mode::kRead);

  std::unique_ptr<base::Stream> str2 =
      base::Stream::Open("127.0.0.1:3456", base::Stream::Mode::kRead);
}
