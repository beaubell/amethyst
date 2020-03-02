#pragma once
/***********************************************************************
 Amethyst-GL
  - Scene randering function prototypes

 Authors (c):
 2006-2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "camera.h"
#include "lib/object.h"
#include "yaml-cpp/node/node.h"

#include <list>
#include <unordered_map>
#include <string>


namespace amethyst {
namespace client {

class Scene {

  public:
    typedef std::unordered_map<std::string, lib::Object::sptr> ObjectMap;

    Scene();
    virtual ~Scene();
    void render(const Eye eye = Eye::MONO);

    void add_object(lib::Object::sptr);
    void select_object_next();
    void target_object_next();
    void control_ship_next();
    
    void toYAMLFile(const std::string &name);
    void fromYAML(const YAML::Node& node);
    
    Camera& get_camera();
    const Camera& get_camera() const;
    ObjectMap& get_obj_list();
    const ObjectMap& get_obj_list() const;

  private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Camera camera_;
    ObjectMap objects_;
    ObjectMap points_;

}; // class Scene

} // namespace client
} // namespace amethyst
