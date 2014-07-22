#include <memory>

#include "gtest/gtest.h"

#include <cmath>
#include "core/scene_element.h"
#include "core/scene3d.h"
#include "core/rotation_matrix.h"

TEST(SceneTest, ObjectTranslationTest) {
  core::Point3D point1(1,0,0);
  core::Point3D point2(1,1,0);
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
  Line.set_rotation_center_x(point1.x());
  Line.set_rotation_center_y(point1.y());
  Line.set_rotation_center_z(point1.z());
  Line.set_scale_x(1);
  Line.set_scale_y(1);
  Line.set_scale_z(1);

  Line = Line.Transform(Line);

  ASSERT_TRUE(fabs(Line.Vertexes()[0].x() - 2) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[0].y() - 2) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[0].z() - 3) < precision);

  ASSERT_TRUE(fabs(Line.Vertexes()[1].x() - 2) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].y() - 3) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].z() - 3) < precision);
}

TEST(SceneTest, ObjectRotationTest) {
  core::Point3D point1(1,0,0);
  core::Point3D point2(3,1,-2);
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
  Line.set_rotation_center_x(point1.x());
  Line.set_rotation_center_y(point1.y());
  Line.set_rotation_center_z(point1.z());
  Line.set_scale_x(1);
  Line.set_scale_y(1);
  Line.set_scale_z(1);

  Line = Line.Transform(Line);

  ASSERT_TRUE(fabs(Line.Vertexes()[1].x() + 1)< precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].y() - 2) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].z() + 1) < precision);
}

TEST(SceneTest, ObjectScalingTest) {
  core::Point3D point1(1, 0, 0);
  core::Point3D point2(5,-4, 1);
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
  Line.set_rotation_center_x(point1.x());
  Line.set_rotation_center_y(point1.y());
  Line.set_rotation_center_z(point1.z());
  Line.set_scale_x(0.5);
  Line.set_scale_y(0.5);
  Line.set_scale_z(3);

  Line = Line.Transform(Line);

  ASSERT_TRUE(fabs(Line.Vertexes()[0].x() - 2) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[0].y() + 1) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[0].z() + 1) < precision);

  ASSERT_TRUE(fabs(Line.Vertexes()[1].x() - 4) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].y() + 3) < precision);
  ASSERT_TRUE(fabs(Line.Vertexes()[1].z() - 2) < precision);
}
