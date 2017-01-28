#include "snc_core.h"
#include "snc_client.h"
#include "snc_server.h"

/* utility functions */
void inputError(const char *format, ...){
    fprintf(stderr, "invalid or missing options\n");
    fprintf(stderr, "usage: snc [-l] [-u] [-s source_ip_address] [hostname] port\n");

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}


int validate_port(char *p){
    int port = atoi(p);

    if(port < 1025 || port > 65535){
        inputError("Invalid port number: %s\n", p);
    }
    return port;
}

char* validate_ip(char *p){
    struct sockaddr_in sa;
    if(inet_pton(AF_INET, p, &(sa.sin_addr))==0){
        inputError("Invalid source ip address number: %s\n", p);
    }
    return p;
}

/* parse input and execute */
int main(int argc, char *argv[]){
    /* initialize options */
    options_init();

    /* parse command line args */
    int index;
    int c;
    //int argMax = argc;
    //if (strcmp(argv[0]==
    while (index < argc) {
        if ((c = getopt(argc, argv, "ls:u")) !=-1) {
            /* parse command line args */
            switch(c){
                case 'l': opt.listen = 1;
                          break;

                case 's': opt.source = 1;
                          opt.ip = validate_ip(optarg);
                          break;

                case 'u': opt.udp = 1;
                          break;

                default: inputError(" "); //inputError();
            }
            index = optind;
        }
        else {
            /* mandatory last argument as port number */
            opt.port = validate_port(argv[argc-1]);
            /* presume second to the last is hostname, will get checked later*/
            if((argc>2)&&(index<(argc-1))) {
                opt.hostname = argv[index];
            }
            ++index;
        }
    }
    options_print();

    /* validate inputs */
    /* error to use -s with -l */
    if(opt.listen && opt.source){
        inputError("error to use -s with -l\n");
    }

    if (hostname_to_ip(opt.hostname, opt.hostname)){
        inputError("hostname can not be resolved\n");
    }

    options_print();


    /* dispatch server/client */
    if(opt.listen)
        start_server();
    else
        start_client();
    return 0;
}