// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_PATH_IMPL_POSIX_H_
#define BASE_PATH_IMPL_POSIX_H_

#include <string>
#include <vector>

#include "base/path.h"

namespace base {

class PathImplPosix : public Path::Impl {
 public:
  explicit PathImplPosix(const std::string& path);
  virtual ~PathImplPosix() {}

  virtual void Append(const std::string& filename) override;
  virtual void ToParentDir() override;
  virtual bool IsExists() const override;
  virtual bool IsDirectory() const override;
  virtual bool IsEqualTo(const Impl* path) const override;
  virtual std::string GetSpec() const override;
  virtual std::string GetUrl() const override;
  virtual std::unique_ptr<Path::Impl> Copy() const override;

 private:
  void Optimize();
  std::vector<std::string> path_;
};

}  // namespace base

#endif  // BASE_PATH_IMPL_POSIX_H_
