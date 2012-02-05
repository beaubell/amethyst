#include "gravpotential.h"
#include "../global.h"
#include <boost/bind.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

namespace amethyst {
namespace client {

using namespace boost;
  
GravPotential::GravPotential(Amethyst_GL &amgl)
: _amgl(amgl),
  _scon_update( _amgl.input->sig_kb_a.connect(bind(&GravPotential::update,this)) ),  // Setup Keyboard Shortcuts
  _scon_render( _amgl.sig_render_scene.connect(bind(&GravPotential::render,this, _1)) ),  // Add to Render List
  _texname(0)
{
    _potentianl_plane = {2e13,2e13,0, -2e13,2e13,0, -2e13,-2e13,0, 2e13,-2e13,0};
    unsigned int grid = 1024;
    std::cout << "Grid Density: " << fabs(_potentianl_plane[0]-_potentianl_plane[3])/(double)grid << std::endl;

    // Load cl Kernel
    std::ifstream input; std::string code;
    input.open("/home/beau/.amethyst/kernels/gravpotential.cl", std::ifstream::in);
    while (input.good())
    {
        char c = input.get();
        code.append(1, c);
    }
    input.close();

    cl::Program::Sources source = cl::Program::Sources(1, std::make_pair (code.c_str(), code.size()-1));
    cl::Program program = cl::Program(lib::amethyst_cl_context, source);

    // Compile CL Kernel
    try {
       program.build(lib::cl_devices);
    }
    catch(...)
    {
       
    }
    
    std::cout << "Build_log: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(lib::cl_devices[0]);
    kern_pot = cl::Kernel(program,"gravpotential");

    glDisable( GL_TEXTURE_2D );
    glEnable( GL_TEXTURE_RECTANGLE_ARB );
    
    // Allocate space for GL texture
    glGenTextures(1, &_texname );

    // select our current texture
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, _texname );
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_LUMINANCE32F_ARB, grid, grid, 0, GL_LUMINANCE, GL_FLOAT, NULL);
    //glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA16F, grid, grid, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_R16F, grid, grid, 0, GL_LUMINANCE, GL_FLOAT, NULL);

    //clGetSupportImageFormats(lib::amethyst_cl_context)
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
}

void GravPotential::update()
{
    glFinish();
    
    std::cout << "Amethyst GravPotential: Running Kernel!" << std::endl;
    
    GLuint grid = 1024;
    int gpu_id = 0;
    cl::Event kernel_event;
    cl::CommandQueue queue(lib::amethyst_cl_context, lib::cl_devices[gpu_id], CL_QUEUE_PROFILING_ENABLE, NULL);
    std::vector<cl::Memory> mem_objects;

    mem_objects.push_back(_cl_tex);

    queue.enqueueAcquireGLObjects(&mem_objects);

    //cl::KernelFunctor run_image = kern_pot.bind(queue, cl::NDRange(N,N))

    //kern_pot.setArg(0, Global.universe._cl_buf_mass);
    //kern_pot.setArg(1, Global.universe._cl_buf_location);
    //kern_pot.setArg(2, NULL);aaa
    kern_pot.setArg(0, (unsigned int)grid);
    kern_pot.setArg(1, _cl_tex);

    //Execute Stage 1 (Phasecode) Kernel  (dont wait for events on the first run!)
    queue.enqueueNDRangeKernel(kern_pot, cl::NullRange, cl::NDRange(grid,grid), cl::NullRange, NULL, &kernel_event);

    queue.enqueueReleaseGLObjects(&mem_objects);
    queue.finish();
    std::cout << "Amethyst GravPotential: Kernel Finished?" << std::endl;

}

void GravPotential::render_toggle()
{
    //_scon_render.blocked();
}

void GravPotential::render(const lib::Cartesian_Vector& reference)
{
    bool texture_smooth = true;
    GLint grid = 1024;

    //glDisable( GL_TEXTURE_2D );
    //glDisable( GL_LIGHTING );
    glEnable( GL_TEXTURE_RECTANGLE_ARB );
    
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
    glTexCoord2d(0.0  ,0.0 ); glVertex3d(1e13,1e13,0);
    glTexCoord2d(grid ,0.0 ); glVertex3d(-1e13,1e13,0);
    glTexCoord2d(grid ,grid); glVertex3d(-1e13,-1e13,0);
    glTexCoord2d(0.0  ,grid); glVertex3d(1e13,-1e13,0);
    glEnd();
    glEnable(GL_CULL_FACE);

    // Switch Shader
    //std::cout << "Render" << std::endl;
    // Set Shader Bounds

    glPopMatrix();
    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    //glEnable( GL_TEXTURE_2D );
    //glEnable( GL_LIGHTING );
}

}
}
