#ifndef AMETHYST_CLIENT_FILE_H
#define AMETHYST_CLIENT_FILE_H

/***************************************************************************
 *   FILE Routines                                                         *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/

#ifdef WIN32
#include <windows.h>
#include <io.h>

# ifndef R_OK
#  define R_OK  4               /* Test for read permission.  */
#  define W_OK  2               /* Test for write permission.  */
#  define X_OK  1               /* Test for execute permission.  */
#  define F_OK  0               /* Test for existence.  */
# endif

#endif

#include <stdio.h>  // fopen, scanf, printf, fclose, feof

#endif /* AMETHYST_CLIENT_FILE_H */
