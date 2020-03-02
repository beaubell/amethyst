#pragma once
/***********************************************************************
 Amethyst-lib
  - Physics Object Class Declarations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "vector.h"
#include "orientation.h"
#include "matrix.h"
#include "rendermodel.h"

#include "yaml-cpp/node/node.h"

#include <string>
#include <memory>

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

       typedef std::shared_ptr<Object> sptr;
       typedef std::weak_ptr<Object>   wptr;

       void force_add(const Cartesian_Vector &force);
       void force_add(const Spherical_Vector &force);

       void force_clear();

       // Calulate total force to determine current acceleration
       void force_apply(void);           //

       // Calculate velocity from acceleration;
       void accel_apply(const double &time);    //time == 1 for 1 second

       // calculate velocity to determine actual movement
       void velocity_apply(const double &time); //time == 1 for 1 second

       // Apply acceleration and velocity calculations;
       virtual void iterate(const double &time);

       virtual YAML::Node toYAML();
       virtual void       fromYAML(YAML::Node);
       
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
       //struct force *int_forces; //deprecated

       /* Virtual Class Pointer to hold rendering functions */
       RenderModel::sptr model;

       // let other threads know that this object is presently being modified.
       bool        lock;
       std::string name;

       Object& operator= (const Object &right);
       };

} // namespace lib
} // namespace amethyst
