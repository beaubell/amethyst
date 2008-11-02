/***********************************************************************
 Amethyst-GL
  - Main XML Configuration file functions

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include <iostream>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "global.h"

#include "config_xml.h"
#include "scene_xml.h"

namespace amethyst {
namespace client {

void parse_xml_network (xmlDocPtr doc, xmlNodePtr cur) {

    xmlChar *key;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("server") )) {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        Global.net_server = reinterpret_cast<char *>(key);
        xmlFree(key);
      }
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("port") )) {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        Global.net_port = atoi(reinterpret_cast<char *>(key));
        xmlFree(key);
      }
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("handle") )) {
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        Global.net_handle = reinterpret_cast<char *>(key);
        xmlFree(key);
      }
      cur = cur->next;
    }
    return;
}

Joy_Axis_Map parse_xml_map_joystick (xmlDocPtr, xmlNodePtr cur) {

  Joy_Axis_Map joy_map;
  xmlChar *att;

  joy_map.joystick = 0;
  joy_map.axis     = 0;

  att = xmlGetProp(cur, reinterpret_cast<const xmlChar *>("num"));
  if (att) {
    joy_map.joystick = atoi(reinterpret_cast<char *>(att));
    xmlFree(att);
  }

  att= xmlGetProp(cur, reinterpret_cast<const xmlChar *>("axis"));
  if (att) {
    joy_map.axis = atoi(reinterpret_cast<char *>(att));
    xmlFree(att);
  }

  return joy_map;
}

Joy_Axis_Map parse_xml_map_axis (xmlDocPtr doc, xmlNodePtr cur) {

  cur = cur->xmlChildrenNode;

  Joy_Axis_Map joy_map;
  joy_map.joystick = 0;
  joy_map.axis     = 0;

  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("joystick"))) {
      joy_map = parse_xml_map_joystick(doc, cur);
    }
    cur = cur->next;
  }

  return joy_map;
}

void parse_xml_mapping (xmlDocPtr doc, xmlNodePtr cur) {

  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("pitch"))) {
      Global.axis_pitch    = parse_xml_map_axis (doc, cur);
    }
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("roll"))) {
      Global.axis_roll     = parse_xml_map_axis (doc, cur);
    }
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("yaw"))) {
      Global.axis_yaw      = parse_xml_map_axis (doc, cur);
    }
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("throttle"))) {
      Global.axis_throttle = parse_xml_map_axis (doc, cur);
    }
    cur = cur->next;
  }
  return;
}

void parse_xml_input (xmlDocPtr doc, xmlNodePtr cur) {

  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("mapping"))) {
      parse_xml_mapping (doc, cur);
    }
    cur = cur->next;
  }
  return;
}

void parse_xml_starfighter (xmlDocPtr doc, xmlNodePtr cur) {

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("network"))) {
          parse_xml_network (doc, cur);
      }
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("input"))) {
          parse_xml_input (doc, cur);
      }
      if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("scene"))) {
          xmlChar *key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
          Global.scene = reinterpret_cast<char *>(key);
          xmlFree(key);
      }
      cur = cur->next;
    }
    return;
}


void parse_xml_config(const char *docname) {

    xmlDocPtr  doc;
    xmlNodePtr cur;

    doc = xmlParseFile(docname);

    if (doc == NULL) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return;
    }

    if (xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("amethyst"))) {
        fprintf(stderr,"document of wrong type, root node != amethyst.\n");
        xmlFreeDoc(doc);
        return;
    }

    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if (!xmlStrcmp(cur->name, reinterpret_cast<const xmlChar *>("config"))) {
          parse_xml_starfighter (doc, cur);
        }

        cur = cur->next;
    }

    xmlFreeDoc(doc);
    return;
}

} // namespace client
} // namespace amethyst
