/***************************************************************************
 *   Copyright (C) 2005 by Beau V.C. Bellamy                               *
 *   beau@autolfs.org                                                      *
 ***************************************************************************/

#include "net.h"

#include <iostream>

//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <unistd.h>
#include "SDL/SDL_net.h"

#include "global.h"

int udpsend(UDPsocket sock, int channel, UDPpacket *out)
{
	//Uint32 t,t2;
	//int err;
	
	//t=SDL_GetTicks();

	if(!SDLNet_UDP_Send(sock, channel, out))
	{
		printf("SDLNet_UDP_Send: %s\n",SDLNet_GetError());
		exit(1);
	}

	return(1);
}


void net_test(void)
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

  //UDPsocket sock;
  //IPaddress ip;
  //UDPpacket *in, *out;
  //Uint16 port = strtol("23232",NULL,0);
  //char *host = "localhost";

  if(SDLNet_ResolveHost(&Global.net_server_ip,Global.net_server.c_str(),Global.net_port)==-1)
  {
	printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
	exit(4);
  }

  // open udp client socket
  if(!(Global.net_socket = SDLNet_UDP_Open(0)))
  {
        printf("SDLNet_UDP_Open: %s\n",SDLNet_GetError());
        exit(5);
  }

  // allocate max packet
  if(!(Global.pack_out=SDLNet_AllocPacket(65535)))
  {
	printf("SDLNet_AllocPacket: %s\n",SDLNet_GetError());
	exit(6);
  }
  if(!(Global.pack_in=SDLNet_AllocPacket(65535)))
  {
	printf("SDLNet_AllocPacket: %s\n",SDLNet_GetError());
	exit(6);
  }

  // bind server address to channel 0
  if(SDLNet_UDP_Bind(Global.net_socket, 0, &Global.net_server_ip)==-1)
  {
        printf("SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
        exit(7);
  }

  // request file / expect filesize
  //out->data[0]=1<<4;
  //strcpy((char *)out->data+1,"FOOOOOO");
  //out->len=strlen("FOOOOOO")+2;
  //if(udpsend(Global.net_socket,0,out,in,200,1)<1)
  //   exit(9);


  //int numsent = SDLNet_UDP_Send(socket, packet.channel, &packet);
  //std::cout << numsent;

  std::cout << "\nPacket Header  : " << (sizeof(packet_header));
  std::cout << "\nObject Transfer: " << (sizeof(object_transfer));

  net_send_telematry();
}


void net_send_telematry(void){

  // Cast packet_header struct over out packet
  packet_header   *head   = (packet_header*)((char *)Global.pack_out->data);

  head->type = 1;   // Type of packet  (1 = object transfer)
  head->misc = 1;   // Misc (Num of objects...)


  // Cast object_transfer struct over out packet after header struct
  object_transfer *object = (object_transfer *)((char *)Global.pack_out->data + sizeof(packet_header));

  strncpy(object->name, Global.net_handle.c_str(),8);
  object->name[9] = '\0';

  strncpy(object->pad, "XOXOXOXOXOXO",8);
  object->pad[9] = '\0';

  object->location     = Global.ship.location;
  object->velocity     = Global.ship.velocity;
  object->acceleration = Global.ship.acceleration;

  object->attitude     = Global.ship.attitude;


//  out->data[0]=1<<4;
//  strcpy((char *)out->data+1,"FOOOOOO");

    Global.pack_out->len = sizeof(packet_header) + sizeof(object_transfer);


    if(udpsend(Global.net_socket,0,Global.pack_out)<1)
       std::cout << "ERROR!/n";

}

