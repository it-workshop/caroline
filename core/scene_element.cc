// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include "core/scene_element.h"
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

Point3D SceneElement::Transform(Point3D point) const {
  Point3D new_point;

  new_point.Set(point.x() + pos_x_, point.y() + pos_y_, point.z() + pos_z_);

  // TODO(VladimirGl): Rotation and scaling
  
  double min_x_ = mesh_->Vertexes()[0].x();
  double max_x_ = mesh_->Vertexes()[0].x();
  double min_y_ = mesh_->Vertexes()[0].y();
  double max_y_ = mesh_->Vertexes()[0].y();
  double min_z_ = mesh_->Vertexes()[0].z();
  double max_z_ = mesh_->Vertexes()[0].z();
  
  
  for (int i = 0; i < mesh_->Vertexes().size(); i++) {     
 
      if (mesh_->Vertexes()[i].x() < min_x_) 
         min_x_ = mesh_->Vertexes()[i].x();
      if (mesh_->Vertexes()[i].y() < min_y_) 
         min_y_ = mesh_->Vertexes()[i].y();
      if (mesh_->Vertexes()[i].z() < min_z_) 
         min_z_ = mesh_->Vertexes()[i].z();
      
      if (mesh_->Vertexes()[i].x() > max_x_) 
         max_x_ = mesh_->Vertexes()[i].x();
      if (mesh_->Vertexes()[i].y() > max_y_) 
         max_y_ = mesh_->Vertexes()[i].y();
      if (mesh_->Vertexes()[i].z() > max_z_) 
         max_z_ = mesh_->Vertexes()[i].z();
  }
  
  double mean_x_ = (max_x_ - min_x_) /2 ;
  double mean_y_ = (max_y_ - min_y_) /2 ;
  double mean_z_ = (max_z_ - min_z_) /2 ;
  
  for (int i = 0; i < mesh_->Vertexes().size(); i++) {
      mesh_->Vertexes()[i].x() = (mesh_->Vertexes()[i].x() - mean_x_) * scale_x_ + mean_x_; 
      mesh_->Vertexes()[i].y() = (mesh_->Vertexes()[i].y() - mean_y_) * scale_y_ + mean_y_; 
      mesh_->Vertexes()[i].z() = (mesh_->Vertexes()[i].z() - mean_z_) * scale_y_ + mean_z_;       
  }
  
  double new_x_;
  double new_y_;
  double new_z_;
  
  for (int i = 0; i < mesh_->Vertexes().size(); i++) {
      double tmp;
      
      new_x_ = mesh_->Vertexes()[i].x() - rotation_center_x();    
      new_y_ = mesh_->Vertexes()[i].y() - rotation_center_y();    
      new_z_ = mesh_->Vertexes()[i].z() - rotation_center_z();    
      
      tmp = new_x_;
      new_x_ = tmp * cos(rotation_xy()) - new_y_ * sin(rotation_xy());
      new_y_ = tmp * sin(rotation_xy()) + new_y_ * cos(rotation_xy());
      
      tmp = new_x_;
      new_x_ = tmp * cos(rotation_xz()) - new_z_ * sin(rotation_xz());
      new_z_ = tmp * sin(rotation_xz()) + new_z_ * cos(rotation_xz());

      tmp = new_y_;
      new_y_ = tmp * cos(rotation_yz()) - new_z_ * sin(rotation_yz());
      new_z_ = tmp * sin(rotation_yz()) + new_z_ * cos(rotation_yz());

      mesh_->Vertexes()[i].x() = new_x_ + rotation_center_x();         
      mesh_->Vertexes()[i].y() = new_y_ + rotation_center_y();         
      mesh_->Vertexes()[i].z() = new_z_ + rotation_center_z();         

  }
  
  return new_point;
}

void SceneElement::SetStandardTransform() {
  pos_x_ = 0;
  pos_y_ = 0;
  pos_z_ = 0;

  scale_x_ = 1;
  scale_y_ = 1;
  scale_z_ = 1;
  
  rotation_xy_=0;
  rotation_xz_=0;
  rotation_yz_=0;

  rotation_center_x_=0;
  rotation_center_y_=0;
  rotation_center_z_=0;
}

}  // namespace core
