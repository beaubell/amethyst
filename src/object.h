/*
   Physics Object, Declarations
   Author: Beau V.C. Bellamy
*/

// The cautious seldom err.
//                 - Confucious

#ifndef OBJECT_H
#define OBJECT_H

#ifdef __GNUG__
#pragma interface
#endif

using namespace std;


typedef struct _cartesian_coord {

     double x;
     double y;
     double z;
     } Cartesian_Coord;

typedef Cartesian_Coord Cartesian_Vector;


typedef struct _spherical_coord {

     // Azimuthal angle in the xy plane from the z axis
     // radians, 0 <= a < 2PI
     double a;

     // Polar angle from the z axis
     // radians, 0 <= p <= PI
     double p;

     // radius, distance from the center
     double r;
     } Spherical_Coord;

typedef Spherical_Coord Spherical_Vector;


struct force {

     // 
     bool format;

     // Vector: direction and magnitude of applied force
     union vector {

         /* Cartesian Format */
         Cartesian_Vector     cart_v;

         /* Spherical Format */
         Spherical_Vector     sphr_v;
         };

     /* location of applied force (0,0,0 = center of gravity = no rotation) */
     Cartesian_Coord      location;
     };



class Object {

   private:

   public:
     Object();
     ~Object();

     void force_add(Cartesian_Vector force);
     void force_add(Spherical_Vector force);

     void force_clear();

     void force_apply(double time);  //time == 1 for 1/1 converstion
     
     void velocity_apply(double time); //time == 1 for 1/1 converstion

     // it is assumed that center of mass is 0.0x, 0.0y, 0.0z

     /* Absolute Mass */
     double mass;


     // location variables
       /* Absolute Location */
       Cartesian_Coord location;

       /* Absolute Velocity */
       Cartesian_Vector velocity;

       /* Absolute Acceleration */
       Cartesian_Vector acceleration;

     // attitude variables
       /* Absolute Attitude, fixme*/
       //Cartesian_Coord attitude;

       /* Absolute rotation speed, fixme */ 
       //Cartesian_Coord rotspd;


     /* Pointer to external forces (Gravity, etc) */
     //struct force *ext_forces;  // deprecated

     /* Pointer to internal forces (Main and Attitude Thrusters) */
     struct force *int_forces;

     /* Pointer to next object class, if applicable */
     Object *next;

     /* Pointer to Meta data. (like object graphics) */
     void *meta;
     };



#endif /* OBJECT_H */

