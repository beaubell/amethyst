#pragma once
/***********************************************************************
 Amethyst-GL
  - Scene XML interface function declarations

 Authors (c):
 2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "scene.h"

namespace amethyst {
namespace client {

void scene_load(Scene& scene, const std::string &name);
void scene_xml_write(const Scene& scene, const std::string &name);

} // namespace amethyst
} // namespace client
