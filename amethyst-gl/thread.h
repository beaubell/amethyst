#ifndef AMETHYST_CLIENT_THREAD_H
#define AMETHYST_CLIENT_THREAD_H

/***********************************************************************
 Amethyst-GL
  - Thread function prototypes

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

namespace amethyst {
namespace client {


#define  SF_THREAD_CREATION_FAILED -1;

typedef int SF_Thread_Id;

#if 0
SF_Thread_Id  thread_create (int (*func_ptr)(void*), void *data, const char *name, int *control);
void          thread_stop_all(void);
#endif

} // namespace client
} // namespace amethyst


#endif /* AMETHYST_CLIENT_THREAD_H */
