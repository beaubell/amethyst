#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_copy3d(const __global double3* obj_in,
                          uint     rowidx_in,
                 __global double3* obj_out,
                          uint     rowidx_out
          )
{
  // Object being worked
  uint row_size = get_global_size(0);
  uint obj_id   = get_global_id(0);

  uint out_idx = mad24(row_size, rowidx_out, obj_id);
  uint in_idx = mad24(row_size, rowidx_in, obj_id);

  // Do Final RK4 Step.
  obj_out[out_idx] = obj_in[in_idx];
}