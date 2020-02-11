
#include "gravpotential.h"
#include "lib/utility.h"
#include "lib/cl.h"
#include "../opengl.h"
#include "../shaderprog.h"
#include "../global.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

#include "SDL_keycode.h"

namespace amethyst {
namespace client {

using namespace boost;

float get_seconds_elapsed(cl::Event& ev)
{
    cl_ulong start, end;
    ev.getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
    ev.getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
    return (end - start) * 1.0e-9f;
}


GravPotential::GravPotential(Amethyst_GL &amgl)
: _amgl(amgl),
  _scon_update( _amgl.input->sig_kb[SDL_SCANCODE_Z].connect(bind(&GravPotential::update,this)) ),  // Setup Keyboard Shortcuts
  _scon_inc_max( _amgl.input->sig_kb[SDL_SCANCODE_W].connect(bind(&GravPotential::inc_max,this)) ),
  _scon_dec_max( _amgl.input->sig_kb[SDL_SCANCODE_S].connect(bind(&GravPotential::dec_max,this)) ),
  _scon_inc_min( _amgl.input->sig_kb[SDL_SCANCODE_Q].connect(bind(&GravPotential::inc_min,this)) ),
  _scon_dec_min( _amgl.input->sig_kb[SDL_SCANCODE_A].connect(bind(&GravPotential::dec_min,this)) ),
  _scon_inc_scaler( _amgl.input->sig_kb[SDL_SCANCODE_E].connect(bind(&GravPotential::inc_scaler,this)) ),
  _scon_dec_scaler( _amgl.input->sig_kb[SDL_SCANCODE_D].connect(bind(&GravPotential::dec_scaler,this)) ),
  _scon_render( _amgl.sig_render_scene.connect(bind(&GravPotential::render,this, _1)) ),  // Add to Render List
  _texname(0),
  _shader_min(20.0f),
  _shader_max(29.0f),
  _shader_scaler(0.01f),
  //_shader_min(0),
  //_shader_max(3.93122e12),
  _shader_min_loc(0),
  _shader_max_loc(0),
  _grid_x(16384),
  _grid_y(16384)
{
    //std::string log = "Module: science_potplane Activating...";
    std::string log = "Module: science_potplace is broken."; //FIXME
    Global.log.add(log);
   
   auto init = std::initializer_list<double>({ 1.5e12, 1.5e12, 0, 0,
                                              -1.5e12, 1.5e12, 0, 0,
			                      -1.5e12,-1.5e12, 0, 0,
					       1.5e12,-1.5e12, 0, 0});
   std::copy(init.begin(), init.end(), _potential_plane);
 
    //_potential_plane = { 1.5e12, 0.75e12, 0,
    //                           0, 0.75e12, 0,
    //                           0,-0.75e12, 0,
    //                      1.5e12,-0.75e12, 0};
    
    std::cout << "Grid Density: " << fabs(_potential_plane[0]-_potential_plane[4])/(double)_grid_x << std::endl;

    // Setup plane bounds
    _cl_buf_plane_corners = cl::Buffer(lib::amethyst_cl_context, CL_MEM_READ_ONLY, sizeof(_potential_plane), NULL, NULL);

    // Load kernel
    kern_pot = lib::cl_loadkernel(std::string("../amethyst-gl/science/gravpotential.cl"), std::string("gravpotential"));

    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_RECTANGLE_ARB );

    // Allocate space for GL texture
    glGenTextures(1, &_texname );

    // select our current texture and initialize
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, _texname );
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_R32F, _grid_x, _grid_y, 0, GL_LUMINANCE, GL_FLOAT, NULL);

    // Make CL texture fom GL one
    try {
      _cl_tex = cl::Image2DGL( lib::amethyst_cl_context, CL_MEM_WRITE_ONLY, GL_TEXTURE_RECTANGLE_ARB, 0, _texname, NULL);
    }
    catch(cl::Error e)
    {
      std::cout << "ERROR:" << e.what() << "(" << lib::oclErrorString(e.err()) << ")" << std::endl;
      throw e;
    }

    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    glEnable( GL_TEXTURE_2D );

    //Load Shaders
    _shaderprog = std::make_shared<ShaderProgram>(std::string("fixed.vert"),std::string("colorizer.frag"));

    //Get handles to shader parameters
    _shaderprog->use();
    _shader_min_loc = glGetUniformLocation(_shaderprog->getHandle(), "data_min");
    GLenum err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glGetUniformLocation(shaderProgram, data_min): " << err << std::endl;

    _shader_max_loc = glGetUniformLocation(_shaderprog->getHandle(), "data_max");
    err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glGetUniformLocation(shaderProgram, data_max): " << err << std::endl;

    set_min(_shader_min);
    set_max(_shader_max);
    glUseProgram(0);
}

