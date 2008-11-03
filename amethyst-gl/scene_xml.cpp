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
#include "lib/object.h"
#include "lib/ship.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <limits>

namespace amethyst {
namespace client {

using lib::Cartesian_Vector;
using lib::Object;
using lib::Ship;
using lib::Ship_ptr;

static void scene_xml_parse_client(xmlDocPtr doc, xmlNodePtr cur, std::string &selected);
static void scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur, Object& obj);
static void scene_xml_parse_ship(xmlDocPtr doc, xmlNodePtr cur, Ship_ptr ship);
static void scene_xml_parse_shader(xmlDocPtr doc, xmlNodePtr cur);
static void scene_xml_parse_vector(xmlDocPtr doc, xmlNodePtr cur, Cartesian_Vector &vector);
static void scene_xml_parse_quat(xmlDocPtr doc, xmlNodePtr cur, Quaternion &quat);

static void scene_xml_write_vector(std::ofstream &outfile, const Cartesian_Vector &v, const std::string &vname);
static void scene_xml_write_quat(std::ofstream &outfile, const Quaternion &q, const std::string &qname);


class parse_error : public std::runtime_error
{
    public:
      explicit parse_error(const std::string& msg) : std::runtime_error(msg), what_(msg) { }

      virtual const char* what() const throw ()
        { return what_.c_str(); }

      virtual ~parse_error() throw () {}

      std::string what_;
};


void scene_load(const std::string &name)
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

    std::string selected_object;
    std::string selected_ship;

    // Run through root tree
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("client") ))
        {
            scene_xml_parse_client (doc, cur, selected_object);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("shader") ))
        {
            scene_xml_parse_shader (doc, cur);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("object") ))
        {
            Object *temp;
            try
            {
                temp = new Object;
                scene_xml_parse_object (doc, cur, *temp);
            }
            catch (parse_error& e)
            {
                delete temp;
                xmlFreeDoc(doc);
                throw e;
            }

            scene_add_object(temp);
            Global.universe.object_add(temp);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("ship") ))
        {
            lib::Ship_ptr temp;
            try
            {
                temp = Ship_ptr(new Ship());
                scene_xml_parse_ship (doc, cur, temp);
            }
            catch (parse_error& e)
            {
                temp.reset();
                xmlFreeDoc(doc);
                throw e;
            }

            scene_add_object(get_pointer(temp));
            Global.ships.insert(temp);
            Global.universe.object_add(get_pointer(temp));
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);

    //Find Player and set
    Global.obj_view = Global.universe.object_find(selected_object);
    if (Global.obj_view == NULL)
    {
        Global.obj_view = &Global.reference_object;
        throw parse_error("Selected object \"" + selected_object + "\" is not specified in scene file");
    }

    if(Global.obj_view != &Global.reference_object)
        Global.ship = dynamic_cast<Ship*>(Global.obj_view);

    if(Global.ship == NULL)
    {
        Global.log.add("Object: " + Global.obj_view->name + ", is not pilotable.");
        Global.ship = &Global.reference_ship;
    }

    return;
}


static void scene_xml_parse_client(xmlDocPtr doc, xmlNodePtr cur, std::string &selected)
{
    xmlChar *temp;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {

        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("selected") ))
        {
            xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            selected = reinterpret_cast<char *>(key);
            xmlFree(key);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("camera") ))
        {
            temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("yaw"));
            if (temp)
            {
                Global.cam_yaw = strtod(reinterpret_cast<char *>(temp), NULL);
                xmlFree(temp);
            }

            temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("pitch"));
            if (temp)
            {
                Global.cam_pitch = strtod(reinterpret_cast<char *>(temp), NULL);
                xmlFree(temp);
            }

            temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("dist"));
            if (temp)
            {
                Global.cam_zoom = strtod(reinterpret_cast<char *>(temp), NULL);
                xmlFree(temp);
            }
        }
        cur = cur->next;
    }

    return;
}


static void scene_xml_parse_shader(xmlDocPtr doc, xmlNodePtr cur)
{
    xmlChar *temp;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {

        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("vertex") ))
        {
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            Global.vshader = reinterpret_cast<char *>(temp);
            xmlFree(temp);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("fragment") ))
        {
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            Global.fshader = reinterpret_cast<char *>(temp);
            xmlFree(temp);
        }
        cur = cur->next;
    }

    return;
}


static void scene_xml_parse_object(xmlDocPtr doc, xmlNodePtr cur, Object &new_obj)
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
            try
            {
                temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                std::string model_name = reinterpret_cast<char *>(temp);
                new_obj.meta = model_load(model_name);
            }
            catch(parse_error &e)
            {
                xmlFree(temp);
                throw e;
            }
            xmlFree(temp);
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("location") ))
        {
            try
            {
                scene_xml_parse_vector(doc, cur, new_obj.location);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <location> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("velocity") ))
        {
            try
            {
                scene_xml_parse_vector(doc, cur, new_obj.velocity);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <velocity> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("acceleration") ))
        {
            try
            {
                scene_xml_parse_vector(doc, cur, new_obj.acceleration);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <acceleration> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("attitude") ))
        {
            try
            {
                scene_xml_parse_quat(doc, cur, new_obj.attitude);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <attitude> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("angular_velocity") ))
        {
            try
            {
                scene_xml_parse_quat(doc, cur, new_obj.angular_velocity);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <angular_velocity> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("angular_acceleration") ))
        {
            try
            {
                scene_xml_parse_quat(doc, cur, new_obj.angular_acceleration);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <angular_acceleration> in <object name=\"" + new_obj.name + "\">"));
            }
        }
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("mass") ))
        {
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            new_obj.mass = strtod(reinterpret_cast<char *>(temp), NULL);
            xmlFree(temp);
        }
        cur = cur->next;
    }


    return;
}


