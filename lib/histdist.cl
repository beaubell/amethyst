#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void
histdist(const __global double3* obj_in,
                          uint     ref,
               __global double*  dist_out
          )
{
  // Get dimensions and where we are
  uint max_obj  = get_global_size(0);
  uint max_step = get_global_size(1);

  uint obj_id   = get_global_id(0);
  uint step_id   = get_global_id(1);

  // determine indexes to objects
  //uint ref_idx = mad24(max_obj, step_id, ref);
  uint ref_idx = obj_id;

  uint out_idx = mad24(max_obj, step_id, obj_id);
  uint in_idx = mad24(max_obj, step_id, obj_id);

  // distances!
  double dist = distance(obj_in[ref_idx], obj_in[in_idx]);

  dist_out[out_idx] = dist;
}