void GravPotential::update()
{
    glFinish();
    
    std::cout << "Amethyst GravPotential: Running Kernel!" << std::endl;
    Global.log.add("Generating pot plane ( " + lexical_cast<std::string>(_grid_x) + " x " + lexical_cast<std::string>(_grid_y) + " )  (" + lexical_cast<std::string>(Global.universe.object_count()) + " Objects)");
    
    int gpu_id = 0;
    cl::Event send_event, kernel_event;
    cl::CommandQueue queue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, NULL);
    std::vector<cl::Memory> mem_objects;
    std::vector<cl::Event> wait_queue;

    std::cout << "Size of Potential Plane: " << sizeof(_potential_plane) << std::endl;
    
    // Send Corner Information to GPU
    queue.enqueueWriteBuffer(_cl_buf_plane_corners, CL_FALSE, 0, sizeof(_potential_plane), _potential_plane, NULL, &send_event);
    wait_queue.push_back(send_event);

    // Aquire GL Buffer
    //FIXME mem_objects.push_back(_cl_tex);
    //FIXME queue.enqueueAcquireGLObjects(&mem_objects);

    // Set Kernel Arguments
    kern_pot.setArg(0, (unsigned int)Global.universe.object_count());
    kern_pot.setArg(1, Global.universe._cl_buf_mass);
    kern_pot.setArg(2, Global.universe._current.location);
    kern_pot.setArg(3, _cl_buf_plane_corners);
    kern_pot.setArg(4, (unsigned int)_grid_x);
    //FIXME kern_pot.setArg(5, _cl_tex);

    //Execute Kernel
    queue.enqueueNDRangeKernel(kern_pot, cl::NullRange, cl::NDRange(_grid_x,_grid_y), cl::NullRange, &wait_queue, &kernel_event);
    queue.finish();

    //Release GL Buffer
    //FIXME queue.enqueueReleaseGLObjects(&mem_objects);

    std::string msg = "Finished. Elapsed: " + lexical_cast<std::string>(get_seconds_elapsed(kernel_event)) + " (s)";
    Global.log.add(msg);
    std::cout << "Amethyst GravPotential: Kernel Finished?" << std::endl;
    //queue.finish();

}

void GravPotential::render_toggle()
{
    //_scon_render.blocked();
}

void GravPotential::render(const lib::Cartesian_Vector& reference)
{
    bool texture_smooth = false;

    //glDisable( GL_TEXTURE_2D );
    glDisable( GL_LIGHTING );
    glDisable( GL_BLEND );
    glEnable( GL_TEXTURE_RECTANGLE_ARB );

    // Switch Shader and set boounds
    _shaderprog->use();
    //set_min(_shader_min);
    //set_max(_shader_max);
    
    glBindTexture( GL_TEXTURE_RECTANGLE_ARB, _texname);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, texture_smooth?GL_LINEAR:GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, texture_smooth?GL_LINEAR:GL_NEAREST);

    glPushMatrix();
    lib::Cartesian_Vector location;
    lib::Cartesian_Vector temp = location - reference;
    // Move to object location
    glTranslated(temp.x, temp.y, temp.z);

    // Render
    glDisable(GL_CULL_FACE);
    glBegin( GL_QUADS );
    glTexCoord2d(0.0     ,0.0    ); glVertex3dv(&_potential_plane[0]);
    glTexCoord2d(_grid_x ,0.0    ); glVertex3dv(&_potential_plane[4]);
    glTexCoord2d(_grid_x ,_grid_y); glVertex3dv(&_potential_plane[8]);
    glTexCoord2d(0.0     ,_grid_y); glVertex3dv(&_potential_plane[12]);
    glEnd();
    glEnable(GL_CULL_FACE);

    

    glPopMatrix();

    glUseProgram(0);

    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    glEnable( GL_BLEND );
    glEnable( GL_LIGHTING );
    //glEnable( GL_TEXTURE_2D );
}


void GravPotential::set_min(float bound)
{
    _shaderprog->use();
    _shader_min = bound;
    glUniform1f(_shader_min_loc, _shader_min);
    GLint err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glUniform1f(shader_data_min_loc, shader_data_min)): " << err << std::endl;

    std::cout << "Set min: " << bound << std::endl;
    glUseProgram(0);
}
//float GravPotential::get_lower_bound();
void GravPotential::inc_min()
{
  set_min(_shader_min + _shader_scaler);
}
void GravPotential::dec_min()
{
  set_min(_shader_min - _shader_scaler);
}

void GravPotential::set_max(float bound)
{
    _shaderprog->use();
    _shader_max = bound;
    glUniform1f(_shader_max_loc, _shader_max);
    GLint err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glUniform1f(shader_data_max_loc, shader_data_max)): " << err << std::endl;

    std::cout << "Set max: " << bound << std::endl;
    glUseProgram(0);
}
//float GravPotential::get_higher_bound();
void GravPotential::inc_max()
{
  set_max(_shader_max + _shader_scaler);
}
void GravPotential::dec_max()
{
  set_max(_shader_max - _shader_scaler);
}

float GravPotential::get_scaler()
{
  return _shader_scaler;
}

void GravPotential::inc_scaler()
{
   _shader_scaler *= 10.0f;
}

void GravPotential::dec_scaler()
{
   _shader_scaler /= 10.0f;
}


}
}
