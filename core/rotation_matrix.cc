

#include "rotation_matrix.h"
#include <cmath>

namespace core {

Rotation_Matrix::Rotation_Matrix()
  : a_xx_ (1),
    a_xy_ (0),
    a_xz_ (0),
    a_yx_ (0),
    a_yy_ (1),
    a_yz_ (0),
    a_zx_ (0),
    a_zy_ (0),
    a_zz_ (1) {}


Rotation_Matrix::Rotation_Matrix(double angle, double x, double y, double z){
  if ( (x == 0) && (y == 0) && (z == 0) ) {
    Rotation_Matrix r;
    *this = r;
  }
  else {
    double Norm = sqrt (x*x + y*y + z*z);
    x = x / Norm;
    y = y / Norm;
    z = z / Norm;
    double Cos = cos (angle);
    double Sin = sin (angle);
    a_xx_ = Cos + x*x*(1 - Cos);
    a_xy_ = x*y*(1 - Cos) - z*Sin;
    a_xz_ = x*z*(1 - Cos) + y*Sin;
    a_yx_ = y*x*(1 - Cos) + z*Sin;
    a_yy_ = Cos + y*y*(1 - Cos);
    a_yz_ = y*z*(1 - Cos) - x*Sin;
    a_zx_ = z*x*(1 - Cos) - y*Sin;
    a_zy_ = z*y*(1 - Cos) + x*Sin;
    a_zz_ = Cos + z*z*(1 - Cos);
  }
}

void Rotation_Matrix::Rotate (Point3D& point){
  Point3D new_point;
  new_point.set_x (a_xx()*point.x() + a_xy()*point.y() + a_xz()*point.z());
  new_point.set_y (a_yx()*point.x() + a_yy()*point.y() + a_yz()*point.z());
  new_point.set_z (a_zx()*point.x() + a_zy()*point.y() + a_zz()*point.z());
  point = new_point;
}

Rotation_Matrix::~Rotation_Matrix() {}

} // namespace core
