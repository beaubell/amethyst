/*
   Utility Functions, Definitions
   Amethyst Physics Library (c) 2003
   Author: Beau V.C. Bellamy
*/

//
//

#include "utility.h"

#include <H5Cpp.h>

#include <fstream>
#include <iostream>
#include <vector>

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

} // namespace lib
} // namespace amethyst
