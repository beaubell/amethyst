#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_reductionscale(const __global double3*  obj_acceleration_exp,
                   const          double    scale,
                         __global double3*  obj_velocity
                  )
{

  size_t num_objects = get_global_size(0);

  // Object being worked
  uint obj_id      = get_global_id(0);
  uint obj_acc_index = (num_objects - 1)*obj_id;

  double3 acceleration_sum = (double3)(0.0, 0.0, 0.0);

  for (uint obj_i = 0; obj_i < (num_objects - 1); obj_i++)
  {
     acceleration_sum += obj_acceleration_exp[obj_acc_index + obj_i];
  }

  obj_velocity[obj_id] = acceleration_sum*scale;
}
