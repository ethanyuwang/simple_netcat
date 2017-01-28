#include "snc_core.h"


struct options opt;

void options_init(void){
    opt.port = 0;

    /* deafault is client mode */
    opt.listen = 0;
    /* deafault source is not selected */
    opt.source = 0;
    /* default socket type is TCP */
    opt.udp = 0;
}

void options_print(void){
    if (opt.listen == 0)
        printf("client, ");
    else
        printf("listen, ");

    if (opt.source == 1)
        printf("source: ");
    
    if (opt.ip != NULL)
        printf("%s, ", opt.ip);

    if (opt.udp == 0)
        printf("TCP, ");
    else
        printf("UDP, ");

    if (opt.hostname != NULL)
        printf("%s, ", opt.hostname);

    printf("%d\n", opt.port);
}


int hostname_to_ip(char * hostname , char* ip)
{
    struct hostent *he;

    struct in_addr **addr_list;
    int i;
    if ( (he = gethostbyname( hostname ) ) == NULL) {
        herror("gethostbyname");
        return 1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++) {
        strcpy(ip , inet_ntoa(*addr_list[i]));
        return 0;
    }
    return 1;
}


void internalError(const char *format, ...){
    fprintf(stderr, "internal error: ");

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    //fprintf(stderr, "QUITTING.\n");
    exit(EXIT_FAILURE);
}


void die(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}
