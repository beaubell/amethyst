/***********************************************************************
 Amethyst-GL
  - HUD Object Labels Class implementations

 Authors (c):
 2016-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
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


void HUDObjectNames::render(const Camera& camera, const Eye eye)
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

            PVMatrix pvm = camera.getMatrii(eye);

            glm::dmat4 m_temp = glm::translate(pvm.view, glm::dvec3(temp.x, temp.y, temp.z));

            glm::dmat4 m_screen = pvm.proj * m_temp;

            glm::dvec4 v_screen = m_screen * glm::dvec4(0.0f, 0.0f, 0.0f, 1.0f);

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

    // Remove labels for deleted objects
    objectlabels_.remove_if([](ObjectLabel &objlbl){ return std::get<0>(objlbl).expired(); });

    // Check for title changes
    //TODO
}

} // namespace lib
} // namespace amethyst
