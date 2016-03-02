/***********************************************************************
 Amethyst-GL
  - ??? FIXME ??? implementations

 Authors (c):
 2012-2012 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "uiw_simstats.h"
#include "../global.h"
#include "../opengl.h"
#include "lib/utility.h"

namespace amethyst {
namespace client {

UIW_SimStats::UIW_SimStats(Amethyst_GL &amgl,  UI &ui)
 : UI_Window(ui, std::string("Simulation Stats")),
   _amgl(amgl),
   _tbtime(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbCOMtitle(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbCOMx(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbCOMy(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbCOMz(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbVOMtitle(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbVOMx(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbVOMy(new UI_TextBox(ui.get_font(), ui.uifont_shader)),
   _tbVOMz(new UI_TextBox(ui.get_font(), ui.uifont_shader))
{
  setPosition(glm::vec2(10.0, -200.0));
  resize(glm::vec2(520.0, 100.0));

  _tbtime->setPosition(glm::vec2(10.0,5.0));
  addWidget(_tbtime);

  {
    glm::vec2 pos(10.0f, 30.0f);

    _tbCOMtitle->setText("Center of Mass");
    _tbCOMtitle->setPosition(pos);
    addWidget(_tbCOMtitle);

    pos.y += 12.0f;
    _tbCOMx->setPosition(pos);
    addWidget(_tbCOMx);

    pos.y += 12.0f;
    _tbCOMy->setPosition(pos);
    addWidget(_tbCOMy);

    pos.y += 12.0f;
    _tbCOMz->setPosition(pos);
    addWidget(_tbCOMz);
  }
  
  {
    glm::vec2 pos(200.0f, 30.0f);

    _tbVOMtitle->setText("dr/dt Center of Mass");
    _tbVOMtitle->setPosition(pos);
    addWidget(_tbVOMtitle);

    pos.y += 12.0f;
    _tbVOMx->setPosition(pos);
    addWidget(_tbVOMx);

    pos.y += 12.0f;
    _tbVOMy->setPosition(pos);
    addWidget(_tbVOMy);

    pos.y += 12.0f;
    _tbVOMz->setPosition(pos);
    addWidget(_tbVOMz);
  }
  update();
}

void UIW_SimStats::update()
{
    std::stringstream temp;
    temp.precision(8);
    std::string temp2;
    temp <<  Global.universe._current.time;
    temp >> temp2;

    double days = Global.universe._current.time / (86400.0);
    std::string days_text;
    temp.clear();
    temp << days;
    temp >> days_text;

    std::string text = "Time  : " + temp2 + " s   Days: " + days_text;
    _tbtime->setText(text);

    /// Caluculate center of mass
    lib::Cartesian_Vector CoM(0.0, 0.0, 0.0);
    lib::Cartesian_Vector VoM(0.0, 0.0, 0.0);
    float_type total_mass = 0.0;

    Universe &uni = Global.universe;
    if(!uni.list().empty())
    {
        std::list<lib::Object::ptr>::iterator obj1 = uni.list().begin();

        do
        {

            CoM += (*obj1)->location * (*obj1)->mass;
            VoM += (*obj1)->velocity * (*obj1)->mass;
            total_mass += (*obj1)->mass;

            obj1++;
        }  while (obj1 != uni.list().end());

        CoM = CoM / total_mass;
        VoM = VoM / total_mass;

    }

    {
    std::stringstream xtemp;
    xtemp.precision(8);
    std::string xstr;
    xtemp <<  CoM.x;
    xtemp >> xstr;

    std::stringstream ytemp;
    ytemp.precision(8);
    std::string ystr;
    ytemp <<  CoM.y;
    ytemp >> ystr;

    std::stringstream ztemp;
    ztemp.precision(8);
    std::string zstr;
    ztemp <<  CoM.z;
    ztemp >> zstr;

    text = "x: " + xstr;
    _tbCOMx->setText(text);

    text = "y: " + ystr;
    _tbCOMy->setText(text);

    text = "z: " + zstr;
    _tbCOMz->setText(text);
    }

    {
    std::stringstream xtemp;
    xtemp.precision(8);
    std::string xstr;
    xtemp <<  VoM.x;
    xtemp >> xstr;

    std::stringstream ytemp;
    ytemp.precision(8);
    std::string ystr;
    ytemp <<  VoM.y;
    ytemp >> ystr;

    std::stringstream ztemp;
    ztemp.precision(8);
    std::string zstr;
    ztemp <<  VoM.z;
    ztemp >> zstr;

    text = "x: " + xstr;
    _tbVOMx->setText(text);
    text = "y: " + ystr;
    _tbVOMy->setText(text);
    text = "z: " + zstr;
    _tbVOMz->setText(text);
    }
}

} // namespace lib
} // namespace amethyst


