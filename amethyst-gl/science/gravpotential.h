#ifndef GRAVPOTENTIAL_H
#define GRAVPOTENTIAL_H

#include "../amethyst-gl.h"
#include "../opengl.h"
#include "lib/cl.h"

#include <boost/signals2.hpp>

namespace amethyst::client {

class GravPotential
{
public:
    explicit GravPotential(Amethyst_GL &agl);

    void update();
    void render(const lib::Cartesian_Vector& reference);
    void render_toggle();

    void set_min(float bound);
    float get_min();
    void inc_min();
    void dec_min();

    void set_max(float bound);
    float get_max();
    void inc_max();
    void dec_max();

    float get_scaler();
    void inc_scaler();
    void dec_scaler();

    cl::Kernel kern_pot;
    cl::Program::Sources source;
    cl::Program program;
    GLint      glsl_pot;

private:
    Amethyst_GL &_amgl;
    GLdouble _potential_plane[16];
    cl::Buffer _cl_buf_plane_corners;
    
    boost::signals2::scoped_connection _scon_update;
    boost::signals2::scoped_connection _scon_inc_max;
    boost::signals2::scoped_connection _scon_dec_max;
    boost::signals2::scoped_connection _scon_inc_min;
    boost::signals2::scoped_connection _scon_dec_min;
    boost::signals2::scoped_connection _scon_inc_scaler;
    boost::signals2::scoped_connection _scon_dec_scaler;
    boost::signals2::scoped_connection _scon_render;
    
    GLuint   _texname;
    cl::ImageGL _cl_tex;

    ShaderProgram::sptr _shaderprog;
    float _shader_min;
    float _shader_max;
    float _shader_scaler;
    int   _shader_min_loc;
    int   _shader_max_loc;
    unsigned int _grid_x;
    unsigned int _grid_y;
};

} // namespace amethyst::client

#endif // GRAVPOTENTIAL_H
