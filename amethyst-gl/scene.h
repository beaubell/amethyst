#pragma once
/***********************************************************************
 Amethyst-GL
  - Scene randering function prototypes

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"
#include "lib/object.h"

#include <list>


namespace amethyst {
namespace client {

class Scene {

  public:
    Scene();
    virtual ~Scene();
    void render(const Eye eye = Eye::MONO);

    void add_object(lib::Object::sptr);
    void select_object_next();
    void target_object_next();
    void control_ship_next();
    
    void toYAMLFile(const std::string &name);
    
    Camera& get_camera();
    const Camera& get_camera() const;
    std::list<lib::Object::sptr>& get_obj_list();
    const std::list<lib::Object::sptr>& get_obj_list() const;

  private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Camera camera_;
    std::list<lib::Object::sptr>  object_list_;

}; // class Scene

} // namespace client
} // namespace amethyst
