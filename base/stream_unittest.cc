// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Konstantin Bormotov <bormotovk@gmail.com>

#include "gtest/gtest.h"
#include "base/stream.h"
#include "base/logging.h"

TEST (StreamTest, TCPOpenTest) {
  base::Logger::GetInstance()->Init(std::string(), base::Logger::Level::LOG_ERR);
  std::unique_ptr<base::Stream> stream = base::Stream::Open("tcp-bind://127.0.0.1:3452", base::Stream::kReadWrite);
  std::unique_ptr<base::Stream> stream2 = base::Stream::Open("tcp://127.0.0.1:3452", base::Stream::kReadWrite);

  std::string sent_message("Hello, 'm test message");
  std::unique_ptr<char[]> recieve_buffer(new char[sent_message.size()+1]);
  size_t transmitted = 0;
  do {
    transmitted += stream2->Write(sent_message.c_str() + transmitted, sent_message.size() - transmitted);
  } while (transmitted != sent_message.size());

  size_t received = 0;
  do {
    received += stream->Read(recieve_buffer.get() + received, sent_message.size() - received);
  } while (received != sent_message.size());
  recieve_buffer[received] = '\0';

  EXPECT_STREQ(sent_message.c_str(), recieve_buffer.get());
}