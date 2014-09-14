// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_METRIC_FACTORY_H_
#define CORE_METRIC_FACTORY_H_

#include <memory>
#include <string>

#include "core/metric.h"

namespace core {

namespace stat {

/// Used to create different metrics.
class MetricFactory {
 public:
  /// Constructor.
  MetricFactory() {}

  /// Creates the metric of type = name.
  /// @param[in] name Name of the created metric.
  static std::unique_ptr<Metric> Create(const std::string &name);
};

}  // namespace stat

}  // namespace core


#endif  // CORE_METRIC_FACTORY_H_
