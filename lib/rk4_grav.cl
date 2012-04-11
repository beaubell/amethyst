#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_grav(const __global double*   masses,
         const __global double3*  obj_locations,
               __global double3*  obj_acceleration_exp
         )
{
  double BigG = 6.6738480e-11;

  size_t num_objects = get_global_size(0);
  size_t sig_objects = get_global_size(1);
  int obj1 = get_global_id(0); //object being worked
  int obj2 = get_global_id(1); //object effecting first object (I just read it's data)

  // Skip over current object
  if(obj2 != obj1)
  {
    double dist = distance(obj_locations[obj1], obj_locations[obj2]);

    double force = (BigG*masses[obj2])/pow(dist,2);

    double3 diff_vector = obj_locations[obj2] - obj_locations[obj1];
    double3 force_vector = normalize(diff_vector) * force;

    obj_acceleration_exp[(sig_objects)*obj1 + obj2] = force_vector;
  }
  else
  {
    obj_acceleration_exp[(sig_objects)*obj1 + obj2] = (double3)(0.0, 0.0, 0.0);
  }
}