static void scene_xml_parse_ship(xmlDocPtr doc, xmlNodePtr cur, Ship_ptr new_ship)
{
    xmlChar *temp;

    cur = cur->xmlChildrenNode;
    while (cur != NULL)
    {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("object") ))
        {
            try
            {
                scene_xml_parse_object(doc, cur, *new_ship);
            }
            catch(parse_error &e)
            {
                throw(parse_error(e.what_ + ": <object> in <ship>"));
            }
        }
        cur = cur->next;
    }


    return;
}


static void scene_xml_parse_vector(xmlDocPtr, xmlNodePtr cur, Cartesian_Vector &vector)
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

    return;
}


static void scene_xml_parse_quat(xmlDocPtr, xmlNodePtr cur, Quaternion &quat)
{
    xmlChar *temp;

    // Corrupt values of quat before assignment in order to catch errors.
    quat.w = quat.x = quat.y = quat.z = std::numeric_limits<double>::signaling_NaN();

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("w"));
    if (temp)
    {
        quat.w = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("w= property not found in vector"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("x"));
    if (temp)
    {
        quat.x = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("x= property not found in vector"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("y"));
    if (temp)
    {
        quat.y = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("y= property not found in vector"));

    temp = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("z"));
    if (temp)
    {
        quat.z = strtod(reinterpret_cast<char *>(temp), NULL);
        xmlFree(temp);
    } else throw(parse_error("z= property not found in vector"));

    return;
}


void scene_xml_write (const std::string &name)
{

    std::ofstream outfile;
    std::string   filename = Global.dir_scene + "scn_" + name + ".xml";

    outfile.open(filename.c_str());

    outfile << std::setprecision(30);

    //Begin writing file
    outfile << "<?xml version=\"1.0\"?>" << std::endl;
    outfile << "<scene>" << std::endl;
    outfile << "  <name>" << name << "</name>" << std::endl;
    outfile << "  <client>" << std::endl;
    outfile << "    <selected>" << Global.ship->name << "</selected>" << std::endl;
    outfile << "    <camera yaw=\"" << Global.cam_yaw << "\" pitch=\""
                                    << Global.cam_pitch << "\" dist=\""
                                    << Global.cam_zoom << "\" />" << std::endl;
    outfile << "  </client>" << std::endl;
    outfile << "  <shader>" << std::endl;
    outfile << "    <vertex>" << Global.vshader << "</vertex>" << std::endl;
    outfile << "    <fragment>" << Global.fshader << "</fragment>" << std::endl;
    outfile << "  </shader>" << std::endl;

    if(!object_list.empty())
    {
        std::list<Object *>::iterator obj1 = object_list.begin();
        do
        {
            outfile << "  <object name=\"" << (*obj1)->name << "\">" << std::endl;
            outfile << "    <model>" << reinterpret_cast<Model *>((*obj1)->meta)->name << "</model>" << std::endl;
            scene_xml_write_vector(outfile, (*obj1)->location, "location");
            scene_xml_write_vector(outfile, (*obj1)->velocity, "velocity");
            scene_xml_write_vector(outfile, (*obj1)->acceleration, "acceleration");
            scene_xml_write_vector(outfile, (*obj1)->force, "force");
            scene_xml_write_quat(outfile, (*obj1)->attitude, "attitude");
            scene_xml_write_quat(outfile, (*obj1)->angular_velocity, "angular_velocity");
            scene_xml_write_quat(outfile, (*obj1)->angular_acceleration, "angular_acceleration");
            outfile << "    <mass>" << (*obj1)->mass << "</mass>" << std::endl;
            outfile << "    <!-- debug info:" << std::endl;
            outfile << "      displaylist:" << reinterpret_cast<Model *>((*obj1)->meta)->dl << std::endl;
            outfile << "    -->" << std::endl;
            outfile << "  </object>" << std::endl;

            obj1++;
        }  while (obj1 != object_list.end());

    }

    outfile << "</scene>" << std::endl;
    outfile.close();
}


static void scene_xml_write_vector(std::ofstream &outfile, const Cartesian_Vector &v, const std::string &vname)
{
    outfile << "    <" << vname << " x=\"" << v.x << "\" y=\"" << v.y << "\" z=\"" << v.z << "\" />" << std::endl;
}

static void scene_xml_write_quat(std::ofstream &outfile, const Quaternion &q, const std::string &qname)
{
    outfile << "    <" << qname << " w=\"" << q.w << "\" x=\"" << q.x << "\" y=\"" << q.y << "\" z=\"" << q.z << "\" />" << std::endl;
}

} // namespace amethyst
} // namespace client
