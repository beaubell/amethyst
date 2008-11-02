/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "object.h"
#include "net.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <list>

#include "server.h"

using namespace amethyst::lib;
using namespace amethyst::server;
using namespace amethyst;


struct packet_header
{
  // Sequence Counters
  u_int16_t  out_seq;
  u_int16_t  in_seq;

  // Time
  //u_int32_t  timestamp;

  // Packet Type
  u_int16_t  type;    // 1 == object transfer,  2 == chat?  maybe?
  u_int16_t  misc;    // if type==1,  misc = number of objects
} __attribute__((__packed__));


struct object_transfer
{
  char        name[12];
  vectord_3d  location;
  vectord_3d  velocity;
  vectord_3d  acceleration;

  vectord_4d  attitude;

  char        pad[12];
} __attribute__((__packed__));


class Net_Object: public Object
{
  public:
    uint32_t    ip;
    uint16_t    port;
    char        name[12];

    time_t      updated;
};

class __global
{
  public:
    std::list<Net_Object>           net_object_list;
    char  timestamp[50];
};


// Instantiate Global Class
__global Global;

// Prototypes
char*       mk_timestamp        (void);
static void bail                (const char *on_what);
Net_Object* new_net_object      (uint32_t ip, uint16_t port);
Net_Object* find_net_object     (uint32_t ip, uint16_t port);
int         expire_net_objects  (unsigned int expire_offset);
void        parse_incoming_dgram(uint32_t ip, uint16_t port, char *dgram, int dgram_size);
int         build_outgoing_dgram(uint32_t ip, uint16_t port, char *dgram);


char * mk_timestamp ()
{
    // Get unix time
    time_t unix_time;
    time(&unix_time);

    // Convert to tm struct
    struct tm *tm = localtime(&unix_time);

    // Format time string
    strftime((char *)&Global.timestamp, sizeof(Global.timestamp), "%D %T - ", tm);

    // Return it
    return (char *)&Global.timestamp;
}


static void
bail (const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}


//  Creates an object and links it into the linked list.
Net_Object* new_net_object(uint32_t ip, uint16_t port)
{
        Net_Object new_obj;

        new_obj.ip   = ip;
        new_obj.port = port;

        // Add object to list
        Global.net_object_list.push_back(new_obj);

        // Display IP address
        {
          in_addr ip_struct;
          ip_struct.s_addr = ip;
          std::cout << mk_timestamp()
                    <<  "NEW OBJECT FROM "
                    << inet_ntoa(ip_struct)
                    << ":"
                    << ntohs(port)
                    << "\n";
        }

        return &Global.net_object_list.back();
}


//  Check to see if object based on network ip and port already exists in our list.
Net_Object* find_net_object(uint32_t ip, uint16_t port)
{
        // Check if list is empty, make object if it is, otherwise continue
        if(Global.net_object_list.empty()) return new_net_object(ip, port);

        //Make an iterator and point it at the start of our net_object list;
        std::list<Net_Object>::iterator obj1 = Global.net_object_list.begin();

        do
        {
             // check to see if this is the one
             if((obj1->ip == ip) && (obj1->port == port))
               return &*obj1;  // Oh teh le3t!  it is.

             obj1++;
        }  while (obj1 != Global.net_object_list.end());

        // We didn't find our object so lets create one.
        return new_net_object(ip, port);

}


//  Expire objects based on time.
int expire_net_objects(unsigned int expire_offset)
{
        // Check if list is empty, if it is, return 0
        if(Global.net_object_list.empty()) return 0;

        // Get expipre time
        time_t expire_time;
        time(&expire_time);
        expire_time -= expire_offset;

        int objects_del_count = 0;

        //Make an iterator and point it at the start of our net_object list;
        std::list<Net_Object>::iterator obj1 = Global.net_object_list.begin();

        do
        {
             // check to see if this object is expired
             if(obj1->updated < expire_time)
             {
               // Print Info about object being deleted
               {
                  in_addr ip_struct;
                  ip_struct.s_addr = obj1->ip;
                  std::cout << mk_timestamp()
                            << "    OBJECT FROM "
                            << inet_ntoa(ip_struct)
                            << ":"
                            << ntohs(obj1->port)
                            << " IS EXPIRED\n";
               }

               // Delete the expired object
               Global.net_object_list.erase(obj1);

               objects_del_count++;

               // FIXME Must return,  the iterator is now broke.
               return 1;
             }

             obj1++;

        }  while (obj1 != Global.net_object_list.end());

        return objects_del_count;

}


void parse_incoming_dgram(uint32_t ip, uint16_t port, char *dgram, int dgram_size)
{

    // Cast packet_header struct over in packet
    packet_header   *head   = (packet_header*)((char *)dgram);

    // Print header info
    //printf("Type: %i Misc: %i\n", head->type, head->misc);

    if (ntohs(head->type) == 1)
    {

         Net_Object *net_obj = find_net_object(ip, port);

         // Cast object_transfer struct over out packet after header struct
         object_transfer *inc_object = (object_transfer *)((char *)dgram + sizeof(packet_header));

         //Print name and pad
         //printf("name: %s   pad: %s\n", &inc_object->name, &inc_object->pad);
         strncpy(net_obj->name, inc_object->name, 12);

         //Unpack data from packets
         net_unpack(net_obj->location,     inc_object->location);
         net_unpack(net_obj->velocity,     inc_object->velocity);
         net_unpack(net_obj->acceleration, inc_object->acceleration);

         net_unpack(net_obj->attitude,     inc_object->attitude);

         time(&net_obj->updated);
    } // if
}


