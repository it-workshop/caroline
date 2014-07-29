// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>
// Author: Mlodik Mikhail <mlodik_m@mail.ru>

#include "core/scene_element.h"

#include <cmath>
#include <cstddef>
#include <iostream>

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

  for (size_t i = 0; i < old_vertexes.size(); i++) {
    new_vertexes.push_back(Transform(old_vertexes.at(i)));
  }

  return new_vertexes;
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

Point3D SceneElement::Transform(Point3D point) const {
  Point3D new_point;

  new_point.Set(point.x() + pos_x_, point.y() + pos_y_, point.z() + pos_z_);

  Point3D MinPoint = this->FindMin();
  Point3D MaxPoint = this->FindMax();

  double mean_x = (MaxPoint.x() + MinPoint.x()) /2;
  double mean_y = (MaxPoint.y() + MinPoint.y()) /2;
  double mean_z = (MaxPoint.z() + MinPoint.z()) /2;

  new_point.set_x((new_point.x() - mean_x) * scale_x_ + mean_x);
  new_point.set_y((new_point.y() - mean_y) * scale_y_ + mean_y);
  new_point.set_z((new_point.z() - mean_z) * scale_z_ + mean_z);

  Point3D rotated_point;

  rotated_point.set_x(new_point.x() - rotation_center_x() );
  rotated_point.set_y(new_point.y() - rotation_center_y() );
  rotated_point.set_z(new_point.z() - rotation_center_z() );

  RotationMatrix rot_(angle_, axis_x_, axis_y_, axis_z_);
  rotated_point = rot_.Rotate(rotated_point);

  new_point.set_x(rotated_point.x() + rotation_center_x() );
  new_point.set_y(rotated_point.y() + rotation_center_y() );
  new_point.set_z(rotated_point.z() + rotation_center_z() );

  return new_point;
}

SceneElement SceneElement::Transform(SceneElement scene_element) const {
  SceneElement moved_scene_element = scene_element;
  Point3D tmp;
  for (int i = 0; i < scene_element.Vertexes().size(); i++) {
    tmp = scene_element.Vertexes()[i];
    moved_scene_element.Vertexes()[i] = scene_element.Transform(tmp);
  }
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

void SceneElement::Merge(const Mesh& mesh,
                         SceneElement* result_scene) {
  Mesh new_mesh = MergeSortByX(mesh);
  SceneElement old_scene = *result_scene;

  SceneElement new_scene = old_scene.Transform(old_scene);
  Mesh TmpMesh;
  for (int i = 0; i < new_scene.Vertexes().size(); i++)
    TmpMesh.AddVertex(new_scene.Vertexes().at(i));
  for (int i = 0; i < new_scene.Faces().size(); i++)
    TmpMesh.AddFace(new_scene.Faces().at(i));

  const int shift = old_scene.Vertexes().size();
  const int old_faces_number = old_scene.Faces().size();

  for (int i = 0; i < new_mesh.Vertexes().size(); i++) {
    TmpMesh.AddVertex(new_mesh.Vertexes()[i]);
  }
  for (int i = 0; i < mesh.Faces().size(); i++) {
    Triangle TmpFace(
      mesh.Faces()[i].Point1() + shift,
      mesh.Faces()[i].Point2() + shift,
      mesh.Faces()[i].Point3() + shift);
    TmpMesh.AddFace(TmpFace);
  }

  std::vector<bool> destined_to_be_destroyed;
  for (int i = 0; i < TmpMesh.Vertexes().size(); i++)
    destined_to_be_destroyed.push_back(false);

  int vert_counter = TmpMesh.Vertexes().size();
  for (int i = 0; (i < shift) && (vert_counter > shift); i++) {
    Point3D curr_point = TmpMesh.Vertexes().at(i);

    int begin = shift;
    int end = TmpMesh.Vertexes().size();
    int left_border = BinarySearchByX(TmpMesh.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x() - merge_error,
                                      '<');
    int right_border = BinarySearchByX(TmpMesh.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x() + merge_error,
                                      '>');

    for (int j = left_border; j <= right_border; j++)
      if ((fabs(curr_point.y() - TmpMesh.Vertexes().at(j).y()) < merge_error)
         &&
         (fabs(curr_point.z() - TmpMesh.Vertexes().at(j).z()) < merge_error)) {
        vert_counter--;
        for (int i0 = old_faces_number; i0 < new_mesh.Faces().size(); i0++) {
          if (new_mesh.Faces().at(i0).Point1() == j) {
            Triangle TmpFace(i,
                              new_mesh.Faces().at(i0).Point2(),
                              new_mesh.Faces().at(i0).Point3());
            new_mesh.Faces().at(i0) = TmpFace;
          }
          if (new_mesh.Faces().at(i0).Point2() == j) {
            Triangle TmpFace(new_mesh.Faces().at(i0).Point1(),
                              i,
                              new_mesh.Faces().at(i0).Point3());
            new_mesh.Faces().at(i0) = TmpFace;
          }
          if (new_mesh.Faces().at(i0).Point3() == j) {
            Triangle TmpFace(new_mesh.Faces().at(i0).Point1(),
                              new_mesh.Faces().at(i0).Point2(),
                              i);
            new_mesh.Faces().at(i0) = TmpFace;
          }
        }
        destined_to_be_destroyed.at(j) = true;
      }
    }
  new_scene.SetMesh(&TmpMesh);

  Mesh result_mesh;
  for (int i = 0; i < new_scene.Vertexes().size(); i++) {
    if (destined_to_be_destroyed.at(i) == false)
      result_mesh.AddVertex(new_scene.Vertexes().at(i));}
  result_mesh.Vertexes().shrink_to_fit();

  for (int i = 0; i < new_scene.Faces().size(); i++)
    result_mesh.AddFace(new_scene.Faces().at(i));
  new_scene.SetMesh(&result_mesh);
  *result_scene = new_scene;
  std::cout << result_scene->Vertexes().size() << '\n';
}

}  // namespace core
