//#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#pragma OPENCL EXTENSION cl_amd_fp64 : enable

__kernel void
gravpotential(               uint      num_objects,
              const __global double*   masses,
              const __global double3*  obj_locations,
              const __global double*   square,
                             uint      grid,
              __write_only   image2d_t image_data
              )
{
  int x = get_global_id(0);
  int y = get_global_id(1);

  double BigG = 6.6738480e-10;
  double x_grid_frac = (double)(x)/(double)(grid);
  double y_grid_frac = (double)(y)/(double)(grid);

  //double3 top_edge = mix(square[0],square[1],x_grid_frac);
  double3 top_edge;
  top_edge.x = mix(square[0],square[3],x_grid_frac);
  top_edge.y = mix(square[1],square[4],x_grid_frac);
  top_edge.z = mix(square[2],square[5],x_grid_frac);

  //double3 bot_edge = mix(square[9],square[6],x_grid_frac);
  double3 bot_edge;
  bot_edge.x = mix(square[9],square[6],x_grid_frac);
  bot_edge.y = mix(square[10],square[7],x_grid_frac);
  bot_edge.z = mix(square[11],square[8],x_grid_frac);

  double3 pixel_location = mix(top_edge,bot_edge,y_grid_frac);

  double potential = 0;
 
  for (uint obj_i = 0; obj_i < num_objects; obj_i++)
  {
     double dist = distance(obj_locations[obj_i],pixel_location);
     potential += (BigG*masses[obj_i])/dist;
  }

  int2 coord = (int2)(x,y);

  float4 color = (float4)((float)(potential),0.0f ,0.0f,0.0f);
  write_imagef(image_data, coord, color);
} 

