// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/path_service_posix.h"

#include <memory>
#include <string>

#include "base/path_impl_posix.h"

namespace base {

PathServicePosix::PathServicePosix(const std::string& executable_path) {
  root_path_ = PathFromImpl(
      std::unique_ptr<Path::Impl>(new PathImplPosix("/")));
  executable_path_ = PathFromImpl(std::move(
      std::unique_ptr<Path::Impl>(new PathImplPosix(executable_path))));
}

// static
bool PathService::Init(const std::string& executable_path) {
  return new PathServicePosix(executable_path);
}

Path PathServicePosix::GetWorkingDirectoryPath() const {
  return PathFromImpl(std::unique_ptr<Path::Impl>(new PathImplPosix(".")));
}

// static
Path PathService::MakePath(const std::string& path) {
  return PathFromImpl(std::unique_ptr<Path::Impl>(new PathImplPosix(path)));
}

}  // namespace base
