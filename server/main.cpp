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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include "utility.h"

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

    Net_Object *next;
    Net_Object *prev;
};

class __global
{
    public:

    Net_Object   *obj_start;
    Net_Object   *obj_end;

    int          objects;

    __global()
    {
        obj_start = NULL;
        obj_end   = NULL;
    }

    //  Creates an object and links it into the linked list.
    Net_Object* new_net_object(uint32_t ip, uint16_t port)
    {
        if (obj_end == NULL)
        {
            // Create First Object
            obj_start       = new Net_Object;
            obj_start->prev = NULL;
            obj_end         = obj_start;
        } else
        {
            // Create Object on end
            obj_end->next       = new Net_Object;
            obj_end->next->prev = obj_end;
            obj_end             = obj_end->next;
        }

        obj_end->next =  NULL;

        obj_end->ip   = ip;
        obj_end->port = port;
        objects++;

        in_addr ip_struct;
        ip_struct.s_addr = ip;
        std::cout << "NEW OBJECT FROM " << inet_ntoa(ip_struct) << "!\n";

        return obj_end;
    }

    //  Check to see if object based on network ip and port already exists in our list.
    Net_Object* find_net_object(uint32_t ip, uint16_t port)
    {
        Net_Object   *obj1 = obj_start;

        // if obj1 is null, there are no objects so create one and return;
        if(obj1 == NULL) return new_net_object(ip, port);

        do
        {
             // check to see if this is the one
             if((obj1->ip == ip) && (obj1->port == port))
               return obj1;  // Oh teh le3t!  it is.

             obj1 = obj1->next;
        }  while (obj1 != NULL);

        // We didn't find our object so lets create one.
        return new_net_object(ip, port);

    }

#if 0 
    //  Check to see if object based on network ip and port already exists in our list.
    int del_net_object(uint32_t ip, uint16_t port)
    {
        Net_Object   *obj1 = obj_start;

        // if obj1 is null, there are no objects so return;
        if(obj1 == NULL) return 0;

        do
        {
             // check to see if this is the one
             if((obj1->ip == ip) && (obj1->port == port))
             {
               // Is this the last object?
               if(obj1->next == NULL)
               {
                  // Point the end to the prev last
                  obj_end = obj1->prev;

                  // Cap the end
                  obj1->prev->next = NULL;
               }
               if(obj1->prev == NULL)
               {
                    obj_start = NULL;
 
               }

               return 1;
             }


             obj1 = obj1->next;
        }  while (obj1 != NULL);

        // We didn't find our object.
        return 0;

    }
#endif

};

// Instantiate Global Class
__global Global;

static void
bail (const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}

void parse_incoming_dgram(uint32_t ip, uint16_t port, char *dgram, int dgram_size)
{

    // Cast packet_header struct over in packet
    packet_header   *head   = (packet_header*)((char *)dgram);

    // Print header info
    //printf("Type: %i Misc: %i\n", head->type, head->misc);

    if (ntohs(head->type) == 1)
    {

         Net_Object *net_obj = Global.find_net_object(ip, port);

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
    }


}


int build_outgoing_dgram(uint32_t ip, uint16_t port, char *dgram)
{
    int  offset       = 0;
    int  object_count = 0;

    // Cast packet_header struct over out packet
    packet_header   *head   = (packet_header*)dgram;

    head->type = htons(1);   // Type of packet  (1 = object transfer)
    //head->misc = 0;   // Misc (Num of objects...)

    offset += sizeof(packet_header);

    Net_Object   *obj1 = Global.obj_start;

    // No objects,  so just send what we have.
    if (obj1 == NULL) return offset;

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
          }

         obj1 = obj1->next;
    }  while (obj1 != NULL);

    head->misc = htons(object_count);

    return offset;

}


int main(int argc, char** argv)
{
    printf(" amethyst::Cartesian_Vector: %i\n", sizeof(amethyst::Cartesian_Vector));
    printf(" amethyst::Quaternion      : %i\n", sizeof(amethyst::Quaternion));
    printf(" float                     : %i\n", sizeof(float));
    printf(" double                    : %i\n", sizeof(double));
    printf(" vectord_3d                : %i\n", sizeof(vectord_3d));
    printf(" vectord_4d                : %i\n", sizeof(vectord_4d));

    printf(" packet_header             : %i\n", sizeof(packet_header));
    printf(" object_transfer           : %i\n", sizeof(object_transfer));

    int z;
    char *srvr_addr = NULL;
    struct sockaddr_in adr_inet; /* AF_INET */
    struct sockaddr_in adr_clnt; /* AF_INET */
    int len_inet;                /* Length */
    int s;                       /* socket */
    char rx_dgram[1500];         /* Recv Buffer */
    char tx_dgram[1500];         /* TX   Bufffer */
//  time_t td;                   /* Current Time and Date */


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
       //expire_objects(30);  // 30 secs

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

    return 0;
}
