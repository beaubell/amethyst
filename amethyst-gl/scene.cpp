/***********************************************************************
 Amethyst-GL
  - Scene rendering function implementations

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/


#include <list>
#include <math.h>

#include "lib/vector.h"
#include "lib/orientation.h"
#include "lib/object.h"
#include "lib/ship.h"

#include "global.h"
#include "opengl.h"
#include "stars.h"
#include "hud.h"
#include "model.h"

#include "scene.h"

#include "yaml-cpp/yaml.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <iostream>

namespace amethyst {
namespace client {

//using lib::Object;
using lib::Cartesian_Vector;

double sun_rot = 0;

Scene::Scene() {
    //TODO
}

Scene::~Scene() {
    //TODO
}

// Called to draw scene
void
Scene::render(const Eye eye)
{
  // Get Gobal State
  const Cartesian_Vector &reference = Global.obj_view->location;
  const Quaternion       &attitude  = Global.obj_view->attitude;

  camera_.setScreen(Screen(Global.screen_x, Global.screen_y));

  //Stars
  {
    // Set camera position without respect to camera zoom-out so that stars appear far away.
    auto tempdist = camera_.getDistance();
    camera_.setDistance(1.0);
    camera_.setAttitude(attitude);
    PVMatrix pvm = camera_.getMatrii(eye);

    // Render Stars
    stars_render(pvm.proj, pvm.view);

    // Resore camera distance
    camera_.setDistance(tempdist);
  }

  // Get PV matrix set
  PVMatrix pvm = camera_.getMatrii(eye);
  glm::mat4 m_model = glm::mat4(1);

#if 0
  //Draw Sun
  glPushMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    //Move to ref position
    {
      Cartesian_Vector sun = Cartesian_Vector(100.0, 100.0, -100000.0);
      Cartesian_Vector temp = sun - reference;
      glTranslated(temp.x, temp.y, temp.z);

     //Lights
      GLfloat lightPos[] = {(float)temp.x, (float)temp.y, (float)temp.z, 1.0f };
      glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    }

    //Rotate planet on axis
    //glRotated(sun_rot, 0.0, -1.0, 0.0);
    //sun_rot = sun_rot + 0.01;
    // glDisable(GL_COLOR_MATERIAL);
    //glCallList(Global.sun_mdl);
    //glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
  glPopMatrix();
#endif

  /// FIXME Special case for Sol 
  Scene_Object::sptr sol = std::dynamic_pointer_cast<Scene_Object>(Global.universe.object_find("Sol"));
  if (sol != NULL)
  {
    Cartesian_Vector temp = sol->location - reference;

    glm::mat4 m_sun = glm::translate(m_model, glm::vec3(temp.x, temp.y, temp.z));
    sol->render(pvm.proj, pvm.view, m_sun);
  }

  // Draw Objects in List.
  if(!object_list_.empty())
  {
    auto obj1 = object_list_.begin();
    do
    {
        if(sol != *obj1 && (*obj1)->model)
        {
            Cartesian_Vector temp = (*obj1)->location - reference;
            glm::mat4 m_mdlref = glm::translate(m_model, glm::vec3(temp.x, temp.y, temp.z));
            (*obj1)->model->render(pvm.proj, pvm.view, m_mdlref);
        }
        obj1++;
    }  while (obj1 != object_list_.end());

  }

#if 0
  // Draw Network Objects
  for (int i = 0; i < Global.net_ships; i++)
  {
    glPushMatrix();
      Cartesian_Vector net_p = Global.net_ship[i].location - reference;
      //Move to ship position
      glTranslated(net_p.x, net_p.y, net_p.z);
      Quaternion       *net_q = &Global.net_ship[i].attitude;

      double theta = 2.0 * acos(net_q->w);
      TODEG(theta);

      //Rotate Ship
      glRotated(theta, net_q->x, net_q->y, net_q->z);
      {
        GLfloat fDiffLight[] =  { 1.0f, 1.0f, 0.0f };  // Green!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      //glCallList(Global.dlShip);  // FIXME XXX NETWORK OBJECTS NEED A MODEL

    glPopMatrix();
  }
#endif

}


void
Scene::add_object(lib::Object::sptr newobject)
{
    if (newobject)
      object_list_.push_back(newobject);

}


void
Scene::select_object_next()
{
    lib::Object::sptr &selected = Global.obj_view;            // Reference to ship pointer
    Scene_Object::sptr &reference = Global.reference_object; // Pointer to reference_object

    if(!object_list_.empty())
    {
        auto obj1 = Global.universe.list().begin();

        if(reference == selected)
        {
            selected = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(selected == *obj1)
            {
                obj1++;
                if (obj1 == Global.universe.list().end())
                {
                    obj1 = Global.universe.list().begin();
                    selected = *obj1;
                }
                else
                    selected = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.universe.list().end());

    } else
    {
        selected = reference;
    }

}


void
Scene::target_object_next()
{
    lib::Object::sptr& target   = Global.obj_target;            // Reference to target pointer
    Scene_Object::sptr& reference = Global.reference_object; // Pointer to reference_object

    if(!object_list_.empty())
    {
        auto obj1 = Global.universe.list().begin();

        if(reference == target)
        {
            target = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(target == *obj1)
            {
                obj1++;
                if (obj1 == Global.universe.list().end())
                {
                    obj1 = Global.universe.list().begin();
                    target = *obj1;
                }
                else
                    target = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.universe.list().end());

    } else
    {
        target = reference;
    }

}

void
Scene::control_ship_next()
{
    lib::Ship::sptr& control  = Global.ship;            // Reference to control ship
    Scene_Ship::sptr& reference = Global.reference_ship; // Pointer to reference_ship

    if(!object_list_.empty())
    {
        auto obj1 = Global.ships.begin();

        if(reference == control)
        {
            control = *obj1;
            return;
        }

        do
        {
            // Find Object
            if(control == *obj1)
            {
                obj1++;
                if (obj1 == Global.ships.end())
                {
                    //obj1 = Global.ships.begin();
                    //control = get_pointer(*obj1);
                    control = reference;
                }
                else
                    control = *obj1;
                return;
            }

            obj1++;
        }  while (obj1 != Global.ships.end());

    } else
    {
        control = reference;
    }

}

void
Scene::toYAMLFile(const std::string& fn) {
    
    using namespace YAML;
    
    Node scene;
    scene["name"] = fn;
    Node client;
    client["selected"] = Global.obj_view->name;
    client["camera"] = camera_.toYAML();
    
    scene["client"] = client;
    
    for (auto& obj: Global.universe.list()) {

        // FIXME, Dont use model name to filter ships.
        if (obj->model->getName() != "Ship") {
            scene["objects"].push_back(obj->toYAML());
        }
    }

    for (auto& [name, obj]: points_) {
        scene["points"].push_back(obj->toYAML());
    }

    for (auto& obj: Global.ships) {
        scene["ships"].push_back(obj->toYAML());
    }
    
    Node root;
    root["scene"] = scene;
    
    std::string filename(Global.dir_scene + "scn_" + fn + ".yaml");
    std::ofstream fout(filename);
    if (fout.is_open()) {
        std::cout << "Saving " << filename << "." << std::endl;
        fout << root;
    } else {
        std::cout << "FAILURE: Unable to open " << filename << " for output." << std::endl;
    }

    
}


void
Scene::fromYAML(const YAML::Node& scene) {

    using namespace YAML;

    if (scene["name"].IsScalar()) {
        //TODO: set name
    }

    std::string selected_object;
    Node client = scene["client"];
    if (client.IsMap()) {

        if (client["selected"].IsScalar())
            selected_object = client["selected"].as<std::string>();

        if (client["camera"].IsMap())
            camera_.fromYAML(client["camera"]);

    }

    // Loop through objects
    Node objects = scene["objects"];
    if (objects.IsSequence()) {
        for (auto yobj: objects) {
            Scene_Object::sptr temp = std::make_shared<Scene_Object>();
            temp->fromYAML(yobj);
            Global.universe.object_add(temp);
            std::cout << "Object: " << temp->name << " added." << std::endl;
            add_object(temp);
        }
    }

    // Loop through points
    Node points = scene["points"];
    if (points.IsSequence()) {
        for (auto yobj: points) {
            Scene_Ship::sptr temp = std::make_shared<Scene_Ship>();
            temp->fromYAML(yobj);
            points_[temp->name] = temp;
        }
    }


    // Loop through ships
    Node ships = scene["ships"];
    if (ships.IsSequence()) {
        for (auto yobj: ships) {
            Scene_Ship::sptr temp = std::make_shared<Scene_Ship>();
            temp->fromYAML(yobj);
            Global.universe.object_add(temp);
            Global.ships.insert(temp);
            add_object(temp);
        }
    }

    // Search for Selected Object
    //Find Player and set
    Global.obj_view = Global.universe.object_find(selected_object);
    if (!Global.obj_view)
    {
        Global.obj_view = Global.reference_object;
        throw std::runtime_error("Selected object \"" + selected_object + "\" is not specified in scene file");
    }

    if(Global.obj_view != Global.reference_object)
        Global.ship = std::dynamic_pointer_cast<lib::Ship>(Global.obj_view);

    if(Global.ship == NULL)
    {
        Global.log.add("Object: " + Global.obj_view->name + ", is not pilotable.");
        Global.ship = Global.reference_ship;
    }

}


Camera&
Scene::get_camera() {

    return camera_;
}


const Camera&
Scene::get_camera() const {
    return camera_;
}


std::list<lib::Object::sptr>&
Scene::get_obj_list() {
 
    return object_list_;
}

const std::list<lib::Object::sptr>&
Scene::get_obj_list() const {
    return object_list_;
}

} // namespace amethyst
} // namespace client
