// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/path.h"

#include <algorithm>

#include "base/path_service.h"

namespace base {

Path::Path(const std::string& path)
  : impl_(PathService::GetInstance()->MakePath(path).impl_.release()) {}

Path::Path(const Path& path)
  : impl_(path.impl_->Copy()) {}

Path::Path(Path&& path)
  : impl_(path.impl_.release()) {}

Path::Path()
  : impl_(PathService::GetInstance()->root_path().impl_->Copy()) {}

Path::Path(std::unique_ptr<Impl>&& impl)
  : impl_(std::move(impl)) {}

Path& Path::operator=(const Path& path) {
  impl_ = path.impl_->Copy();
  return *this;
}

Path& Path::operator=(Path&& path) {
  impl_.reset(path.impl_.release());
  return *this;
}

std::unique_ptr<Path::Impl> Path::Impl::Join(
    const std::string& filename) const {
  std::unique_ptr<Impl> copy(this->Copy());
  copy->Append(filename);
  return copy;
}

std::unique_ptr<Path::Impl> Path::Impl::ParentDir() const {
  std::unique_ptr<Impl> copy(this->Copy());
  copy->ToParentDir();
  return copy;
}

}  // namespace base
