/*
   Utility Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include "utility.h"
#include "physics.h"

#include <H5Cpp.h>

#include <fstream>
#include <iostream>
#include <strstream>
#include <vector>
#include <cmath>

#ifdef __GNUG__
#pragma implementation
#endif

namespace amethyst {
namespace lib{

void print_vector(const char *title, const Cartesian_Vector &vector)
{
  printf("%s CV: x:%#1.16lE y:%#1.16lE z:%#1.16lE\n", title, vector.x, vector.y, vector.z);
}

void print_vector(const char *title, const Spherical_Vector &vector)
{
  printf("%s SV: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title, vector.a, vector.p, vector.r);
}

void print_vector(const char *title, const Quaternion &vector)
{
  printf("%s QU: a:%#1.16lE p:%#1.16lE r:%#1.16lE\n", title, vector.x, vector.y, vector.z);
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

void readTextFile(const std::string& filename, std::string& in_buffer)
{
  std::ifstream input;

  in_buffer.clear();

  input.open(filename.c_str(), std::ifstream::in);
  while (input.good())
  {
    char c = input.get();
    in_buffer.append(1, c);
  }
  input.close();
}

// Writes a vector to an HDF5 File
void dumpVectorHDF5(const std::string &filename, const std::vector<Cartesian_Vector> &vec_in)
{
  // Open File for Writing
  H5::H5File h5file( filename.c_str(), H5F_ACC_TRUNC );

  // Create group
  h5file.createGroup(std::string("/dump"));

  // Prepare and write range data
  //h5file.write_2D_double(std::string("/dump/vector"), _time);

  const hsize_t rank = 2;
  hsize_t dimsf[rank];
  dimsf[0] = vec_in.size();
  dimsf[1] = 4;

  // Create dataspace
  H5::DataSpace dataspace( rank, dimsf );

  // Define Datatype
  H5::FloatType datatype( H5::PredType::NATIVE_DOUBLE );
  datatype.setOrder( H5T_ORDER_LE);;

  // Create a new dataset within the file...
  H5::DataSet dataset = h5file.createDataSet( "/dump/vector", datatype, dataspace);

    // Write data
  dataset.write(&vec_in[0], H5::PredType::NATIVE_DOUBLE);

  h5file.close();
  return;
}

void gen_model_solarsystem(Universe &uni)
{
    lib::Object *sol, *earth, *moon, *merc, *v, *p1, *p2, *mars, *jup, *sat, *ura, *nep;
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
  lib::placement_SimpleOrbit(*sol, *earth, 149.598e9);

  moon = uni.object_find("Moon");
  //moon->mass = 7.35e22;    // PDR 2002
  moon->mass = 1;
  moon->location.clear();
  moon->velocity.clear();
  lib::placement_SimpleOrbit(*earth, *moon, 384.400e6);

  p1 = uni.object_find("S-E L1 Probe");
  p1->mass = 1;
  lib::placement_L1(*sol, *earth, *p1);
  //p1->location.x = p1->location.x + 1e6;

  Object * SEl2 = uni.object_find("S-E L2 Probe");
  SEl2->mass = 1;
  lib::placement_L2(*sol, *earth, *SEl2);

  p2 = uni.object_find("E-M L1 Probe");
  p2->mass = 1;
  lib::placement_L1(*earth, *moon, *p2);

  print_object(*p1);
  print_object(*SEl2);

  

  merc = uni.object_find("Mercury");
  //merc->mass = 3.3038e23;    // PDR 2002
  merc->mass = 1;
  merc->location.clear();
  merc->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *merc, 57.909e9);

  v = uni.object_find("Venus");
  //v->mass = 4.8691e24;    // PDR 2002
  v->mass = 1;
  v->location.clear();
  v->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *v, 108.209e9);

  mars = uni.object_find("Mars");
  //mars->mass = 6.4164e23;    // PDR 2002
  mars->mass = 1;
  mars->location.clear();
  mars->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *mars, 227.939e9);

  jup = uni.object_find("Jupiter");
  //jup->mass = 1.8992e27;    // PDR 2002
  jup->mass = 1;
  jup->location.clear();
  jup->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *jup, 778.298e9);

  sat = uni.object_find("Saturn");
  sat->mass = 1;
  sat->location.clear();
  sat->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *sat, 1429.394e9);

  ura = uni.object_find("Uranus");
  ura->mass = 1;
  ura->location.clear();
  ura->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *ura, 2875.039e9);

  nep = uni.object_find("Neptune");
  nep->mass = 1;
  nep->location.clear();
  nep->velocity.clear();
  lib::placement_SimpleOrbit(*sol, *nep, 4504.450e9);

  Object *solmercl1 = uni.object_find("S-Merc L1 Probe");
  lib::placement_L1(*sol, *merc, *solmercl1);

  Object *solmarsl1 = uni.object_find("S-Mars L1 Probe");
  lib::placement_L1(*sol, *mars, *solmarsl1);


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
  
}


void gen_object_variation(Universe &uni,
                          const Object &source,
                          const Cartesian_Vector &r_stepsize,
                          const Cartesian_Vector &v_stepsize,
                          const uint r_steps[3],
                          const uint v_steps[3],
                          const int  r_stepoff[3],
                          const int  v_stepoff[3])
{
  const uint x_steps = r_steps[0];
  const uint y_steps = r_steps[1];
  const uint z_steps = r_steps[2];

  const uint vx_steps = v_steps[0];
  const uint vy_steps = v_steps[1];
  const uint vz_steps = v_steps[2];
  
  for(int xi = 0; xi < x_steps; xi++)
  {
    double x = source.location.x + double(xi+r_stepoff[0])*r_stepsize.x;
    for(int yi = 0; yi < y_steps; yi++)
    {
      double y = source.location.y + double(yi+r_stepoff[1])*r_stepsize.y;
      for(int zi = 0; zi < z_steps; zi++)
      {
        double z = source.location.z + double(zi+r_stepoff[2])*r_stepsize.z;
        for(int vxi = 0; vxi < vx_steps; vxi++)
        {
          double vx = source.velocity.x + double(vxi+v_stepoff[0])*v_stepsize.x;
          for(int vyi = 0; vyi < vy_steps; vyi++)
          {
            double vy = source.velocity.y + double(vyi+v_stepoff[1])*v_stepsize.y;
            for(int vzi = 0; vzi < vz_steps; vzi++)
            {
              double vz = source.velocity.z + double(vzi+v_stepoff[2])*v_stepsize.z;

              // Create new object
              Object *obj = new Object(source);

              // Copy Source Data
              //(*Object) = source;

              obj->location.x = x;
              obj->location.y = y;
              obj->location.z = z;

              obj->velocity.x = vx;
              obj->velocity.y = vy;
              obj->velocity.z = vz;
              std::strstream ss;
              std::string str;
              ss << xi << "-" << yi << "-" << zi << ":" << vxi << "-" << vyi << "-" << vzi;
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
