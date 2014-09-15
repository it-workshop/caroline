// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Sirotkin Dmitry <dmitriy.v.sirotkin@gmail.com>

#ifndef CORE_MAP_FILTER_H_
#define CORE_MAP_FILTER_H_

namespace core {

class DepthMap;

class MapFilter {
 public:
  MapFilter() {}

  virtual ~MapFilter() {}

  virtual DepthMap filter(const DepthMap &map) = 0;
};

}  // namespace core

#endif  // CORE_MAP_FILTER_H_
