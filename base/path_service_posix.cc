// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/path_service_posix.h"

#include <memory>
#include <string>

#include "base/path_impl_posix.h"

namespace base {

PathServicePosix::PathServicePosix() {
  root_path_ = PathFromImpl(
      std::unique_ptr<Path::Impl>(new PathImplPosix("/")));
}

bool PathServicePosix::Init(const std::string& executable_path) {
  executable_path_ = PathFromImpl(
      std::unique_ptr<Path::Impl>(new PathImplPosix(executable_path)));
  return true;
}

Path PathServicePosix::GetWorkingDirectoryPath() const {
  return PathFromImpl(std::unique_ptr<Path::Impl>(new PathImplPosix(".")));
}

Path PathServicePosix::MakePath(const std::string& path) const {
  return PathFromImpl(std::unique_ptr<Path::Impl>(new PathImplPosix(path)));
}

// static
std::unique_ptr<PathService> PathService::CreateInstance() {
  return std::unique_ptr<PathService>(new PathServicePosix());
}

}  // namespace base
