

SET(amethystserver_SRCS
  main.cpp
)


link_directories (${SDL_LIBRARY})  
ADD_EXECUTABLE(amethyst_server ${amethystserver_SRCS})
TARGET_LINK_LIBRARIES(amethyst_server libamethyst)
