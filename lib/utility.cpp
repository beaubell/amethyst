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


void placement_SimpleOrbit(const Object &primary, Object &satellite, double distance)
{
  satellite.location = primary.location;
  satellite.location.x += distance;

  satellite.velocity = primary.velocity;
  double grav_acc = G*primary.mass/pow(distance,2);
  satellite.velocity.y += sqrt(grav_acc * distance);
}


void placement_L1(const Object &primary, const Object &satellite, Object &L1)
{

  /// Find Location of L1 Point
  Cartesian_Vector to_body1 = primary.location - satellite.location;
  double distance = to_body1.magnitude();
  to_body1.normalize();

  double distanceL1 = distance*pow(satellite.mass/(3.0*primary.mass),1.0/3.0);
  L1.location = satellite.location + to_body1*distanceL1;

  /// Find Velocity of L1 Point
  double disL1ratio = distanceL1/distance;
  L1.velocity = (primary.velocity - satellite.velocity)*disL1ratio + satellite.velocity;
  //L1.velocity = primary.velocity*(-disL1ratio) + satellite.velocity*(1.0+disL1ratio);

}

double distance_L1(const Object &primary, const Object &satellite, Object &probe)
{
  Object L1;
  placement_L1(primary, satellite, L1);
  
  return (probe.location - L1.location).magnitude();
}

} // namespace lib
} // namespace amethyst
