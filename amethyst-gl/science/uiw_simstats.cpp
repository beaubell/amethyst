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

//#include <boost/accumulators/statistics/weighted_sum.hpp>
//#include <boost/accumulators/accumulators.hpp>

namespace amethyst {
namespace client {

UIW_SimStats::UIW_SimStats(Amethyst_GL &amgl,  UI &ui)
 : UI_Window(ui, std::string("Simulation Stats")),
   _amgl(amgl)
{
  position_x = 0;
  position_y = 200;
  size_x = 520;
  size_y = 100;
}


void UIW_SimStats::render()
{
    UI_Window::render();

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
    //days_text += temp2;

    std::string text = "Time  : " + temp2 + " s   Days: " + days_text;
    glPushMatrix();
      glTranslatef(position_x + 10, position_y - 24, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();

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

        //lib::print_vector("pre center of mass: ", CoM);
        //std::cout << "Total mass: " << total_mass << std::endl;

        CoM = CoM / total_mass;
        VoM = VoM / total_mass;

    }

    //lib::print_vector("center of mass: ", CoM);
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

    float ref_x = 10;
    float ref_y = -55;
    
    glPushMatrix();
      text = "Center of Mass";  
      glTranslatef(position_x + ref_x, position_y + ref_y, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "x: " + xstr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 12, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "y: " + ystr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 24, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "z: " + zstr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 36, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
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

    float ref_x = 200;
    float ref_y = -55;

    glPushMatrix();
      text = "dr/dt Center of Mass";
      glTranslatef(position_x + ref_x, position_y + ref_y, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "x: " + xstr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 12, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "y: " + ystr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 24, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    glPushMatrix();
      text = "z: " + zstr;
      glTranslatef(position_x + ref_x + 2, position_y + ref_y - 36, 0.0f);
      font.Render(text.c_str());
    glPopMatrix();
    }
}


} // namespace lib
} // namespace amethyst


