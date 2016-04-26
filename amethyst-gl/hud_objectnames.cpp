/***********************************************************************
 Amethyst-GL
  - HUD Object Labels Class implementations

 Authors (c):
 2016-2016 Beau V.C. Bellamy (beau@bellamy.beau@gmail.com)
 ***********************************************************************/

#include "opengl.h"
#include "hud_objectnames.h"
#include "global.h"
#include "lib/physics.h"
#include "lib/utility.h"
#include "scene.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace amethyst {
namespace client {

HUDObjectNames::HUDObjectNames(ftgl::FTFont &font, ShaderProgramFont::sptr textshader)
 : font_(font),
   textshader_(textshader)
{
    update();
}


void HUDObjectNames::render(const Eye eye)
{
    // Iterate through list of label objects
    for(auto& label : objectlabels_)
    {
        // Lock object to verify that it is valid
        if (auto labelobj = std::get<0>(label).lock())
        {
            auto& labeltext = std::get<1>(label);

            // Get position of Object
            lib::Cartesian_Vector &reference = Global.obj_view->location;
            lib::Cartesian_Vector temp = labelobj->location - reference;

            // Get screen position of Object
            float x = Global.screen_x;
            float y = Global.screen_y;

            glm::mat4 m_proj = get_proj(eye);
            const Quaternion &attitude  = Global.obj_view->attitude;
            glm::mat4 m_view = set_camera(attitude, Global.cam_zoom, eye);

            glm::mat4 m_temp = glm::translate(m_view, glm::vec3(temp.x, temp.y, temp.z));

            glm::mat4 m_screen = m_proj * m_temp;

            glm::vec4 v_screen = m_screen * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            // If negative w, point is defintately outside viewing fustrum
            if(v_screen.w < 0.0f)
                continue;

            // Convert from Homogeneous Coords to Screen Coords
            v_screen /= v_screen.w;
            v_screen.x *= -x*0.5f;
            v_screen.y *= y*0.5f;

            // Render label
            auto m_ident = glm::mat4(1.0f);
            auto m_sview = glm::translate(m_ident, -glm::vec3(v_screen));

            glm::mat4 m_sproj = glm::ortho(-x*0.5f, +x*0.5f, y*0.5f, -y*0.5f);
            labeltext->render(m_sproj, m_sview);

            //std::cout << "renderlabel: " << labeltext->_text << " screenloc (" << v_screen.x << "," << v_screen.y << "," << v_screen.z <<  "," << v_screen.w << ")" << std::endl;
        }
    }
}

void HUDObjectNames::update()
{

    //Grab reference to universe;
    auto& uni = Global.universe;

    // Check for Object Additions (or Expired Objects)
    for(auto obj : uni._object_list)
    {
        bool found = false;

        // Does Label exist?
        for(auto& label : objectlabels_)
        {
            // In order to compare against the objects::wptr, we must lock it.
            // If it fails, the object is expired and we can therefore delete the label.
            if (auto labelobj = std::get<0>(label).lock())
            {
                if (obj == labelobj)
                {
                    found = true;
                    break;
                }
            }
            else
            {
                // Remove label object
                // FIXME, fails to compile
                //objectlabels_.remove(label);
            }
        }

        // Add Label
        if (!found)
        {
            auto labeltext = std::make_shared<UI_TextBox>(font_, textshader_);
            labeltext->setText(obj->name);
            objectlabels_.emplace_front(obj, labeltext);

            std::cout << "Added object " << obj->name << " to label list." << std::endl;
        }
    }

    // Check for title changes
    //TODO
}

} // namespace lib
} // namespace amethyst
