/***********************************************************************
 Amethyst Physics Library
  -  Utility Functions, Implementation

 Authors (c):
 2003-2020 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "utility.h"
#include "physics.h"

#include <H5Cpp.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include <boost/lexical_cast.hpp>

namespace amethyst {
namespace lib{

void print_vector(const std::string &title, const Cartesian_Vector &vector)
{
  printf("%s CV: x:%#1.16lE y:%#1.16lE z:%#1.16lE\n", title.c_str(), vector.x, vector.y, vector.z);
}

void print_vector(const std::string &title, const Spherical_Vector &vector)
{
  printf("%s SV: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title.c_str(), vector.a, vector.p, vector.r);
}

void print_vector(const std::string &title, const Quaternion &vector)
{
  printf("%s QU: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title.c_str(), vector.x, vector.y, vector.z);
}


void print_object(const Object &obj)
{
  printf("Object Summary: %s\n", obj.name.c_str());
  printf(" Mass         : %#1.16lE\n", obj.mass);
  print_vector(" Location     :", obj.location);
  print_vector(" Velocity     :", obj.velocity);
  print_vector(" Acceleration :", obj.acceleration);
  print_vector(" Force        :", obj.force);

}


void gen_model_solarsystem(Universe &uni)
{
  lib::Object::sptr sol, earth, moon, merc, v, p1, p2, mars, jup, sat, ura, nep;
  sol = uni.object_find("Sol");
  if(sol != NULL)
  {
    sol->mass = 1.9891e30;  // AIP PDR 2002
    sol->location.clear();
    sol->velocity.clear();
  }

  earth = uni.object_find("Earth");
  earth->mass = 5.9743e24;  // PDR 2002
  earth->location.clear();
  earth->velocity.clear();

  if (1)
  {
  double massratio = earth->mass/sol->mass;
  double semi = 149.598e9;
  double Re = semi * (1 - massratio);
  printf("Sun Earth mass ratio: %f",massratio);

  
  sol->location.x = -Re*(massratio)/(1-massratio);
  earth->location.x = semi;

  
  double omega = sqrt(G*(sol->mass) * (1 + massratio)/pow(Re,3));
  sol->velocity.y   += -omega * Re * (massratio)/ (1 - massratio);
  earth->velocity.y +=  omega * Re / (1 - massratio);
  }
  else
  {
    lib::placement_SimpleOrbit(*sol, *earth, 149.598e9);
  }
  

  moon = uni.object_find("Moon");
  moon->mass = 7.35e22;    // PDR 2002
  //moon->mass = 1;
  moon->location.clear();
  moon->velocity.clear();
  lib::placement_SimpleOrbit(*earth, *moon, 384.400e6);

  p1 = uni.object_find("S-E L1 Probe");
  p1->mass = 1;
  lib::placement_L1(*sol, *earth, *p1);
  //p1->location.x = p1->location.x + 1e6;

#if 0
  //create halo orbit
  lib::Object::ptr haloorbiter = lib::Object::ptr(new lib::Object);
  haloorbiter->mass = 1;
  haloorbiter->name = std::string("Halo Orbiter");
  haloorbiter->location = p1->location + Cartesian_Vector(0.0,100e3,0.0);
  haloorbiter->velocity = p1->velocity + Cartesian_Vector(0.0,0.00,-0.04);
  uni.object_add(haloorbiter);

  uint r_step[3] = {1,1,1};
  uint v_step[3] = {1,1,15};
  //int r_offset[3] = {-11530,0,0}; // 1.0 stepsize
  int r_offset[3] = {-115357,0,0};
  int v_offset[3] = {0,0,-544330};

  lib::gen_object_variation(uni,
                         *haloorbiter,
                         lib::Cartesian_Vector(0.1, 5.0, 5.0),
                         lib::Cartesian_Vector(0.01, 0.0001, -0.00000005),
                         r_step, v_step,
                         r_offset, v_offset);
#endif

#if 0
  for (uint i = 0; i < 500; i++)
  {
    lib::Object::ptr L1_line = lib::Object::ptr(new lib::Object);

    /// Find Location of L1 Point
    Cartesian_Vector to_body1 = sol->location - earth->location;
    double distance = to_body1.magnitude();
    to_body1.normalize();

    double massratio = sol->mass/earth->mass;
    double bary = distance/(1 + massratio);

    double distanceL1 = (distance-bary)*cbrt(earth->mass/(3.0*(sol->mass))) -5000e3  - 3040 -36.48 -0.20608 -0.00076288 - (double)(i) * 0.00000256;
    //double distanceL1 = (distance-bary)*cbrt(earth->mass/(3.0*(sol->mass)))  -5000e3 -7.52e3 -0.03656e3 -(double)(i) * 0.00000016e3;

    L1_line->location = earth->location + to_body1*distanceL1;

    /// Find Velocity of L1 Point
    double disL1ratio = distanceL1/(distance);
    L1_line->velocity = (sol->velocity - earth->velocity)*disL1ratio + earth->velocity;
    //L1.velocity = primary.velocity*(-disL1ratio) + satellite.velocity*(1.0+disL1ratio);

    L1_line->name = boost::lexical_cast<std::string>(i);
    uni.object_add(L1_line);
  }
#endif
    
    Object::sptr SEl2 = uni.object_find("S-E L2 Probe");
    SEl2->mass = 1;
    lib::placement_L2(*sol, *earth, *SEl2);

    p2 = uni.object_find("E-M L1 Probe");
    p2->mass = 1;
    lib::placement_L1(*earth, *moon, *p2);

    print_object(*p1);
    print_object(*SEl2);
    

  

  merc = uni.object_find("Mercury");
  merc->mass = 3.3038e23;    // PDR 2002
  //merc->mass = 1;
  merc->location.clear();
  merc->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *merc, 57.909e9);

  v = uni.object_find("Venus");
  v->mass = 4.8691e24;    // PDR 2002
  //v->mass = 1;
  v->location.clear();
  v->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *v, 108.209e9);

  mars = uni.object_find("Mars");
  mars->mass = 6.4164e23;    // PDR 2002
  //mars->mass = 1;
  mars->location.clear();
  mars->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *mars, 227.939e9);

  jup = uni.object_find("Jupiter");
  jup->mass = 1.8992e27;    // PDR 2002
  //jup->mass = 1;
  jup->location.clear();
  jup->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *jup, 778.298e9);

  sat = uni.object_find("Saturn");
  sat->mass = 5.683e26;
  sat->location.clear();
  sat->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *sat, 1429.394e9);

  ura = uni.object_find("Uranus");
  ura->mass = 8.681e25;
  ura->location.clear();
  ura->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *ura, 2875.039e9);

  nep = uni.object_find("Neptune");
  nep->mass = 1.024e26;
  nep->location.clear();
  nep->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *nep, 4504.450e9);

  Object::sptr solmercl1 = uni.object_find("S-Merc L1 Probe");
  //lib::placement_L1(*sol, *merc, *solmercl1);

  Object::sptr solmarsl1 = uni.object_find("S-Mars L1 Probe");
  //lib::placement_L1(*sol, *mars, *solmarsl1);

#if 0
  uni.object_del(merc);
  uni.object_del(v);
  uni.object_del(mars);
  uni.object_del(moon);
  uni.object_del(p2);
  uni.object_del(jup);
  uni.object_del(sat);
  uni.object_del(nep);
  uni.object_del(ura);
  uni.object_del(solmercl1);
  uni.object_del(solmarsl1);
#endif

}


void gen_object_variation(Universe &uni,
                          const Object &source,
                          const Cartesian_Vector &r_stepsize,
                          const Cartesian_Vector &v_stepsize,
                          const unsigned int r_steps[3],
                          const unsigned int v_steps[3],
                          const int  r_stepoff[3],
                          const int  v_stepoff[3])
{
  const auto x_steps = r_steps[0];
  const auto y_steps = r_steps[1];
  const auto z_steps = r_steps[2];

  const auto vx_steps = v_steps[0];
  const auto vy_steps = v_steps[1];
  const auto vz_steps = v_steps[2];
  
  for(unsigned int xi = 0; xi < x_steps; xi++)
  {
    double x = source.location.x + double(xi+r_stepoff[0])*r_stepsize.x;
    for(unsigned int yi = 0; yi < y_steps; yi++)
    {
      double y = source.location.y + double(yi+r_stepoff[1])*r_stepsize.y;
      for(unsigned int zi = 0; zi < z_steps; zi++)
      {
        double z = source.location.z + double(zi+r_stepoff[2])*r_stepsize.z;
        for(unsigned int vxi = 0; vxi < vx_steps; vxi++)
        {
          double vx = source.velocity.x + double(vxi+v_stepoff[0])*v_stepsize.x;
          for(unsigned int vyi = 0; vyi < vy_steps; vyi++)
          {
            double vy = source.velocity.y + double(vyi+v_stepoff[1])*v_stepsize.y;
            for(unsigned int vzi = 0; vzi < vz_steps; vzi++)
            {
              double vz = source.velocity.z + double(vzi+v_stepoff[2])*v_stepsize.z;

              // Create new object
              Object::sptr obj = std::make_shared<Object>(source);

              // Copy Source Data
              //(*Object) = source;

              obj->location.x = x;
              obj->location.y = y;
              obj->location.z = z;

              obj->velocity.x = vx;
              obj->velocity.y = vy;
              obj->velocity.z = vz;
              std::stringstream ss;
              std::string str;
              ss << xi + r_stepoff[0] << "," << yi + r_stepoff[1]<< "," << zi + r_stepoff[2] << ":"
                 << vxi + v_stepoff[0] << "," << vyi + v_stepoff[1]<< "," << vzi + v_stepoff[2];
              ss >> str;
              obj->name = str;

              uni.object_add(obj);
              
            }
          }
        }
      }
    }
  }


}


} // namespace lib
} // namespace amethyst
