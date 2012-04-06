#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_sum(const __global double3* obj_orig,
        const __global double3* obj_k,
              __global double3* obj_out
       )
{
  // Object being worked
  uint obj_id = get_global_id(0);

  // Do Final RK4 Step.
  obj_out[obj_id] = obj_orig[obj_id] + obj_k[obj_id];
}