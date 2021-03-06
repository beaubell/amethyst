#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_finalsum(const __global double3* obj_orig,
             const __global double3* obj_k1,
             const __global double3* obj_k2,
             const __global double3* obj_k3,
             const __global double3* obj_k4,
                   __global double3* obj_out
         )
{
  // Object being worked
  uint obj_id = get_global_id(0);

  // Do Final RK4 Step.
  obj_out[obj_id] = obj_orig[obj_id] + (obj_k1[obj_id] + 2.0*obj_k2[obj_id] + 2.0*obj_k3[obj_id] + obj_k4[obj_id]) / 6.0;
}