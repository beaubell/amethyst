#ifndef AMETHYST_LIB_MATRIX_H
#define AMETHYST_LIB_MATRIX_H

/***********************************************************************
 Amethyst-GL
  - Matrix3x3 Class declarations

 Authors (c):
 2008-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "vector.h"

namespace amethyst {
namespace lib {

template <typename T>
class Matrix
{

    public:
        T e11, e12, e13, e21, e22, e23, e31, e32, e33;
        Matrix()
        : e11(0), e12(0), e13(0),
          e21(0), e22(0), e23(0),
          e31(0), e32(0), e33(0)
        {}
        
        Matrix(T r1c1, T r1c2, T r1c3,
               T r2c1, T r2c2, T r2c3,
               T r3c1, T r3c2, T r3c3)
        : e11(r1c1), e12(r1c2), e13(r1c3),
          e21(r2c1), e22(r2c2), e23(r2c3),
          e31(r3c1), e32(r3c2), e33(r3c3)
        {}

        Scalar det(void) const
        {
          return e11*e22*e33 -
            e11*e32*e23 +
            e21*e32*e13 -
            e21*e12*e33 +
            e31*e12*e23 -
            e31*e22*e13;
        }
        
        Matrix   transpose(void) const
	{
          return Matrix<T>(e11,e21,e31,e12,e22,e32,e13,e23,e33);
        }
        
        Matrix   inverse(void) const
        {
          T d = e11*e22*e33 -
                     e11*e32*e23 +
                     e21*e32*e13 -
                     e21*e12*e33 +
                     e31*e12*e23 -
                     e31*e22*e13;

          if (d == 0) d = 1;

          return Matrix<T>( (e22*e33-e23*e32)/d,
                           -(e12*e33-e13*e32)/d,
                            (e13*e23-e13*e22)/d,
                           -(e21*e33-e23*e31)/d,
                            (e11*e33-e13*e31)/d,
                           -(e11*e23-e13*e21)/d,
                            (e21*e32-e22*e32)/d,
                           -(e11*e32-e12*e31)/d,
                            (e11*e22-e12*e21)/d );
        }

	
	
        Matrix&  operator+=(const Matrix& m);
        Matrix&  operator-=(const Matrix& m);
        Matrix&  operator*=(const T s);
        Matrix&  operator/=(const T s);

};

/*
Matrix operator+(const Matrix& m1, const Matrix& m2);
Matrix operator-(const Matrix& m1, const Matrix& m2);
Matrix operator/(const Matrix& m,  const T s);
Matrix operator*(const Matrix& m1, const Matrix& m2);
Matrix operator*(const Matrix& m,  const T s);
Matrix operator*(const T  s,  const Matrix& m);

Cartesian_Vector operator*(const Matrix& m,  const Cartesian_Vector& v);
Cartesian_Vector operator*(const Cartesian_Vector& v, const Matrix& m);
*/
template <typename T>
Matrix<T> operator+(const Matrix<T>& m1, const Matrix<T>& m2)
{
    return Matrix<T>(m1.e11+m2.e11, m1.e12+m2.e12, m1.e13+m2.e13,
                  m1.e21+m2.e21, m1.e22+m2.e22, m1.e23+m2.e23,
                  m1.e31+m2.e31, m1.e32+m2.e32, m1.e33+m2.e33);
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& m1, const Matrix<T>& m2)
{
    return Matrix<T>(m1.e11-m2.e11, m1.e12-m2.e12, m1.e13-m2.e13,
                  m1.e21-m2.e21, m1.e22-m2.e22, m1.e23-m2.e23,
                  m1.e31-m2.e31, m1.e32-m2.e32, m1.e33-m2.e33);
}

template <typename T>
Matrix<T> operator/(const Matrix<T>& m,  const T s)
{
    return Matrix<T>(m.e11/s, m.e12/s, m.e13/s,
                  m.e21/s, m.e22/s, m.e23/s,
                  m.e31/s, m.e32/s, m.e33/s);
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m1, const Matrix<T>& m2)
{
    return Matrix<T>(m1.e11*m2.e11 + m1.e12*m2.e21 + m1.e13*m2.e31,
                  m1.e11*m2.e12 + m1.e12*m2.e22 + m1.e13*m2.e32,
                  m1.e11*m2.e13 + m1.e12*m2.e23 + m1.e13*m2.e33,
                  m1.e21*m2.e11 + m1.e22*m2.e21 + m1.e23*m2.e31,
                  m1.e21*m2.e12 + m1.e22*m2.e22 + m1.e23*m2.e32,
                  m1.e21*m2.e13 + m1.e22*m2.e23 + m1.e23*m2.e33,
                  m1.e31*m2.e11 + m1.e32*m2.e21 + m1.e33*m2.e31,
                  m1.e31*m2.e12 + m1.e32*m2.e22 + m1.e33*m2.e32,
                  m1.e31*m2.e13 + m1.e32*m2.e23 + m1.e33*m2.e33);
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& m,  const T s)
{
    return Matrix<T>(m.e11*s, m.e12*s, m.e13*s,
                  m.e21*s, m.e22*s, m.e23*s,
                  m.e31*s, m.e32*s, m.e33*s);
}

template <typename T>
Matrix<T> operator*(const T  s,  const Matrix<T>& m)
{
    return Matrix<T>(m.e11*s, m.e12*s, m.e13*s,
                  m.e21*s, m.e22*s, m.e23*s,
                  m.e31*s, m.e32*s, m.e33*s);
}

template <typename T>
Cartesian_Vector<T> operator*(const Matrix<T>& m,  const Cartesian_Vector<T>& v)
{
    return Cartesian_Vector<T>( m.e11*v.x + m.e12*v.y + m.e13*v.z,
                             m.e21*v.x + m.e22*v.y + m.e23*v.z,
                             m.e31*v.x + m.e32*v.y + m.e33*v.z);
}

template <typename T>
Cartesian_Vector<T> operator*(const Cartesian_Vector<T>& v, const Matrix<T>& m)
{
    return Cartesian_Vector<T>( m.e11*v.x + m.e12*v.y + m.e13*v.z,
                             m.e21*v.x + m.e22*v.y + m.e23*v.z,
                             m.e31*v.x + m.e32*v.y + m.e33*v.z);
}

} // namespace lib
} // namespace amethyst

#endif
