#ifndef SF_THREAD_H
#define SF_THREAD_H

/***************************************************************************
 *  Starfighter Thread Functions                                           *
 *  (c) 2006 Beau V.C. Bellamy (beau@stellarnetservices.net)               *
 ***************************************************************************/


#define  SF_THREAD_CREATION_FAILED -1;

typedef int SF_Thread_Id;


SF_Thread_Id  thread_create (int (*func_ptr)(void*), void *data, char *name, int *control);
void          thread_stop_all(void);

#endif
