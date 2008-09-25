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
#include <stdexcept>
#include <limits>

bool scene_load(std::string &name);
void scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur, amethyst::Object& obj);
bool scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, amethyst::Cartesian_Vector &vector);
bool scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, amethyst::Quaternion &quat);


class parse_error : public std::runtime_error
{
    public:
      explicit parse_error(const std::string& msg) : std::runtime_error(msg), what_(msg) { }

      virtual const char* what() const throw ()
        { return what_.c_str(); }

      virtual ~parse_error() throw () {}

    private:
      std::string what_;
};



bool scene_load(std::string &name)
{

    std::string path = Global.dir_scene + "scn_" + name + ".xml";

    xmlDocPtr  doc;
    xmlNodePtr cur;

    doc = xmlParseFile(path.c_str());

    if (doc == NULL)
    {
        throw(std::runtime_error("Error opening scene file: " + path));
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        xmlFreeDoc(doc);
        throw(std::runtime_error("Empty scene file: " + path));
    }

    if (xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("scene") ))
    {
        xmlFreeDoc(doc);
        throw(std::runtime_error("Scene file parse error, root xml node not \"<scene>\": " + path));
    }

    cur = cur->xmlChildrenNode;

    std::string player_object;

    // Run through root tree
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("player") ))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            player_object = reinterpret_cast<char *>(key);
            xmlFree(key);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("object") ))
        {
            Object *temp;
            try
            {
                temp = new amethyst::Object;
                scene_xml_parse_object (doc, cur, *temp);
            }
            catch (parse_error& e)
            {
                delete temp;
                xmlFreeDoc(doc);
                throw e;
            }

            scene_add_object(temp);
            universe.object_add(temp);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);

    //Find Player and set
    Global.ship = universe.object_find(player_object);
    if (Global.ship == NULL)
    {
        Global.ship = &Global.reference_object;
        throw parse_error("Player object \"" + player_object + "\" is not specified in scene file");
    }

    return true;
}


void scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur, amethyst::Object &new_obj)
{
    xmlChar *temp;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("name"));
    if (temp)
    {
        new_obj.name = reinterpret_cast<char *>(temp);
        xmlFree(temp);
    } else throw(parse_error("object has no name= attribute"));

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {

        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("model") ))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            std::string model_name = reinterpret_cast<char *>(key);
            new_obj.meta = model_load(model_name);
            xmlFree(key);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("location") ))
        {
            if (!scene_xml_parse_vector(doc, cur, new_obj.location))
                std::cout << "OMFG!!! location!" << std::endl;
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("velocity") ))
        {
            if (!scene_xml_parse_vector(doc, cur, new_obj.velocity))
                std::cout << "OMFG!!! velocity!" << std::endl;
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("attitude") ))
        {
            if (!scene_xml_parse_quat(doc, cur, new_obj.attitude))
                std::cout << "OMFG!!! orientation!" << std::endl;
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("mass") ))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            new_obj.mass = strtod(reinterpret_cast<char *>(key), NULL);
            xmlFree(key);
        }
        cur = cur->next;
    }


    return;
}


bool scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, amethyst::Cartesian_Vector &vector)
{
    xmlChar *temp;

    // Corrupt values of vector before assignment in order to catch errors.
    vector.x = vector.y = vector.z = std::numeric_limits<double>::signaling_NaN();

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("x"));
    if (temp)
    {
        vector.x = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("x= property not found in vector"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("y"));
    if (temp)
    {
        vector.y = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("y= property not found in vector"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("z"));
    if (temp)
    {
        vector.z = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("z= property not found in vector"));


    return true;
}


bool scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, amethyst::Quaternion &quat)
{
    xmlChar *temp;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("w"));
    if (temp)
    {
        quat.w = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("x"));
    if (temp)
    {
        quat.x = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("y"));
    if (temp)
    {
        quat.y = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else return false;

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("z"));
    if (temp)
    {
        quat.z = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else return false;

    return true;
}

