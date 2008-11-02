#ifndef AMETHYST_LIB_OBJECT_H
#define AMETHYST_LIB_OBJECT_H

/***********************************************************************
 Amethyst-lib
  - Physics Object Class Declarations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <string>

#include "vector.h"
#include "orientation.h"


namespace amethyst {
namespace lib {


  struct force {

       // Vector: direction and magnitude of applied force
       struct vector {

           /* Cartesian Format */
           Cartesian_Vector     cart_v;

           /* Spherical Format */
           Spherical_Vector     sphr_v;
           };

       /* location of applied force (0,0,0 = center of gravity = no rotation) */
       Cartesian_Coord      location;

       int format;
       };



  class Object {

     private:

     public:
       Object();
       Object(const Object&);
       virtual ~Object() {};

       void force_add(Cartesian_Vector force);
       void force_add(Spherical_Vector force);

       void force_clear();

       // Calulate total force to determine current acceleration
       void force_apply(void);           //

       // Calculate velocity from acceleration;
       void accel_apply(double time);    //time == 1 for 1 second

       // calculate velocity to determine actual movement
       void velocity_apply(double time); //time == 1 for 1 second

       // Apply acceleration and velocity calculations;
       void iterate(double time);

       /* Absolute Mass */
       double mass;


       // location variables
         /* Absolute Location      (in meters from the center of the UNIVERSE!)   */
         Cartesian_Coord  location;

         /* Absolute Velocity      (in m s ^ -1) */
         Cartesian_Vector velocity;

         /* Absolute Acceleration  (in m s ^ -2)   Storage Variable */
         Cartesian_Vector acceleration;

         /* Absolute Applied Force (in Newtons)    Storage Variable */
         Cartesian_Vector force;

       // attitude variables
         /* Absolute Attitude */
         Quaternion attitude;

         /* Absolute Angular Velocity */
         Quaternion angular_velocity;

         /* Absolute Angular Acceleration */
         Quaternion angular_acceleration;


       /* Pointer to external forces (Gravity, etc) */
       //struct force *ext_forces;  // deprecated

       /* Pointer to internal forces (Main and Attitude Thrusters) */
       struct force *int_forces;

       /* Pointer to next object class, if applicable */
       Object *next;

       /* Pointer to Meta data. (like object graphics) */
       void *meta;

       // let other threads know that this object is presently being modified.
       bool        lock;
       std::string name;

       Object& operator= (const Object &right);
       };

} // namespace lib
} // namespace amethyst

#endif /* OBJECT_H */

