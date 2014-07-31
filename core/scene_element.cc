// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>
// Author: Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/scene_element.h"

#include <cmath>
#include <cstddef>
#include <vector>

#include "core/rotation_matrix.h"
#include "core/mesh_merge_utils.h"

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

void SceneElement::AddVertex(const Point3D &point) {
  mesh_->AddVertex(point);
}

void SceneElement::AddFace(const Triangle &face) {
  mesh_->AddFace(face);
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
  return mesh_->Vertexes();
}

std::vector<Triangle> SceneElement::Faces() const {
  return mesh_->Faces();
}

Point3D SceneElement::FindMin(void) const {
  double min_x = mesh_->Vertexes()[0].x();
  double min_y = mesh_->Vertexes()[0].y();
  double min_z = mesh_->Vertexes()[0].z();

  for (int i = 0; i < mesh_->Vertexes().size(); i++) {
    if (mesh_->Vertexes()[i].x() < min_x)
      min_x = mesh_->Vertexes()[i].x();
    if (mesh_->Vertexes()[i].y() < min_y)
      min_y = mesh_->Vertexes()[i].y();
    if (mesh_->Vertexes()[i].z() < min_z)
      min_z = mesh_->Vertexes()[i].z();
  }
  Point3D min_point;
  min_point.Set(min_x, min_y, min_z);
  return min_point;
}

Point3D SceneElement::FindMax(void) const {
  double max_x = mesh_->Vertexes()[0].x();
  double max_y = mesh_->Vertexes()[0].y();
  double max_z = mesh_->Vertexes()[0].z();

  for (int i = 0; i < mesh_->Vertexes().size(); i++) {
    if (mesh_->Vertexes()[i].x() > max_x)
      max_x = mesh_->Vertexes()[i].x();
    if (mesh_->Vertexes()[i].y() > max_y)
      max_y = mesh_->Vertexes()[i].y();
    if (mesh_->Vertexes()[i].z() > max_z)
      max_z = mesh_->Vertexes()[i].z();
  }
  Point3D max_point;
  max_point.Set(max_x, max_y, max_z);
  return max_point;
}

Point3D SceneElement::FindMeanPoint(void) const {
  Point3D new_point;
  new_point.Set((this->FindMin().x() + this->FindMax().x()) / 2,
                (this->FindMin().y() + this->FindMax().y()) / 2,
                (this->FindMin().z() + this->FindMax().z()) / 2);
  return new_point;
}

void SceneElement::Transform(Point3D* point, const Point3D& mean_point) const {
  Point3D new_point;

  new_point.Set(point->x() + pos_x_, point->y() + pos_y_, point->z() + pos_z_);

  new_point.set_x((new_point.x() - mean_point.x()) * scale_x_ + mean_point.x());
  new_point.set_y((new_point.y() - mean_point.y()) * scale_y_ + mean_point.y());
  new_point.set_z((new_point.z() - mean_point.z()) * scale_z_ + mean_point.z());

  Point3D rotated_point;

  rotated_point.set_x(new_point.x() - rotation_center_x() );
  rotated_point.set_y(new_point.y() - rotation_center_y() );
  rotated_point.set_z(new_point.z() - rotation_center_z() );

  RotationMatrix rot_(angle_, axis_x_, axis_y_, axis_z_);
  rotated_point = rot_.Rotate(rotated_point);

  new_point.set_x(rotated_point.x() + rotation_center_x() );
  new_point.set_y(rotated_point.y() + rotation_center_y() );
  new_point.set_z(rotated_point.z() + rotation_center_z() );

  *point = new_point;
}

void SceneElement::Transform(SceneElement* scene) const {
  Point3D tmp;
  Point3D mean_point = this->FindMeanPoint();
  for (int i = 0; i < scene->Vertexes().size(); i++) {
    tmp = scene->Vertexes().at(i);
    scene->Transform(&tmp, mean_point);
    scene->ChangeVertex(tmp, i);
  }
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

Mesh SceneElement::Merge(const Mesh& mesh,
                         const SceneElement& scene) {
  Mesh sorted_mesh = MergeSortByX(mesh);

  SceneElement new_scene = scene;
  new_scene.Transform(&new_scene);

  const int shift = scene.Vertexes().size();

  for (int i = 0; i < sorted_mesh.Vertexes().size(); i++) {
    new_scene.AddVertex(sorted_mesh.Vertexes()[i]);
  }
  for (int i = 0; i < sorted_mesh.Faces().size(); i++) {
    Triangle TmpFace(
      mesh.Faces()[i].Point1() + shift,
      mesh.Faces()[i].Point2() + shift,
      mesh.Faces()[i].Point3() + shift);
    new_scene.AddFace(TmpFace);
  }

  std::vector<int> point_to_merge;
  for (int i = 0; i < new_scene.Vertexes().size(); i++)
    point_to_merge.push_back(i);

  int vert_counter = new_scene.Vertexes().size();

  for (int i = 0; (i < shift) && (vert_counter > shift); i++) {
    Point3D curr_point = new_scene.Vertexes().at(i);

    int begin = shift;
    int end = new_scene.Vertexes().size();
    int left_border = BinarySearchByX(new_scene.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x() - merge_error,
                                      '<');
    int right_border = BinarySearchByX(new_scene.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x() + merge_error,
                                      '>');

    for (int j = left_border; j <= right_border; j++)
      if ((fabs(curr_point.x() - new_scene.Vertexes().at(j).x())
          < merge_error)
          &&
          (fabs(curr_point.y() - new_scene.Vertexes().at(j).y())
          < merge_error)
          &&
          (fabs(curr_point.z() - new_scene.Vertexes().at(j).z())
          < merge_error)) {
        vert_counter--;
        point_to_merge.at(j) = i;
      }
    }

  Mesh GluedMesh;
  std::vector<int> old_number;
  std::vector<int> new_number;

  for (int i = 0; i < new_scene.Vertexes().size(); i++) {
    if (point_to_merge.at(i) == i) {
      GluedMesh.AddVertex(new_scene.Vertexes()[i]);
      old_number.push_back(i);
      new_number.push_back(old_number.size() - 1);
    } else {
      new_number.push_back(-1);
    }
  }

  for (int i = 0; i < new_scene.Faces().size(); i++) {
    Triangle TmpFace(new_number[point_to_merge[new_scene.Faces()[i].Point1()]],
                     new_number[point_to_merge[new_scene.Faces()[i].Point2()]],
                     new_number[point_to_merge[new_scene.Faces()[i].Point3()]]);
    GluedMesh.AddFace(TmpFace);
  }

  return GluedMesh;
}

}  // namespace core
