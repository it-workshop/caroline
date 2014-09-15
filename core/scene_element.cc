// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

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

void SceneElement::AddVertex(const cv::Point3d &point) {
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

std::vector<cv::Point3d> SceneElement::Vertexes() const {
  return mesh_->vertexes();
}

std::vector<Triangle> SceneElement::Faces() const {
  return mesh_->faces();
}

cv::Point3d SceneElement::FindMin() const {
  double min_x = mesh_->vertexes()[0].x;
  double min_y = mesh_->vertexes()[0].y;
  double min_z = mesh_->vertexes()[0].z;

  for (int i = 0; i < mesh_->vertexes().size(); i++) {
    if (mesh_->vertexes()[i].x < min_x)
      min_x = mesh_->vertexes()[i].x;
    if (mesh_->vertexes()[i].y < min_y)
      min_y = mesh_->vertexes()[i].y;
    if (mesh_->vertexes()[i].z < min_z)
      min_z = mesh_->vertexes()[i].z;
  }
  cv::Point3d min_point(min_x, min_y, min_z);
  return min_point;
}

cv::Point3d SceneElement::FindMax() const {
  double max_x = mesh_->vertexes()[0].x;
  double max_y = mesh_->vertexes()[0].y;
  double max_z = mesh_->vertexes()[0].z;

  for (int i = 0; i < mesh_->vertexes().size(); i++) {
    if (mesh_->vertexes()[i].x > max_x)
      max_x = mesh_->vertexes()[i].x;
    if (mesh_->vertexes()[i].y > max_y)
      max_y = mesh_->vertexes()[i].y;
    if (mesh_->vertexes()[i].z > max_z)
      max_z = mesh_->vertexes()[i].z;
  }
  cv::Point3d max_point(max_x, max_y, max_z);
  return max_point;
}

cv::Point3d SceneElement::FindMeanPoint() const {
  cv::Point3d new_point(
        (this->FindMin().x + this->FindMax().x) / 2,
        (this->FindMin().y + this->FindMax().y) / 2,
        (this->FindMin().z + this->FindMax().z) / 2);
  return new_point;
}

void SceneElement::Transform(
    cv::Point3d* point, const cv::Point3d& mean_point) const {
  cv::Point3d new_point(point->x + pos_x_,
                        point->y + pos_y_,
                        point->z + pos_z_);

  new_point.x = (new_point.x - mean_point.x) * scale_x_ + mean_point.x;
  new_point.y = (new_point.y - mean_point.y) * scale_y_ + mean_point.y;
  new_point.z = (new_point.z - mean_point.z) * scale_z_ + mean_point.z;

  cv::Point3d rotated_point;

  rotated_point.x = new_point.x - rotation_center_x_;
  rotated_point.y = new_point.y - rotation_center_y_;
  rotated_point.z = new_point.z - rotation_center_z_;

  RotationMatrix rot_(angle_, axis_x_, axis_y_, axis_z_);
  rotated_point = rot_.Rotate(rotated_point);

  new_point.x = rotated_point.x + rotation_center_x_;
  new_point.y = rotated_point.y + rotation_center_y_;
  new_point.z = rotated_point.z + rotation_center_z_;

  *point = new_point;
}

void SceneElement::Transform(SceneElement* scene) const {
  cv::Point3d tmp;
  cv::Point3d mean_point = this->FindMeanPoint();
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

  for (int i = 0; i < sorted_mesh.vertexes().size(); i++) {
    new_scene.AddVertex(sorted_mesh.vertexes()[i]);
  }
  for (int i = 0; i < sorted_mesh.faces().size(); i++) {
    Triangle TmpFace(
      mesh.faces()[i].Point1() + shift,
      mesh.faces()[i].Point2() + shift,
      mesh.faces()[i].Point3() + shift);
    new_scene.AddFace(TmpFace);
  }

  std::vector<int> point_to_merge;
  for (int i = 0; i < new_scene.Vertexes().size(); i++)
    point_to_merge.push_back(i);

  int vert_counter = new_scene.Vertexes().size();

  for (int i = 0; (i < shift) && (vert_counter > shift); i++) {
    cv::Point3d curr_point = new_scene.Vertexes().at(i);

    int begin = shift;
    int end = new_scene.Vertexes().size();
    int left_border = BinarySearchByX(new_scene.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x - merge_error,
                                      LESS);
    int right_border = BinarySearchByX(new_scene.Vertexes(),
                                      begin,
                                      end,
                                      curr_point.x + merge_error,
                                      MORE);

    for (int j = left_border; j <= right_border; j++)
      if ((fabs(curr_point.x - new_scene.Vertexes().at(j).x)
          < merge_error)
          &&
          (fabs(curr_point.y - new_scene.Vertexes().at(j).y)
          < merge_error)
          &&
          (fabs(curr_point.z - new_scene.Vertexes().at(j).z)
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
