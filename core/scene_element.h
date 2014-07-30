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
  static constexpr double merge_error = 0.2;
  explicit SceneElement(Mesh *mesh);

  void SetMesh(Mesh *mesh);
  void AddVertex(const Point3D& point);
  void AddFace(const Triangle& face);

  void set_x(double x) { pos_x_ = x; }
  void set_y(double y) { pos_y_ = y; }
  void set_z(double z) { pos_z_ = z; }
  void SetPos(double x, double y, double z);

  void set_scale_x(double scale) { scale_x_ = scale; }
  void set_scale_y(double scale) { scale_y_ = scale; }
  void set_scale_z(double scale) { scale_z_ = scale; }
  void SetScale(double scale_x, double scale_y, double scale_z);

  void set_axis_x(double x) { axis_x_ = x; }
  void set_axis_y(double y) { axis_y_ = y; }
  void set_axis_z(double z) { axis_z_ = z; }

  void set_angle(double angle) { angle_ = angle; }

  void set_rotation_center_x(double x) { rotation_center_x_ = x; }
  void set_rotation_center_y(double y) { rotation_center_y_ = y; }
  void set_rotation_center_z(double z) { rotation_center_z_ = z; }

  double x() const { return pos_x_; }
  double y() const { return pos_y_; }
  double z() const { return pos_z_; }

  double scale_x() const { return scale_x_; }
  double scale_y() const { return scale_y_; }
  double scale_z() const { return scale_z_; }

  double axis_x() const { return axis_x_; }
  double axis_y() const { return axis_y_; }
  double axis_z() const { return axis_z_; }

  double angle() const { return angle_; }

  double rotation_center_x() const { return rotation_center_x_; }
  double rotation_center_y() const { return rotation_center_y_; }
  double rotation_center_z() const { return rotation_center_z_; }

  std::vector<Point3D> Vertexes() const;
  std::vector<Triangle> Faces() const;

  void ChangeVertex(Point3D point, int i) {
    mesh_->ChangeVertex(point, i);
  }

  void Transform(SceneElement* transformed_scene) const;

  Mesh Merge(const Mesh& mesh, const SceneElement& result_scene);

 protected:
  void Transform(Point3D* point, const Point3D& mean_point) const;
  Point3D FindMin(void) const;
  Point3D FindMax(void) const;
  Point3D FindMeanPoint(void) const;

  void SetStandardTransform();

 private:
  Mesh *mesh_;

  double pos_x_;
  double pos_y_;
  double pos_z_;

  double scale_x_;
  double scale_y_;
  double scale_z_;

  double axis_x_;
  double axis_y_;
  double axis_z_;

  double angle_;

  double rotation_center_x_;
  double rotation_center_y_;
  double rotation_center_z_;
};

}  // namespace core

#endif  // CORE_SCENE_ELEMENT_H_
