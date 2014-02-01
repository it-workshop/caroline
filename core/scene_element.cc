// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/scene_element.h"

namespace core {

SceneElement::SceneElement() {
  SetStandartTransform();
}

SceneElement::SceneElement(Mesh *mesh) {
  SetStandartTransform();

  mesh_ = mesh;
}

void SceneElement::SetMesh(Mesh *mesh) {
  mesh_ = mesh;
}

void SceneElement::SetPos(const double &x, const double &y, const double &z) {
  pos_x_ = x;
  pos_y_ = y;
  pos_z_ = z;
}

void SceneElement::SetScale(const double &scale_x,
    const double &scale_y, const double &scale_z) {
  scale_x_ = scale_x;
  scale_y_ = scale_y;
  scale_z_ = scale_z;
}

std::vector<Point3D> SceneElement::Vertexes() const {
  std::vector<Point3D> new_vertexes;
  std::vector<Point3D> old_vertexes = mesh_->Vertexes();

  for (int i = 0; i < old_vertexes.size(); i++) {
    new_vertexes.push_back(Transform(old_vertexes.at(i)));
  }
}

std::vector<Triangle> SceneElement::Faces() const {
  return mesh_->Faces();
}

Point3D SceneElement::Transform(Point3D point) const {
  Point3D new_point;

  new_point.Set(point.X() + pos_x_, point.Y() + pos_y_, point.Z() + pos_z_);

  // DO LATER
  // Rotation and scaling
}

void SceneElement::SetStandartTransform() {
  pos_x_ = 0;
  pos_y_ = 0;
  pos_z_ = 0;

  scale_x_ = 1;
  scale_y_ = 1;
  scale_z_ = 1;

  rotation_x_ = 0;
  rotation_y_ = 0;
  rotation_z_ = 0;
}

}  // namespace core
