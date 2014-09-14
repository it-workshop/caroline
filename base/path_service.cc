// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// @author Aleksandr Derbenev <13alexac@gmail.com>

#include "base/path_service.h"

INSTANCE_SINGLETON(base::PathService);

namespace base {

// static
Path PathService::PathFromImpl(std::unique_ptr<Path::Impl>&& impl) {
  return Path(std::move(impl));
}

}  // namespace base
