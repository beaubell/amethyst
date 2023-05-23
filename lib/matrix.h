#ifndef AMETHYST_LIB_MATRIX_H
#define AMETHYST_LIB_MATRIX_H

/***********************************************************************
 Amethyst-GL
  - Matrix3x3 Class declarations

 Authors (c):
 2008-2023 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "vector.h"

namespace amethyst::lib {

class Matrix
{

  public:
    double e11, e12, e13, e21, e22, e23, e31, e32, e33;
    Matrix();
    Matrix(double r1c1, double r1c2, double r1c3,
           double r2c1, double r2c2, double r2c3,
           double r3c1, double r3c2, double r3c3);

    [[nodiscard]] double   det() const ;
    [[nodiscard]] Matrix   transpose() const;
    [[nodiscard]] Matrix   inverse() const;

    Matrix&  operator+=(const Matrix& m);
    Matrix&  operator-=(const Matrix& m);
    Matrix&  operator*=(const double s);
    Matrix&  operator/=(const double s);

};

Matrix operator+(const Matrix& m1, const Matrix& m2);
Matrix operator-(const Matrix& m1, const Matrix& m2);
Matrix operator/(const Matrix& m,  const double s);
Matrix operator*(const Matrix& m1, const Matrix& m2);
Matrix operator*(const Matrix& m,  const double s);
Matrix operator*(const double  s,  const Matrix& m);

Cartesian_Vector operator*(const Matrix& m,  const Cartesian_Vector& v);
Cartesian_Vector operator*(const Cartesian_Vector& v, const Matrix& m);

} // namespace amethyst::lib

#endif
