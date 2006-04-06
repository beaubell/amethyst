/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/

#include "object.h"

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

using namespace amethyst;

//void init_net(void)
//{



//}
static void
bail (const char *on_what) {
    fputs(strerror(errno),stderr);
    fputs(": ",stderr);
    fputs(on_what,stderr);
    fputc('\n',stderr);
    exit(1);
}



int main(int argc, char** argv)
{
    int z;
    char *srvr_addr = NULL;
    struct sockaddr_in adr_inet; /* AF_INET */
    struct sockaddr_in adr_clnt; /* AF_INET */
    int len_inet;                /* Length */
    int s;                       /* socket */
    char dgram[512];             /* Recv Buffer */
    char dtfmt[512];             /* Date/Time Result */
    time_t td;                   /* Current Time and Date */
    struct tm tm;                /* Date time values */


    srvr_addr = "127.0.0.1";

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
       z = recvfrom(s,             // Socket
                    dgram,         // Recieve buffer
                    sizeof dgram,  // Max Buffer
                    0,             // Flags: No Options
                    (struct sockaddr *)&adr_clnt,  // Addr
                    (socklen_t*)&len_inet);
       if (z < 0)
           bail("recvfrom(2)");

       printf("MSG FROM: %s\n", inet_ntoa(adr_clnt.sin_addr));

    }

    return 0;
}
