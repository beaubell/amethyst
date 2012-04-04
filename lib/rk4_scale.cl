#pragma OPENCL EXTENSION cl_khr_fp64 : enable
//#pragma OPENCL EXTENSION cl_amd_vec3 : enable

//#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void
rk4_scale(const          double    scalar,
          const __global double3*  obj_in,
                __global double3*  obj_out
         )
{
  // Object being worked
  uint obj_id = get_global_id(0);

  obj_out[obj_id] = obj_in[obj_id]*scalar;
}