// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/metric_factory.h"

#include "core/metric_impls.h"

namespace core {

namespace stat {

std::unique_ptr<Metric> MetricFactory::Create(const std::string &name) {
  if (name == kSimpleDist)
    return std::unique_ptr<Metric>(new SimpleDist);

  if (name == kRSquare)
    return std::unique_ptr<Metric>(new RSquare);

  return nullptr;
}

}  // namespace stat

}  // namespace core
