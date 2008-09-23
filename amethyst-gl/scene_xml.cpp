/***********************************************************************
 Amethyst-GL
  - Scene XML interface function implementations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <libxml/tree.h>
#include <libxml/parser.h>

#include "scene.h"
#include "scene_xml.h"
#include "model.h"
#include "global.h"

#include "lib/vector.h"

#include <iostream>

bool scene_load(std::string &name);
bool scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur);
bool scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, amethyst::Cartesian_Vector &vector);
bool scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, amethyst::Quaternion &quat);

bool scene_load(std::string &name)
{

    std::string path = Global.dir_scene + "scn_" + name + ".xml";

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

    if (xmlStrcmp(cur->name, (const xmlChar *) "scene"))
    {
        fprintf(stderr,"document of wrong type, root node != scene.\n");
        xmlFreeDoc(doc);
        return false;
    }

    cur = cur->xmlChildrenNode;

    // Run through root tree
    {
        std::string player_object;

        while (cur != NULL)
        {
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"player")))
            {
                xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                player_object = (char *)key;
                xmlFree(key);
            }
            if ((!xmlStrcmp(cur->name, (const xmlChar *)"object")))
            {
                if (!scene_xml_parse_object (doc, cur)) return false;
            }

            cur = cur->next;
        }
    }

    xmlFreeDoc(doc);
    return true;
}


bool scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur)
{
    Object *temp = new amethyst::Object;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"name")))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            temp->name = (char *)key;
            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"model")))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            std::string model_name = (char *)key;
            temp->meta = model_load(model_name);
            xmlFree(key);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"location")))
        {
            if (!scene_xml_parse_vector(doc, cur, temp->location))
                std::cout << "OMFG!!! location!" << std::endl;
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"velocity")))
        {
            if (!scene_xml_parse_vector(doc, cur, temp->velocity))
                std::cout << "OMFG!!! velocity!" << std::endl;
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"attitude")))
        {
            if (!scene_xml_parse_quat(doc, cur, temp->attitude))
                std::cout << "OMFG!!! orientation!" << std::endl;
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"mass")))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            temp->mass = strtod((char *)key, NULL);
            xmlFree(key);
        }
        cur = cur->next;
    }

    scene_add_object(temp);
    universe.object_add(temp);
    Global.ship = temp;
    return true;
}


bool scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, amethyst::Cartesian_Vector &vector)
{
    xmlChar *temp;

    temp = xmlGetProp(cur, (const xmlChar *)"x");
    if (temp)
    {
        vector.x = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"y");
    if (temp)
    {
        vector.y = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"z");
    if (temp)
    {
        vector.z = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    return true;
}


bool scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, amethyst::Quaternion &quat)
{
    xmlChar *temp;

    temp = xmlGetProp(cur, (const xmlChar *)"w");
    if (temp)
    {
        quat.w = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"x");
    if (temp)
    {
        quat.x = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"y");
    if (temp)
    {
        quat.y = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, (const xmlChar *)"z");
    if (temp)
    {
        quat.z = strtod((char *)temp, NULL);
        xmlFree(temp);
    } else return false;

    return true;
}

