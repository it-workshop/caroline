// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/scene_element.h"
#include "core/rotation_matrix.h"
#include <cmath>

namespace core {

SceneElement::SceneElement() {
  SetStandardTransform();
}

SceneElement::SceneElement(Mesh *mesh) {
  SetStandardTransform();

  mesh_ = mesh;
}

void SceneElement::SetMesh(Mesh *mesh) {
  mesh_ = mesh;
}

void SceneElement::SetPos(double x, double y, double z) {
  pos_x_ = x;
  pos_y_ = y;
  pos_z_ = z;
}

void SceneElement::SetScale(double scale_x, double scale_y, double scale_z) {
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

  return new_vertexes;
}

std::vector<Triangle> SceneElement::Faces() const {
  return mesh_->Faces();
}

Point3D SceneElement::FindMin(void) const {
  double min_x_ = mesh_->Vertexes()[0].x();
  double min_y_ = mesh_->Vertexes()[0].y();
  double min_z_ = mesh_->Vertexes()[0].z();

  for (int i = 0; i < mesh_->Vertexes().size(); i++) {

    if (mesh_->Vertexes()[i].x() < min_x_)
      min_x_ = mesh_->Vertexes()[i].x();
    if (mesh_->Vertexes()[i].y() < min_y_)
      min_y_ = mesh_->Vertexes()[i].y();
    if (mesh_->Vertexes()[i].z() < min_z_)
      min_z_ = mesh_->Vertexes()[i].z();
  }
  Point3D min_point;
  min_point.Set(min_x_,min_y_,min_z_);
  return min_point;
}

Point3D SceneElement::FindMax(void) const {
  double max_x_ = mesh_->Vertexes()[0].x();
  double max_y_ = mesh_->Vertexes()[0].y();
  double max_z_ = mesh_->Vertexes()[0].z();

  for (int i = 0; i < mesh_->Vertexes().size(); i++) {

    if (mesh_->Vertexes()[i].x() > max_x_)
      max_x_ = mesh_->Vertexes()[i].x();
    if (mesh_->Vertexes()[i].y() > max_y_)
      max_y_ = mesh_->Vertexes()[i].y();
    if (mesh_->Vertexes()[i].z() > max_z_)
      max_z_ = mesh_->Vertexes()[i].z();
  }
  Point3D max_point;
  max_point.Set(max_x_,max_y_,max_z_);
  return max_point;
}

Point3D SceneElement::Transform(Point3D point) const {
  Point3D new_point;

  new_point.Set(point.x() + pos_x_, point.y() + pos_y_, point.z() + pos_z_);

  Point3D MinPoint = this->FindMin();
  Point3D MaxPoint = this->FindMax();

  double mean_x_ = (MaxPoint.x() + MinPoint.x()) /2 ;
  double mean_y_ = (MaxPoint.y() + MinPoint.y()) /2 ;
  double mean_z_ = (MaxPoint.z() + MinPoint.z()) /2 ;
  
  new_point.set_x ( (new_point.x() - mean_x_) * scale_x_ + mean_x_);
  new_point.set_y ( (new_point.y() - mean_y_) * scale_y_ + mean_y_);
  new_point.set_z ( (new_point.z() - mean_z_) * scale_z_ + mean_z_);
    
  Point3D rotated_point;

  rotated_point.set_x ( new_point.x() - rotation_center_x() );
  rotated_point.set_y ( new_point.y() - rotation_center_y() );
  rotated_point.set_z ( new_point.z() - rotation_center_z() );

  Rotation_Matrix rot_( angle_ , axis_x_, axis_y_, axis_z_);
  rot_.Rotate(rotated_point);

  new_point.set_x ( rotated_point.x() + rotation_center_x() );
  new_point.set_y ( rotated_point.y() + rotation_center_y() );
  new_point.set_z ( rotated_point.z() + rotation_center_z() );
  
  return new_point;
}

SceneElement SceneElement::Transform(SceneElement scene_element) const {
  SceneElement moved_scene_element = scene_element;
  for (int i = 0; i < scene_element.Vertexes().size(); i++)
    moved_scene_element.Vertexes()[i] = scene_element.Transform(scene_element.Vertexes()[i]);
  return moved_scene_element;
}

void SceneElement::SetStandardTransform() {
  pos_x_ = 0;
  pos_y_ = 0;
  pos_z_ = 0;

  scale_x_ = 1;
  scale_y_ = 1;
  scale_z_ = 1;
  
  axis_x_ = 1;
  axis_y_ = 0;
  axis_z_ = 0;

  angle_ = 0;

  rotation_center_x_ = 0;
  rotation_center_y_ = 0;
  rotation_center_z_ = 0;
}

}  // namespace core
