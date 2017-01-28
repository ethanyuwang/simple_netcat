#ifndef _SNC_CORE_H_
#define _SNC_CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string.h>
#include <strings.h>

#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <netdb.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

/* struct that stores ip address, port, and options */
struct options{
    /* source ip address */
    char *ip;
    /* hostname */
    char *hostname;
    /* Port number */
    unsigned short port;

    /* listen mode or client mode */
    int listen;
    /* source ip address */
    int source;
    /* use UDP instead of TCP */
    int udp;
};


/* global options struct */
extern struct options opt;


/* initialize options struct to default values */
extern void options_init(void);

/* print parsed command line */
extern void options_print(void);

/* resolve hostname to ip */
extern int hostname_to_ip(char * hostname , char* ip);

/* print error message to @stderr*/
extern void internalError(const char *, ...);

/* print message to @stderr and exit */
extern void endProgram(char *msg);

#endif
