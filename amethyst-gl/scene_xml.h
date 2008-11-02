#ifndef AMETHYST_CLIENT_SCENE_XML_H
#define AMETHYST_CLIENT_SCENE_XML_H

/***********************************************************************
 Amethyst-GL
  - Scene XML interface function declarations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

namespace amethyst {
namespace client {

void scene_load(const std::string &name);
void scene_xml_write(const std::string &name);

} // namespace amethyst
} // namespace client

#endif /* AMETHYST_CLIENT_SCENE_XML_H */
