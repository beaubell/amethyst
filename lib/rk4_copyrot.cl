#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
rk4_copyrot(const __global double3* obj_in,
                          uint     rowidx_in,
                          uint     ref,
                 __global double3* obj_out,
                          uint     rowidx_out
          )
{
  // Object being worked
  uint row_size = get_global_size(0);
  uint obj_id   = get_global_id(0);

  uint out_idx = mad24(row_size, rowidx_out, obj_id);
  uint in_idx = mad24(row_size, rowidx_in, obj_id);

  double phi = atan2(obj_in[ref].x, obj_in[ref].y);
  double3 new_obj = (double3)(obj_in[in_idx].x*cos(phi) - obj_in[in_idx].y*sin(phi),
                              obj_in[in_idx].x*sin(phi) + obj_in[in_idx].y*cos(phi),
                              obj_in[in_idx].z );

  // Do Final RK4 Step.
  obj_out[out_idx] = new_obj;
}