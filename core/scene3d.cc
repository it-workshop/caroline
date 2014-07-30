// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/scene3d.h"

namespace core {

Scene3D::Scene3D() {
}

void Scene3D::AddElement(SceneElement *element) {
  scene_elements_.push_back(element);
}

int Scene3D::NumberOfElements() const {
  return scene_elements_.size();
}

SceneElement *Scene3D::ElementAt(int number) const {
  return scene_elements_.at(number);
}

}  // namespace core
