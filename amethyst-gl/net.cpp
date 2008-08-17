/***************************************************************************
 *   Starfighter network functions                                         *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#ifdef WIN32
 #include <windows.h>
#endif

#include <iostream>
#include "SDL_net.h"

#include "thread.h"
#include "net.h"
#include "global.h"

SF_Thread_Id net_thread;
int net_thread_stop = 0;

static int net_start_thread(void *data);

int udpsend(UDPsocket sock, int channel, UDPpacket *out)
{
    if(!SDLNet_UDP_Send(sock, channel, out))
    {
        printf("SDLNet_UDP_Send: %s\n",SDLNet_GetError());
        exit(1);
    }
    return(1);
}

int udprecv(UDPsocket sock, UDPpacket *in)
{
    return SDLNet_UDP_Recv(sock, in);
}


int setup_network(void)
{

  std::cout << "Initializing Network...";

  if( SDLNet_Init() == -1)
  {
    std::cout << "Failed";
  }
  else
  {
      std::cout << "Success";
  }
  std::cout << std::endl;

  //////////////
  //////////////

  if(SDLNet_ResolveHost(&Global.net_server_ip,Global.net_server.c_str(),Global.net_port)==-1)
  {
     //fprintf(stderr, "SDLNet_ResolveHost: %s\n",SDLNet_GetError());
     fprintf(stderr, "ERROR: Unable to resolve host: %s\n", Global.net_server.c_str());
     return -1;
  }

  // open udp client socket
  if(!(Global.net_socket = SDLNet_UDP_Open(0)))
  {
     fprintf(stderr, "SDLNet_UDP_Open: %s\n",SDLNet_GetError());
     return -1;
  }

  // allocate packer buffers
  if(!(Global.pack_out=SDLNet_AllocPacket(65535)))
  {
     fprintf(stderr, "SDLNet_AllocPacket: %s\n",SDLNet_GetError());
     return -1;
  }
  if(!(Global.pack_in=SDLNet_AllocPacket(65535)))
  {
     fprintf(stderr, "SDLNet_AllocPacket: %s\n",SDLNet_GetError());
     return -1;
  }

  // bind server address to channel 0
  if(SDLNet_UDP_Bind(Global.net_socket, 0, &Global.net_server_ip)==-1)
  {
     fprintf(stderr, "SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
     return -1;
  }

  std::cout << "Packet Header  : " << (sizeof(packet_header))  << std::endl;
  std::cout << "Object Transfer: " << (sizeof(object_transfer))<< std::endl;

  // Start Network Dispatch Thread
  net_thread = thread_create(net_start_thread, NULL, "net_start_thread()", &net_thread_stop);
  if ( net_thread < 0 ) {
    fprintf(stderr, "Unable to create network thread: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}


void net_send_telemetry(void){

  if(!Global.ship)
    return;

  // Cast packet_header struct over out packet
  packet_header   *head   = (packet_header*)((char *)Global.pack_out->data);

  head->type = SDL_SwapBE16(1);   // Type of packet  (1 = object transfer)
  head->misc = SDL_SwapBE16(1);   // Misc (Num of objects...)


  // Cast object_transfer struct over out packet after header struct
  object_transfer *object = (object_transfer *)((char *)Global.pack_out->data + sizeof(packet_header));

  strncpy(object->name, Global.net_handle.c_str(),9);
  object->name[9] = '\0';

  strncpy(object->pad, "XOXOXOXOXOXO",9);
  object->pad[9] = '\0';

  net_pack(object->location,    Global.ship->location);
  net_pack(object->velocity,    Global.ship->velocity);
  net_pack(object->acceleration,Global.ship->acceleration);

  net_pack(object->attitude,    Global.ship->attitude);


  Global.pack_out->len = sizeof(packet_header) + sizeof(object_transfer);


  if(udpsend(Global.net_socket,0,Global.pack_out)<1)
     std::cout << "ERROR!/n";

}

void net_recv_telemetry(void){

  int  offset       = 0;

  // Cast packet_header struct over out packet
  packet_header   *head   = (packet_header*)((char *)Global.pack_in->data);

  offset += sizeof(packet_header);

  if (SDL_SwapBE16(head->type) == 1)
  {
      Global.net_ships = SDL_SwapBE16(head->misc);

      for(int i = 0; i < Global.net_ships; i++)
      {
          // Cast object_transfer struct over out packet after header struct
          object_transfer *object = (object_transfer *)((char *)Global.pack_in->data + offset);

          Global.net_ship[i].name = "-" + std::string(object->name);

          //std::cout << object->name << "\n";

          //strncpy(object->pad, "XOXOXOXOXOXO",9);
          //object->pad[9] = '\0';

          net_unpack(Global.net_ship[i].location,     object->location);
          net_unpack(Global.net_ship[i].velocity,     object->velocity);
          net_unpack(Global.net_ship[i].acceleration, object->acceleration);

          net_unpack(Global.net_ship[i].attitude,     object->attitude);

          offset += sizeof(object_transfer);
      }
  }
}


int net_start_thread(void *data)
{

  unsigned int delay_send = 0;

  for(;net_thread_stop >= 0;)
  {
    if (delay_send > 4);
    {
       net_send_telemetry();
       delay_send = 0;
    }

    while(udprecv(Global.net_socket, Global.pack_in) == 1)
      net_recv_telemetry();

    SDL_Delay(10);  delay_send++;
  }


  // OK, received terminate signal, now cleanup.
  SDLNet_FreePacket(Global.pack_out);
  SDLNet_FreePacket(Global.pack_in);

  SDLNet_Quit();

  return 0;
}