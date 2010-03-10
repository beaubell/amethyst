/***********************************************************************
 Amethyst-lib
  - Orientation Class Objects Implementation

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

 #include <math.h>
 #include "object.h"
 #include "orientation.h"

namespace amethyst {
namespace lib {

  Quaternion::Quaternion(const Quaternion &old) {

       w = old.w;
       x = old.x;
       y = old.y;
       z = old.z;
       }


  Quaternion::Quaternion(const Euler &old) {

       //Qx = [ cos(a/2), (sin(a/2), 0, 0)]
       //Qy = [ cos(b/2), (0, sin(b/2), 0)]
       //Qz = [ cos(c/2), (0, 0, sin(c/2))]


       Quaternion Qx( cos(old.x/2.0), (sin(old.x/2.0)), 0, 0 );
       Quaternion Qy( cos(old.y/2.0), 0, (sin(old.y/2.0)), 0 );
       Quaternion Qz( cos(old.z/2.0), 0, 0, (sin(old.z/2.0)) );

       *this = Qz * Qy * Qx;
       //*this = Qx;
       }


   //const Quaternion& Quaternion::operator = (Quaternion& right) {
   //}


   double Quaternion::length(){

       return sqrt(x * x + y * y + z * z + w * w);
       }


   void Quaternion::normalize(void) {

       double L = length();

       x /= L;
       y /= L;
       z /= L;
       w /= L;
       }

   Cartesian_Vector  Quaternion::GetVector(void) {

       return Cartesian_Vector(x,y,z);
       }


    Quaternion Quaternion::scale( const double &s)
    {
        return Quaternion(w * s, x * s, y * s, z * s);
    }

    double Quaternion::dot(const Quaternion &right) const
    {
        return (  w * right.w
                + x * right.x
                + y * right.y
                + z * right.z );
    }

    void Quaternion::slerp(const Quaternion &left, Quaternion &right, const float t)
    {
        double w1, w2;

        //if ( left.length() != 1 ) left.normalize();
        //if ( right.length() != 1 ) right.normalize();

        double cos_theta = left.dot(right);
        double theta = acos(cos_theta);
        double sin_theta = sin(theta);

        if( sin_theta > 0.001 )
        {
            w1 = sin((1.0 - t)*theta) / sin_theta;
            w2 = sin(t * theta) / sin_theta;
        }
        else
        {
            w1 = 1.0 - t;
            w2 = t;
        }

        *this = left*w1 + right*w2;
        //w = (left.w * w1 + right.w * w2);
        //x = (left.x * w1 + right.x * w2);
        //y = (left.y * w1 + right.y * w2);
        //z = (left.z * w1 + right.z * w2);
    }

    Quaternion Quaternion::operator*(double scale) const
    {
        return Quaternion(w * scale, x * scale, y * scale, z * scale);

    }

    Quaternion Quaternion::operator+ ( const Quaternion& b ) const
    {
        return Quaternion( w + b.w, x + b.x, y + b.y, z + b.z );
    }

    const Quaternion& Quaternion::operator *= (Quaternion& right)
    {
        double tmp_w = (w * right.w) - (x * right.x) - (y * right.y) - (z * right.z);
        double tmp_x = (w * right.x) + (x * right.w) + (y * right.z) - (z * right.y);
        double tmp_y = (w * right.y) + (y * right.w) + (z * right.x) - (x * right.z);
        double tmp_z = (w * right.z) + (z * right.w) + (x * right.y) - (y * right.x);

        w = tmp_w;
        x = tmp_x;
        y = tmp_y;
        z = tmp_z;

        return *this;
    }

    const Quaternion& Quaternion::operator *= (double scale)
    {
        w = w * scale;
        x = x * scale;
        y = y * scale;
        z = z * scale;
        return *this;
    }

   const Quaternion operator*(const Quaternion &left, const Quaternion &right) {

       Quaternion quat;

       quat.w = (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z);
       quat.x = (left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y);
       quat.y = (left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z);
       quat.z = (left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x);

       return quat;

   }

   const Quaternion operator*(const Quaternion &q, const Cartesian_Vector &v) {

       return  Quaternion(  -(q.x*v.x + q.y*v.y + q.z*v.z),
                              q.w*v.x + q.y*v.z - q.z*v.y,
                              q.w*v.y + q.z*v.x - q.x*v.z,
                              q.w*v.z + q.x*v.y - q.y*v.x);
       }

   const Quaternion operator*(const Cartesian_Vector &v, const Quaternion &q) {

       return  Quaternion(  -(q.x*v.x + q.y*v.y + q.z*v.z),
                              q.w*v.x + q.z*v.y - q.y*v.z,
                              q.w*v.y + q.x*v.z - q.z*v.x,
                              q.w*v.z + q.y*v.x - q.x*v.y);
       }


Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
    Quaternion t;

    t = (q * v) * q.Bar();
    return t.GetVector();
}

} // namespace lib
} // namespace amethyst
