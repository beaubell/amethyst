/***********************************************************************
 Amethyst-GL
  - HUD Radial Class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "hud_radial.h"
#include "global.h"
#include "lib/physics.h"
#include "lib/utility.h"
#include "scene.h"

#include "shaderprog_hud.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace amethyst {
namespace client {

HUDRadial::HUDRadial(ShaderProgramHUD::sptr uishader)
 : hudshader_(uishader)
{
  const unsigned int radials       = 365;
  const double       radial_length = 1e12;

  float  radialvectors[radials + 3][2][3];
  float  radialcolors[radials + 3][2][3];

  double radial_angle = 2.0*M_PI/static_cast<double>(radials);

  for(unsigned int i = 0; i < radials; i++)
  {
    radialvectors[i][0][0] = 0;
    radialvectors[i][0][1] = 0;
    radialvectors[i][0][2] = 0;
    radialvectors[i][1][0] = cos(static_cast<double>(i)*radial_angle)*radial_length;
    radialvectors[i][1][1] = sin(static_cast<double>(i)*radial_angle)*radial_length;
    radialvectors[i][1][2] = 0;

    radialcolors[i][0][0] = radialcolors[i][1][0] = (i == 0)?1.0f:0.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = (i == 32 || i == 60 || i == 91 || i == 121 || i == 152 || i == 182 || i == 213 || i == 244 || i == 274 || i == 305  || i == 335  )?1.0:0.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = (i != 0)?1.0f:0.0f;
  }

  lib::Object::sptr earth = Global.universe.object_find("Earth");
  lib::Object::sptr probe1 = Global.universe.object_find("S-E L1 Probe");
  lib::Object::sptr probe2 = Global.universe.object_find("S-E L2 Probe");
  lib::Object::sptr sol = Global.universe.object_find("Sol");

  // Place L1 and L2 positions
  if (probe1 && probe2 && sol && earth)
  {

    lib::Object L1_exact;
    lib::placement_L1(*sol, *earth, L1_exact);

    lib::Object L2_exact;
    lib::placement_L2(*sol, *earth, L2_exact);

    lib::Cartesian_Vector diff = L2_exact.location - sol->location;
    unsigned int i = 365;
    radialvectors[i][0][0] = 0;
    radialvectors[i][0][1] = 0;
    radialvectors[i][0][2] = 0;
    radialvectors[i][1][0] = diff.x;
    radialvectors[i][1][1] = diff.y;
    radialvectors[i][1][2] = 0;

    radialcolors[i][0][0] = radialcolors[i][1][0] = 1.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = 1.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = 1.0f;

    lib::Cartesian_Vector L1_diff = L1_exact.location - sol->location;
    lib::Cartesian_Vector probe_diff = probe1->location - sol->location;
    i = 366;
    radialvectors[i][0][0] = L1_diff.x;
    radialvectors[i][0][1] = L1_diff.y;
    radialvectors[i][0][2] = L1_diff.z;
    radialvectors[i][1][0] = probe_diff.x;
    radialvectors[i][1][1] = probe_diff.y;
    radialvectors[i][1][2] = probe_diff.z;

    radialcolors[i][0][0] = radialcolors[i][1][0] = 1.0f;
    radialcolors[i][0][1] = radialcolors[i][1][1] = 1.0f;
    radialcolors[i][0][2] = radialcolors[i][1][2] = 1.0f;

  }

  vao_.bind();
  
  buffer_[0].bind();
  buffer_[0].data(sizeof(radialvectors), radialvectors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexLoc());
  glVertexAttribPointer(hudshader_->getVertexLoc(), 3, GL_FLOAT, 0, 0, 0);

  buffer_[1].bind();
  buffer_[1].data(sizeof(radialcolors), radialcolors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(hudshader_->getVertexColorLoc());
  glVertexAttribPointer(hudshader_->getVertexColorLoc(), 3, GL_FLOAT, 0, 0, 0);
  
    /*
    std::stringstream temp;
    std::string temp2;
    temp.precision(8);

    temp << (L1_exact.location - probe1->location).magnitude();
    temp >> temp2;
    std::string text = "L1 Probe Distance: " + temp2;
    //DEPRECATED glWindowPos2i(10, 80);
    fonts[0]->Render(text.c_str());

    temp.clear();
    temp2.clear();
    temp << (L2_exact.location - probe2->location).magnitude();
    temp >> temp2;
    text = "L2 Probe Distance: " + temp2;
    //DEPRECATED glWindowPos2i(10, 67);
    fonts[0]->Render(text.c_str());
    */
}

HUDRadial::~HUDRadial()
{
}

void HUDRadial::render()
{
  //DEPRECATED glEnable(GL_FOG);

  Color FogCol(0.0f, 0.0f, 0.0f, 0.0f);

  // draw radials
  lib::Object::sptr sol = Global.universe.object_find("Sol");

  // Place "Sol" at the origin of radials
  //if (sol)
  {
    float x = Global.screen_x;
    float y = Global.screen_y;

    glm::mat4 m_proj = glm::perspective(glm::radians(30.0f), x/y, 0.1f, 10e10f);
    const Quaternion &attitude  = Global.obj_view->attitude;
    glm::mat4 m_view = set_camera(attitude, Global.cam_zoom);
  
    lib::Cartesian_Vector &reference = Global.obj_view->location;
    lib::Cartesian_Vector temp = sol->location - reference;
    glm::mat4 m_temp = glm::translate(m_view, glm::vec3(temp.x, temp.y, temp.z));
  
    hudshader_->use();
    hudshader_->setProjM(m_proj);
    hudshader_->setViewM(m_temp);
    hudshader_->setFogColor(FogCol);
    hudshader_->setFogStart(10.0f);
    hudshader_->setFogEnd(Global.cam_zoom*10.0);
    vao_.bind();
    glDrawArrays(GL_LINES, 0, 365*2+4);
  }

}

} // namespace lib
} // namespace amethyst
