#ifndef ROTATION_MATRIX_H
#define ROTATION_MATRIX_H

#include "core/point3d.h"

namespace core {

class Rotation_Matrix
{
 public:
  Rotation_Matrix();
  Rotation_Matrix(double angle, double axis_x, double axis_y, double axis_z);

  void Rotate(Point3D& point);

  virtual ~Rotation_Matrix();

  double a_xx() { return a_xx_;}
  double a_xy() { return a_xy_;}
  double a_xz() { return a_xz_;}
  double a_yx() { return a_yx_;}
  double a_yy() { return a_yy_;}
  double a_yz() { return a_yz_;}
  double a_zx() { return a_zx_;}
  double a_zy() { return a_zy_;}
  double a_zz() { return a_zz_;}

 private:
  double a_xx_;
  double a_xy_;
  double a_xz_;
  double a_yx_;
  double a_yy_;
  double a_yz_;
  double a_zx_;
  double a_zy_;
  double a_zz_;

};

} // namespace core

#endif // ROTATION_MATRIX_H
