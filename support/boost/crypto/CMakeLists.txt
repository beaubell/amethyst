

INCLUDE_DIRECTORIES(include ../../)

SET(libSHA2_SRCS
  ./lib/sha2_ctx.cpp
)

ADD_LIBRARY(libsha2 STATIC ${libSHA2_SRCS})

