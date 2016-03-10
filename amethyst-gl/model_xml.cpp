/***********************************************************************
 Amethyst-GL
  - Model XML interface function implementations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "model.h"
#include "model_xml.h"
#include "global.h"

#include "lib/vector.h"

#include <iostream>
#include <stdexcept>

namespace amethyst {
namespace client {

void model_xml_parse_sphere(xmlDocPtr doc, xmlNodePtr cur, Model &model, Texture::sptr tex);
void model_xml_parse_extfile(xmlDocPtr doc, xmlNodePtr cur, Model &model, Texture::sptr tex);

void model_xml_load(std::string &name, Model &model)
{

    std::string path = Global.dir_models + "mdl_" + name + ".xml";

    xmlDocPtr  doc;
    xmlNodePtr cur;

    doc = xmlParseFile(path.c_str());

    if (doc == NULL)
    {
        throw(std::runtime_error("Error opening model file: " + path));
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        xmlFreeDoc(doc);
        throw(std::runtime_error("Empty model file: " + path));
    }

    if (xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("model")) )
    {
        xmlFreeDoc(doc);
        throw(std::runtime_error("Scene file parse error, root xml node not \"<model>\": " + path));
    }

    cur = cur->xmlChildrenNode;
    xmlChar *key = NULL;
    Texture::sptr tex; //FIXME Textures Should Load At Primative Level

    // Run through root tree
    try
    {

        while (cur != NULL)
        {
            if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("name")) )
            {
                key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                model.setName(reinterpret_cast<char *>(key));
                xmlFree(key);
            }
            if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("texture")) )
            {
                key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                std::string texture_name = reinterpret_cast<char *>(key);
                tex = texture_load(texture_name);
                xmlFree(key);
            }
            if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("sphere")) )
            {
                if(tex)
                  model_xml_parse_sphere (doc, cur, model, tex);
            }
            if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("extfile")) )
            {
                if(tex)
                  model_xml_parse_extfile (doc, cur, model, tex);
            }

            cur = cur->next;
        }
    }
    catch(std::runtime_error &e)
    {
        if(key)
          xmlFree(key);
        if(doc)
          xmlFreeDoc(doc);
        throw e;
    }

    xmlFreeDoc(doc);
    return;
}


void model_xml_parse_sphere(xmlDocPtr /*doc unused*/, xmlNodePtr cur, Model &model, Texture::sptr tex)
{

    xmlChar *temp;
    double       radius    = 0.0;
    unsigned int precision = 0;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("radius"));
    if (temp)
    {
        radius = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(std::runtime_error("radius= property not found in sphere"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("precision"));
    if (temp)
    {
        precision = atoi(reinterpret_cast<char *>(temp));
        xmlFree(temp);
    } else throw(std::runtime_error("precision= property not found in sphere"));

    // FIXME This should just store the values in a vector list...
    //glBindTexture(GL_TEXTURE_2D, model.texture);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    TriangleStrip::sptr sphere = model_sphere_create(0.0, 0.0, 0.0, radius, precision);
    sphere->setTexture(tex);

    model.addPrimative(sphere);

    return;
}


void model_xml_parse_extfile(xmlDocPtr /*doc unused*/, xmlNodePtr cur, Model &model, Texture::sptr tex)
{

    xmlChar *temp;
    std::string extfile;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("path"));
    if (temp)
    {
        extfile = reinterpret_cast<char *>(temp);
    } else throw(std::runtime_error("path= property not found in extfile"));

    std::string filepath = Global.dir_models + extfile;

    model_load_file(filepath.c_str(), model, tex);

    return;
}

} // namespace amethyst
} // namespace client