int build_outgoing_dgram(uint32_t ip, uint16_t port, char *dgram)
{
    int  offset       = 0;
    int  object_count = 0;

    // Cast packet_header struct over out packet
    packet_header   *head   = (packet_header*)dgram;

    head->type = htons(1);   // Type of packet  (1 = object transfer)

    offset += sizeof(packet_header);

    // If no objects,  just send what we have.
    if (Global.net_object_list.empty()) return offset;

    //Make an iterator and point it at the start of our net_object list;
    std::list<Net_Object>::iterator obj1 = Global.net_object_list.begin();

    do
    {

         if((obj1->ip != ip) || (obj1->port != port))
         {
             // Increment object count;
             object_count++;

             // Cast object_transfer struct over out packet after header struct
             object_transfer *out_object = (object_transfer *)(dgram + offset);

             strncpy(out_object->name, obj1->name,9);
             out_object->name[9] = '\0';

             strncpy(out_object->pad, "XOXOXOXOXOXO",9);
             out_object->pad[9] = '\0';

             net_pack(out_object->location,     obj1->location);
             net_pack(out_object->velocity,     obj1->velocity);
             net_pack(out_object->acceleration, obj1->acceleration);

             net_pack(out_object->attitude,     obj1->attitude);

             offset += sizeof(object_transfer);
          } // if

         obj1++;
    }  while (obj1 != Global.net_object_list.end());

    head->misc = htons(object_count);

    return offset;

}


int main(int argc, char** argv)
{
    //printf(" amethyst::Cartesian_Vector: %i\n", sizeof(amethyst::Cartesian_Vector));
    //printf(" amethyst::Quaternion      : %i\n", sizeof(amethyst::Quaternion));
    //printf(" float                     : %i\n", sizeof(float));
    //printf(" double                    : %i\n", sizeof(double));
    //printf(" vectord_3d                : %i\n", sizeof(vectord_3d));
    //printf(" vectord_4d                : %i\n", sizeof(vectord_4d));

    //printf(" packet_header             : %i\n", sizeof(packet_header));
    //printf(" object_transfer           : %i\n", sizeof(object_transfer));

    std::cout << "Starting new-style server" << std::endl;
    Server s("", "2525", "/home/beau/.amethyst-server");
    s.run();

#if 0
    int z;
    const char *srvr_addr = NULL;
    struct sockaddr_in adr_inet; /* AF_INET */
    struct sockaddr_in adr_clnt; /* AF_INET */
    int len_inet;                /* Length */
    //int s;                       /* socket */
    char rx_dgram[1500];         /* Recv Buffer */
    char tx_dgram[1500];         /* TX   Bufffer */


    // Read address from commandline
    if ( argc >= 2 ) {
       // Addr on cmdline
       srvr_addr = argv[1];
    }  else {
       srvr_addr = "127.0.0.1";
    }

    // Create Socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == -1)
        bail("socket()");

    // Create socket address for use with bind(2)
    memset(&adr_inet,0,sizeof adr_inet);
    adr_inet.sin_family = AF_INET;
    adr_inet.sin_port = htons(23232);
    adr_inet.sin_addr.s_addr = inet_addr(srvr_addr);

    if ( adr_inet.sin_addr.s_addr == INADDR_NONE )
        bail("bad address.");

    len_inet = sizeof adr_inet;

    // Bind Address to our socket so clients can contact server
    z = bind(s, (struct sockaddr *)&adr_inet, len_inet);
    if ( z == -1)
        bail("bind()");


    //  Now wait for requests:
    for (;;) {

       // Block untill the program recieves a request;
       len_inet = sizeof adr_clnt;
       z = recvfrom(s,                // Socket
                    rx_dgram,         // Recieve buffer
                    sizeof rx_dgram,  // Max Buffer
                    0,                // Flags: No Options
                    (struct sockaddr *)&adr_clnt,  // Addr of client
                    (socklen_t*)&len_inet);
       if (z < 0)
           bail("recvfrom(2)");


       // Parse that beotch
       parse_incoming_dgram(adr_clnt.sin_addr.s_addr, adr_clnt.sin_port, rx_dgram, z);

       // Expire old objects
       expire_net_objects(30);  // 30 secs

       // Generate a response
       int tx_size = build_outgoing_dgram(adr_clnt.sin_addr.s_addr, adr_clnt.sin_port, tx_dgram);

       // Send Response
       z = sendto(s,              // Socket
                  tx_dgram,         // The datagram result to snd
                  tx_size,          // Datagram length
                  0,                // Flags: No Options
                  (struct sockaddr *)&adr_clnt,  // Addr
                  len_inet);        // Client address length


    } // for
#endif
    return 0;
}
