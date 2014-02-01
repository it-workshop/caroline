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

  void SetX(const double& x) { pos_x_ = x; }
  void SetY(const double& y) { pos_y_ = y; }
  void SetZ(const double& z) { pos_z_ = z; }
  void SetPos(const double& x, const double& y, const double& z);

  void SetXScale(const double& scale_x) { scale_x_ = scale_x; }
  void SetYScale(const double& scale_y) { scale_y_ = scale_y; }
  void SetZScale(const double& scale_z) { scale_z_ = scale_z; }
  void SetScale(const double& scale_x,
    const double& scale_y, const double& scale_z);

  void SetRotationX(const double& rotation_x) { rotation_x_ = rotation_x; }
  void SetRotationY(const double& rotation_y) { rotation_y_ = rotation_y; }
  void SetRotationZ(const double& rotation_z) { rotation_z_ = rotation_z; }

  double X() const { return pos_x_; }
  double Y() const { return pos_y_; }
  double Z() const { return pos_z_; }

  double XScale() const { return scale_x_; }
  double YScale() const { return scale_y_; }
  double ZScale() const { return scale_z_; }

  double RotationX() const { return rotation_x_; }
  double RotationY() const { return rotation_y_; }
  double RotationZ() const { return rotation_z_; }

  std::vector<Point3D> Vertexes() const;
  std::vector<Triangle> Faces() const;

 protected:
  Point3D Transform(Point3D point) const;

  void SetStandartTransform();

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
