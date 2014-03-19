// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_SCENE_ELEMENT_H_
#define CORE_SCENE_ELEMENT_H_

#include <vector>

#include "core/point3d.h"
#include "core/mesh.h"

namespace core {

class SceneElement {
 public:
  SceneElement();
  explicit SceneElement(Mesh *mesh);

  void SetMesh(Mesh *mesh);

  void set_x(double x) { pos_x_ = x; }
  void set_y(double y) { pos_y_ = y; }
  void set_z(double z) { pos_z_ = z; }
  void SetPos(double x, double y, double z);

  void set_scale_x(double scale) { scale_x_ = scale; }
  void set_scale_y(double scale) { scale_y_ = scale; }
  void set_scale_z(double scale) { scale_z_ = scale; }
  void SetScale(double scale_x, double scale_y, double scale_z);

  void set_rotation_x(double rotation) { rotation_x_ = rotation; }
  void set_rotation_y(double rotation) { rotation_y_ = rotation; }
  void set_rotation_z(double rotation) { rotation_z_ = rotation; }

  double x() const { return pos_x_; }
  double y() const { return pos_y_; }
  double z() const { return pos_z_; }

  double scale_x() const { return scale_x_; }
  double scale_y() const { return scale_y_; }
  double scale_z() const { return scale_z_; }

  double rotation_x() const { return rotation_x_; }
  double rotation_y() const { return rotation_y_; }
  double rotation_z() const { return rotation_z_; }

  std::vector<Point3D> Vertexes() const;
  std::vector<Triangle> Faces() const;

 protected:
  Point3D Transform(Point3D point) const;

  void SetStandardTransform();

 private:
  Mesh *mesh_;

  double pos_x_;
  double pos_y_;
  double pos_z_;

  double scale_x_;
  double scale_y_;
  double scale_z_;

  double rotation_x_;
  double rotation_y_;
  double rotation_z_;
};

}  // namespace core

#endif  // CORE_SCENE_ELEMENT_H_
