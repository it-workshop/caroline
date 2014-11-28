// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/metric_factory.h"

#include "base/logging.h"
#include "core/metric_impls.h"
#include "core/preferences_service.h"

namespace core {

namespace stat {

namespace {

const char kMetricsConfigNodeName[] = "metrics";

}  // namespace

// static
std::unique_ptr<Metric> MetricFactory::Create(const std::string &name) {
  if (name == kSimpleDist)
    return std::unique_ptr<Metric>(new SimpleDist);

  if (name == kRSquare)
    return std::unique_ptr<Metric>(new RSquare);

  return nullptr;
}

// static
void MetricFactory::RegisterPreferences() {
  PrefService* prefs = PrefService::GetInstance();
  DCHECK(prefs);

  DCHECK(prefs->RegisterList(kMetricsConfigNodeName));
  Json::Value* names = prefs->GetList(kMetricsConfigNodeName);
  DCHECK(names);
  names->append(kSimpleDist);
  names->append(kRSquare);
}

// static
std::vector<std::unique_ptr<Metric>> MetricFactory::CreateFromPreferences() {
  PrefService* prefs = PrefService::GetInstance();
  DCHECK(prefs);

  const Json::Value* names = prefs->GetList(kMetricsConfigNodeName);
  std::vector<std::unique_ptr<Metric>> metrics;
  for (const Json::Value& name : *names) {
    if (!name.isString()) {
      LOG(WARNING) << "String expected for metric name.";
      continue;
    }

    std::unique_ptr<Metric> metric = Create(name.asString());
    if (!metric) {
      LOG(WARNING) << "Unknown metric: " << name;
      continue;
    }

    metrics.push_back(std::move(metric));
  }
  
  return metrics;
}

}  // namespace stat

}  // namespace core
