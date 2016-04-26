/***********************************************************************
 Amethyst-GL
  - HUD Orbit Class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "hud_orbit.h"
#include "global.h"
#include "lib/physics.h"
#include "lib/utility.h"
#include "scene.h"

#include "shaderprog_hud.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

HUDOrbit::HUDOrbit(ShaderProgramHUD::sptr uishader)
 : hudshader_(uishader),
   orbitpoints_(1001)
{
  const double radial_angle = 2.0*M_PI/static_cast<double>(orbitpoints_-1);

  float earthorbit[orbitpoints_][3];
  float earthorbitcol[orbitpoints_][3];
  float moonorbit[orbitpoints_][3];
  float moonorbitcol[orbitpoints_][3];

  for(unsigned int i = 0; i < orbitpoints_; i++)
  {
    double massratio = 3.0035191795284299e-06;
    double semi = 149.598e9;
    double Re = semi * (1 - massratio);
    earthorbit[i][0] = cos(static_cast<double>(i)*radial_angle)*Re;
    earthorbit[i][1] = sin(static_cast<double>(i)*radial_angle)*Re;
    earthorbit[i][2] = 0;
    earthorbitcol[i][0] = 255.0;
    earthorbitcol[i][1] = 255.0;
    earthorbitcol[i][2] = 255.0;

    moonorbit[i][0] = cos(static_cast<double>(i)*radial_angle)*384.400e6;
    moonorbit[i][1] = sin(static_cast<double>(i)*radial_angle)*384.400e6;
    moonorbit[i][2] = 0;
    moonorbitcol[i][0] = 255.0;
    moonorbitcol[i][1] = 255.0;
    moonorbitcol[i][2] = 255.0;

  }
  
  vao_[0].bind();
  
  buffer_[0].bind();
  buffer_[0].data(sizeof(earthorbit), earthorbit, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexLoc());
  glVertexAttribPointer(hudshader_->getVertexLoc(), 3, GL_FLOAT, 0, 0, 0);

  buffer_[1].bind();
  buffer_[1].data(sizeof(earthorbitcol), earthorbitcol, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexColorLoc());
  glVertexAttribPointer(hudshader_->getVertexColorLoc(), 3, GL_FLOAT, 0, 0, 0);
  
  vao_[1].bind();
  
  buffer_[2].bind();
  buffer_[2].data(sizeof(moonorbit), moonorbit, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexLoc());
  glVertexAttribPointer(hudshader_->getVertexLoc(), 3, GL_FLOAT, 0, 0, 0);

  buffer_[3].bind();
  buffer_[3].data(sizeof(moonorbitcol), moonorbitcol, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexColorLoc());
  glVertexAttribPointer(hudshader_->getVertexColorLoc(), 3, GL_FLOAT, 0, 0, 0);
  
}

HUDOrbit::~HUDOrbit()
{
}

void HUDOrbit::render(const Eye eye)
{
  
  Color FogCol(0.0f, 0.0f, 0.0f, 0.0f);

  // draw orbits
  lib::Object::sptr sol = Global.universe.object_find("Sol");
  lib::Object::sptr earth = Global.universe.object_find("Earth");

  // Place "Sol" at the origin of radials
  if (sol)
  {
    auto m_proj = get_proj(eye);
    const Quaternion &attitude  = Global.obj_view->attitude;
    glm::mat4 m_view = set_camera(attitude, Global.cam_zoom, eye);
  
    lib::Cartesian_Vector &reference = Global.obj_view->location;
    lib::Cartesian_Vector temp = sol->location - reference;
    glm::mat4 m_temp = glm::translate(m_view, glm::vec3(temp.x, temp.y, temp.z));
  
    hudshader_->use();
    hudshader_->setProjM(m_proj);
    hudshader_->setViewM(m_temp);
    hudshader_->setFogColor(FogCol);
    hudshader_->setFogStart(10.0f);
    hudshader_->setFogEnd(Global.cam_zoom*10.0);
    vao_[0].bind();
    glDrawArrays(GL_LINE_STRIP, 0, orbitpoints_);
  }

  if (earth)
  {
    auto m_proj = get_proj(eye);
    const Quaternion &attitude  = Global.obj_view->attitude;
    glm::mat4 m_view = set_camera(attitude, Global.cam_zoom, eye);
  
    lib::Cartesian_Vector &reference = Global.obj_view->location;
    lib::Cartesian_Vector temp = earth->location - reference;
    glm::mat4 m_temp = glm::translate(m_view, glm::vec3(temp.x, temp.y, temp.z));
  
    hudshader_->use();
    hudshader_->setProjM(m_proj);
    hudshader_->setViewM(m_temp);
    hudshader_->setFogColor(FogCol);
    hudshader_->setFogStart(10.0f);
    hudshader_->setFogEnd(Global.cam_zoom*10.0);
    vao_[1].bind();
    glDrawArrays(GL_LINE_STRIP, 0, orbitpoints_);
  }
  
}

} // namespace lib
} // namespace amethyst
