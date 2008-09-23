/***********************************************************************
 Amethyst-GL
  - Thread function implementations

 Authors (c):
 2006-2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/


#include <iostream>
#include "SDL_thread.h"

#include "thread.h"

#include <list>


class SF_Thread
{
  public:
    int             handle;
    int            (*function)(void *);
    SDL_Thread     *sdl_thread;
    const char     *name;
    int            *control;
};


class __thread_global
{
  public:
    __thread_global()
    {
       handle_count = 0;
       handle_head  = 0;
    }

    SF_Thread_Id push_thread(SF_Thread &new_thread)
    {
       new_thread.handle = handle_head;
       thread_list.push_back(new_thread);

       handle_count++;
       handle_head++;

       return new_thread.handle;
    }

  public:
    std::list<SF_Thread>  thread_list;
    int handle_count;
    int handle_head;
};


// Instantiate Global Class
__thread_global Thread_Global;



SF_Thread_Id thread_create(int (*func_ptr)(void *), void *data, const char *name, int *control)
{
   printf("Creating Thread: %s...", name);

   // Create Thread
   SDL_Thread *sdl_thread = SDL_CreateThread(func_ptr, data);
   if(!sdl_thread)
   {
       printf("Fail!\n");
       return SF_THREAD_CREATION_FAILED;
   }
   printf("Success!\n");

   // Prepare thread data for insertion into list
   SF_Thread new_thread;
   new_thread.function    = func_ptr;
   new_thread.sdl_thread  = sdl_thread;
   new_thread.name        = name;
   new_thread.control     = control;

   // Push thread unto the list and return thread handle
   return Thread_Global.push_thread(new_thread);
}


void thread_stop_all(void)
{
    // Check if list is empty, return if true;
    if(Thread_Global.thread_list.empty()) return;

    //Make an iterator and point it at the start of our net_object list;
    std::list<SF_Thread>::iterator obj1 = Thread_Global.thread_list.begin();

    do
    {
        printf("Stopping Thread: %s...\n", obj1->name);

        // If control int exists, tell thread to terminate.
        if (obj1->control)
           *(obj1->control) = -1;
        else
           SDL_KillThread(obj1->sdl_thread);

        // Wait for thread to exit.
        SDL_WaitThread(obj1->sdl_thread, NULL);

        obj1++;
    }  while (obj1 != Thread_Global.thread_list.end());

    // Clear thread_list
    Thread_Global.thread_list.clear();

    return;

}
