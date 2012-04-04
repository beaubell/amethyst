#pragma OPENCL EXTENSION cl_khr_fp64 : enable
//#pragma OPENCL EXTENSION cl_amd_vec3 : enable

//#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void
rk4_grav(const          uint      num_objects,
         const __global double*   masses,
         const __global double3*  obj_locations,
               __global double3*  obj_acceleration_exp
         )
{
  double BigG = 6.6738480e-11;

  int obj1 = get_global_id(0); //object being worked
  int obj2 = get_global_id(1); //object effecting first object (I just read it's data)
  int new_obj2 = 0;

  // Skip over current object
  if(obj2 >= obj1)
     new_obj2 = obj2 + 1;
  else
     new_obj2 = obj2;

  double dist = distance(obj_locations[obj1], obj_locations[new_obj2]);

  double force = (BigG*masses[new_obj2])/pow(dist,2);

  double3 diff_vector = obj_locations[new_obj2] - obj_locations[obj1];
  double3 force_vector = normalize(diff_vector) * force;

  obj_acceleration_exp[(num_objects - 1)*obj1 + obj2] = force_vector;
}
