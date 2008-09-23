/***************************************************************************
 *  XML Configuration file functions                                       *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "model.h"
#include "model_xml.h"
#include "global.h"

#include "lib/vector.h"

#include <iostream>

bool model_xml_load(std::string &name, Model &model);
bool model_xml_parse_sphere(xmlDocPtr doc, xmlNodePtr cur, Model &model);
bool model_xml_parse_extfile(xmlDocPtr doc, xmlNodePtr cur, Model &model);
//bool scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur);
//bool scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, amethyst::Cartesian_Vector &vector);
//bool scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, amethyst::Quaternion &quat);

bool model_xml_load(std::string &name, Model &model)
{

    std::string path = Global.dir_models + "mdl_" + name + ".xml";

    xmlDocPtr  doc;
    xmlNodePtr cur;

    doc = xmlParseFile(path.c_str());

    if (doc == NULL)
    {
        fprintf(stderr,"Document not parsed successfully. \n");
        return false;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return false;
    }

    if (xmlStrcmp(cur->name, (const xmlChar *) "model"))
    {
        fprintf(stderr,"document of wrong type, root node != scene.\n");
        xmlFreeDoc(doc);
        return false;
    }

    cur = cur->xmlChildrenNode;

    // Run through root tree
    {

        while (cur != NULL) 
        {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
            {
                xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                model.name = (char *)key;
                xmlFree(key);
            }
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"texture")))
            {
                xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                std::string texture_name = (char *)key;
                model.texture = texture_load(texture_name);
                xmlFree(key);
            }
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"sphere")))
            {
                if (!model_xml_parse_sphere (doc, cur, model)) return false;
            }
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"extfile")))
            {
                if (!model_xml_parse_extfile (doc, cur, model)) return false;
            }

            cur = cur->next;
        }
    }

    xmlFreeDoc(doc);
    return true;
}


bool model_xml_parse_sphere(xmlDocPtr doc, xmlNodePtr cur, Model &model)
{

    xmlChar *temp;
    double radius, precision = 0.0;

    temp = xmlGetProp(cur, (const xmlChar *)"radius");
    if (temp)
    {
        radius = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"precision");
    if (temp)
    {
        precision = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    model.dl = glGenLists(1);
    glNewList(model.dl, GL_COMPILE);
    glBindTexture(GL_TEXTURE_2D, model.texture);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    model_sphere_create(0.0, 0.0, 0.0, radius, precision);
    glEndList();

    return true;
}


bool model_xml_parse_extfile(xmlDocPtr doc, xmlNodePtr cur, Model &model)
{

    xmlChar *temp;
    std::string extfile;

    temp = xmlGetProp(cur, (const xmlChar *)"path");
    if (temp)
    {
        extfile = (char *)temp;
    } else return false;

    std::string filepath = Global.dir_models + extfile;

    model.dl = glGenLists(1);
    glNewList(model.dl, GL_COMPILE);
    model_load_file(filepath.c_str());
    glEndList();

    return true;
}

