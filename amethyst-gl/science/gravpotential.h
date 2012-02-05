#ifndef GRAVPOTENTIAL_H
#define GRAVPOTENTIAL_H

#include "cl.h"
#include "../amethyst-gl.h"
#include "../opengl.h"

#include <boost/signals2.hpp>

namespace amethyst {
namespace client {

class GravPotential
{
public:
    GravPotential(Amethyst_GL &agl);

    void update();
    void render(const lib::Cartesian_Vector& reference);
    void render_toggle();

    void set_lower_bound(double bound);
    double get_lower_bound();
    void inc_lower_bound();
    void dec_lower_bound();

    void set_higher_bound(double bound);
    double get_higher_bound();
    void inc_higher_bound();
    void dec_higher_bound();

    cl::Kernel kern_pot;
    GLint      glsl_pot;

private:
    Amethyst_GL &_amgl;
    GLdouble _potentianl_plane[12];
    
    boost::signals2::scoped_connection _scon_update;
    boost::signals2::scoped_connection _scon_render;
    
    GLuint   _texname;
    cl::Image2DGL _cl_tex;
};

}
}

#endif // GRAVPOTENTIAL_H
