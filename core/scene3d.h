// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_SCENE3D_H_
#define CORE_SCENE3D_H_

#include <vector>

#include "core/scene_element.h"

namespace core {

class Scene3D {
 public:
  Scene3D();

  void AddElement(const SceneElement& element);

  int NumberOfElements() const;

  // Number must be less then NumberOfElements.
  SceneElement ElementAt(int number) const;

 private:
  std::vector<SceneElement> scene_elements_;
};

}  // namespace core

#endif  // CORE_SCENE3D_H_