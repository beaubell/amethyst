#define GL_GLEXT_PROTOTYPES 1

#include "gravpotential.h"
#include "../global.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <cmath>

namespace amethyst {
namespace client {

using namespace boost;


static void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                              &infologLength);

    if (infologLength > 0)
    {
        infoLog = reinterpret_cast<char *>(malloc(infologLength));
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

float get_seconds_elapsed(cl::Event& ev)
{
    cl_ulong start, end;
    ev.getProfilingInfo(CL_PROFILING_COMMAND_END, &end);
    ev.getProfilingInfo(CL_PROFILING_COMMAND_START, &start);
    return (end - start) * 1.0e-9f;
}


GravPotential::GravPotential(Amethyst_GL &amgl)
: _amgl(amgl),
  _scon_update( _amgl.input->sig_kb_z.connect(bind(&GravPotential::update,this)) ),  // Setup Keyboard Shortcuts
  _scon_inc_max( _amgl.input->sig_kb_w.connect(bind(&GravPotential::inc_max,this)) ),
  _scon_dec_max( _amgl.input->sig_kb_s.connect(bind(&GravPotential::dec_max,this)) ),
  _scon_inc_min( _amgl.input->sig_kb_q.connect(bind(&GravPotential::inc_min,this)) ),
  _scon_dec_min( _amgl.input->sig_kb_a.connect(bind(&GravPotential::dec_min,this)) ),
  _scon_inc_scaler( _amgl.input->sig_kb_e.connect(bind(&GravPotential::inc_scaler,this)) ),
  _scon_dec_scaler( _amgl.input->sig_kb_d.connect(bind(&GravPotential::dec_scaler,this)) ),
  _scon_render( _amgl.sig_render_scene.connect(bind(&GravPotential::render,this, _1)) ),  // Add to Render List
  _texname(0),
  _shaderProgram(0),
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
    std::string log = "Module: science_potplane Activating...";
    Global.log.add(log);
    
    _potentianl_plane = { 1.5e12, 1.5e12, 0,
                         -1.5e12, 1.5e12, 0,
                         -1.5e12,-1.5e12, 0,
                          1.5e12,-1.5e12, 0};
    //_potentianl_plane = { 1.5e12, 0.75e12, 0,
    //                           0, 0.75e12, 0,
    //                           0,-0.75e12, 0,
    //                      1.5e12,-0.75e12, 0};
    
    std::cout << "Grid Density: " << fabs(_potentianl_plane[0]-_potentianl_plane[3])/(double)_grid_x << std::endl;

    // Setup plane bounds
    _cl_buf_plane_corners = cl::Buffer(lib::amethyst_cl_context, CL_MEM_READ_ONLY, sizeof(_potentianl_plane), NULL, NULL);
    
    // Load cl Kernel
    std::ifstream input; std::string code;
    input.open("/home/beau/.amethyst/kernels/gravpotential.cl", std::ifstream::in);
    while (input.good())
    {
        char c = input.get();
        code.append(1, c);
    }
    input.close();

    source = cl::Program::Sources(1, std::make_pair (code.c_str(), code.size()-1));
    program = cl::Program(lib::amethyst_cl_context, source);

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

    // select our current texture and initialize
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, _texname );
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_R32F, _grid_x, _grid_y, 0, GL_LUMINANCE, GL_FLOAT, NULL);

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

    //Load Shader
    code.clear();
    input.open("/home/beau/.amethyst/shaders/colorizer.frag", std::ifstream::in);
    while (input.good())
    {
        char c = input.get();
        code.append(1, c);
    }
    input.close();

    const char* chararr= code.c_str();
    GLhandleARB myFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    glShaderSourceARB(myFragmentShader, 1, (const char **)&chararr, NULL);

    GLint success = 0;
    glCompileShaderARB(myFragmentShader);
    glGetObjectParameterivARB(myFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myFragmentShader, 1000, NULL, infoLog);
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        return;
    }

    //Load Vertex Shader
    code.clear();
    input.open("/home/beau/.amethyst/shaders/ambient.vert", std::ifstream::in);
    while (input.good())
    {
        char c = input.get();
        code.append(1, c);
    }
    input.close();

    chararr= code.c_str();
    GLhandleARB myVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    glShaderSourceARB(myVertexShader, 1, (const char **)&chararr, NULL);

    success = 0;
    glCompileShaderARB(myVertexShader);
    glGetObjectParameterivARB(myVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myVertexShader, 1000, NULL, infoLog);
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        return;
    }
    
    _shaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(_shaderProgram, myFragmentShader);
    //glAttachObjectARB(_shaderProgram, myVertexShader);

    success = 0;

    glLinkProgramARB(_shaderProgram);
    glGetObjectParameterivARB(_shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success);
    if (!success)
    {
        printInfoLog(_shaderProgram);
    }

    success = 0;

    glValidateProgramARB(_shaderProgram);
    glGetObjectParameterivARB(_shaderProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(_shaderProgram, 1000, NULL, infoLog);
        std::cout << "Error in program validation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
    }

    glUseProgramObjectARB(_shaderProgram);
    _shader_min_loc = glGetUniformLocationARB(_shaderProgram, "data_min");
    GLenum err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glGetUniformLocationARB(shaderProgram, data_min): " << err << std::endl;

    _shader_max_loc = glGetUniformLocationARB(_shaderProgram, "data_max");
    err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glGetUniformLocationARB(shaderProgram, data_max): " << err << std::endl;

    set_min(_shader_min);
    set_max(_shader_max);
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

    std::cout << "Size of Potential Plane: " << sizeof(_potentianl_plane) << std::endl;
    
    // Send Corner Information to GPU
    queue.enqueueWriteBuffer(_cl_buf_plane_corners, CL_FALSE, 0, sizeof(_potentianl_plane), _potentianl_plane, NULL, &send_event);
    wait_queue.push_back(send_event);

    // Aquire GL Buffer
    mem_objects.push_back(_cl_tex);
    queue.enqueueAcquireGLObjects(&mem_objects);

    // Set Kernel Arguments
    kern_pot.setArg(0, (unsigned int)Global.universe.object_count());
    kern_pot.setArg(1, Global.universe._cl_buf_mass);
    kern_pot.setArg(2, Global.universe._cl_buf_location);
    kern_pot.setArg(3, _cl_buf_plane_corners);
    kern_pot.setArg(4, (unsigned int)_grid_x);
    kern_pot.setArg(5, _cl_tex);

    //Execute Kernel
    queue.enqueueNDRangeKernel(kern_pot, cl::NullRange, cl::NDRange(_grid_x,_grid_y), cl::NullRange, &wait_queue, &kernel_event);
    queue.finish();

    //Release GL Buffer
    queue.enqueueReleaseGLObjects(&mem_objects);

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
    glUseProgramObjectARB(_shaderProgram);
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
    glTexCoord2d(0.0     ,0.0    ); glVertex3dv(&_potentianl_plane[0]);
    glTexCoord2d(0.0     ,_grid_y); glVertex3dv(&_potentianl_plane[3]);
    glTexCoord2d(_grid_x ,_grid_y); glVertex3dv(&_potentianl_plane[6]);
    glTexCoord2d(_grid_x ,0.0    ); glVertex3dv(&_potentianl_plane[9]);
    glEnd();
    glEnable(GL_CULL_FACE);

    

    glPopMatrix();

    glUseProgramObjectARB(0);

    glDisable( GL_TEXTURE_RECTANGLE_ARB );
    glEnable( GL_BLEND );
    glEnable( GL_LIGHTING );
    //glEnable( GL_TEXTURE_2D );
}


void GravPotential::set_min(float bound)
{
    glUseProgramObjectARB(_shaderProgram);
    _shader_min = bound;
    glUniform1f(_shader_min_loc, _shader_min);
    GLint err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glUniform1fARB(shader_data_min_loc, shader_data_min)): " << err << std::endl;

    std::cout << "Set min: " << bound << std::endl;
    glUseProgramObjectARB(0);
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
    glUseProgramObjectARB(_shaderProgram);
    _shader_max = bound;
    glUniform1f(_shader_max_loc, _shader_max);
    GLint err = glGetError();
    if(err)
      std::cout << "OpenGL ERROR (glUniform1fARB(shader_data_max_loc, shader_data_max)): " << err << std::endl;

    std::cout << "Set max: " << bound << std::endl;
    glUseProgramObjectARB(0);
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
