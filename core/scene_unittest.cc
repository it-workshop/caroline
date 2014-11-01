// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Mlodik Mikhail <mlodik_m@mail.ru>

#include <memory>
#include <cmath>

#include "gtest/gtest.h"

#include "core/scene_element.h"
#include "core/scene3d.h"
#include "core/rotation_matrix.h"
#include "core/mesh.h"
#include "core/mesh_merge_utils.h"

TEST(SceneTest, ObjectTranslationTest) {
  cv::Point3d point1(1, 0, 0);
  cv::Point3d point2(1, 1, 0);
  core::Mesh line;
  line.AddVertex(point1);
  line.AddVertex(point2);
  core::SceneElement Line(&line);

  const double precision = 0.00001;
  Line.set_x(1);
  Line.set_y(2);
  Line.set_z(3);
  Line.set_angle(0);
  Line.set_axis_x(2);
  Line.set_axis_y(0);
  Line.set_axis_z(0);
  Line.set_rotation_center_x(point1.x);
  Line.set_rotation_center_y(point1.y);
  Line.set_rotation_center_z(point1.z);
  Line.set_scale_x(1);
  Line.set_scale_y(1);
  Line.set_scale_z(1);

  Line.Transform(&Line);

  EXPECT_TRUE(fabs(Line.Vertexes()[0].x - 2) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[0].y - 2) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[0].z - 3) < precision);

  EXPECT_TRUE(fabs(Line.Vertexes()[1].x - 2) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].y - 3) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].z - 3) < precision);
}

TEST(SceneTest, ObjectRotationTest) {
  cv::Point3d point1(1, 0, 0);
  cv::Point3d point2(3, 1, -2);
  core::Mesh line;
  line.AddVertex(point1);
  line.AddVertex(point2);
  core::SceneElement Line(&line);

  const double PI = 3.141592636;
  const double precision = 0.00001;
  Line.set_x(0);
  Line.set_y(0);
  Line.set_z(0);
  Line.set_angle(PI/2);
  Line.set_axis_x(1);
  Line.set_axis_y(2);
  Line.set_axis_z(2);
  Line.set_rotation_center_x(point1.x);
  Line.set_rotation_center_y(point1.y);
  Line.set_rotation_center_z(point1.z);
  Line.set_scale_x(1);
  Line.set_scale_y(1);
  Line.set_scale_z(1);

  Line.Transform(&Line);

  EXPECT_TRUE(fabs(Line.Vertexes()[1].x + 1)< precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].y - 2) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].z + 1) < precision);
}

TEST(SceneTest, ObjectScalingTest) {
  cv::Point3d point1(1, 0, 0);
  cv::Point3d point2(5, -4, 1);
  core::Mesh line;
  line.AddVertex(point1);
  line.AddVertex(point2);
  core::SceneElement Line(&line);

  const double precision = 0.00001;
  Line.set_x(0);
  Line.set_y(0);
  Line.set_z(0);
  Line.set_angle(0);
  Line.set_axis_x(2);
  Line.set_axis_y(0);
  Line.set_axis_z(0);
  Line.set_rotation_center_x(point1.x);
  Line.set_rotation_center_y(point1.y);
  Line.set_rotation_center_z(point1.z);
  Line.set_scale_x(0.5);
  Line.set_scale_y(0.5);
  Line.set_scale_z(3);

  Line.Transform(&Line);

  EXPECT_TRUE(fabs(Line.Vertexes()[0].x - 2) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[0].y + 1) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[0].z + 1) < precision);

  EXPECT_TRUE(fabs(Line.Vertexes()[1].x - 4) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].y + 3) < precision);
  EXPECT_TRUE(fabs(Line.Vertexes()[1].z - 2) < precision);
}

TEST(SceneTest, MeshMergingTest) {
  cv::Point3d point1(10, 0, 0);
  cv::Point3d point2(2, 1, 0);
  cv::Point3d point3(3, 0, 1);
  cv::Point3d point4(5, 0, 0);
  core::Mesh mesh;
  mesh.AddVertex(point1);
  mesh.AddVertex(point2);
  mesh.AddVertex(point3);
  mesh.AddVertex(point4);
  mesh.AddFace(core::Triangle(0, 1, 2));
  mesh.AddFace(core::Triangle(0, 1, 3));
  mesh.AddFace(core::Triangle(0, 2, 3));
  mesh.AddFace(core::Triangle(1, 2, 3));

  cv::Point3d point10(15, 0, 0);
  cv::Point3d point20(12, 1, 0);
  cv::Point3d point30(13, 0, 1);
  cv::Point3d point40(10, 0, 0);
  core::Mesh mesh0;
  mesh0.AddVertex(point10);
  mesh0.AddVertex(point20);
  mesh0.AddVertex(point30);
  mesh0.AddVertex(point40);
  mesh0.AddFace(core::Triangle(0, 1, 2));
  mesh0.AddFace(core::Triangle(0, 1, 3));
  mesh0.AddFace(core::Triangle(0, 2, 3));
  mesh0.AddFace(core::Triangle(1, 2, 3));
  core::SceneElement TestScene(&mesh);
  EXPECT_EQ(TestScene.Vertexes().size(), 4);
  core::Mesh GluedMesh = TestScene.Merge(mesh0, TestScene);
  std::vector<cv::Point3d> some_vertices;
  some_vertices = GluedMesh.vertexes();
  EXPECT_EQ(GluedMesh.vertexes().size(), 7);
  EXPECT_EQ(GluedMesh.faces().at(4).Point1(), 0);
}
