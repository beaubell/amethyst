/***************************************************************************
 *   Copyright (C) 2005 by Beau V.C. Bellamy                               *
 *   beau@autolfs.org                                                      *
 ***************************************************************************/

#include <iostream>

#include "SDL/SDL_net.h"


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

  std::cout << SDLNet_UDP_Open(0) << std::endl;

}