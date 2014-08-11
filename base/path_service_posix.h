// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_PATH_SERVICE_POSIX_H_
#define BASE_PATH_SERVICE_POSIX_H_

#include <string>

#include "base/path_service.h"

namespace base {

class PathServicePosix : public PathService {
 public:
  PathServicePosix();
  virtual ~PathServicePosix() {}

  virtual bool Init(const std::string& executable_path) override;

  virtual Path GetWorkingDirectoryPath() const override;

  virtual Path MakePath(const std::string& path) const override;
};

}  // namespace base

#endif  // BASE_PATH_SERVICE_POSIX_H_
