#pragma OPENCL EXTENSION cl_khr_fp64 : enable
//#pragma OPENCL_EXTENSION cl_amd_vec3 : enable

//#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void
rk4_reductionscale(const          uint      num_objects,
                   const          double    step_size,
                   const __global double3*  obj_acceleration_exp,
                         __global double3*  obj_velocity
                  )
{
  // Object being worked
  uint obj_id = get_global_id(0);
  uint obj_acc_index = (num_objects - 1)*obj_id;

  double3 acceleration_sum = (double3)(0.0, 0.0, 0.0);

  for (uint obj_i = 0; obj_i < (num_objects - 1); obj_i++)
  {
     acceleration_sum += obj_acceleration_exp[obj_acc_index + obj_i];
  }

  obj_velocity[obj_id] = acceleration_sum*step_size;
}
