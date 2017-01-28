#include "snc_server.h"

int sockfd;


void start_server(){
	printf("server is started\n");
	
	/* initialize a socket as UDP or TCP */
	if(opt.udp)
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
        internalError("Socket failed to initialize");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(opt.port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        internalError("bind");

    if(listen(sockfd, 5) == -1)
        internalError("listen");

    struct sockaddr_in cliaddr;
    int clifd, clilen = sizeof(cliaddr);
    /* accept a connection */
    if((clifd = accept(sockfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen)) == -1)
        internalError("accept");

    /* print client address and port number when a new connection
       request is received */
    char str[INET_ADDRSTRLEN];
    unsigned short p;

    char sendline[MAX], recvline[MAX];
    fd_set rset;
    int maxfdp1;
    int bytes;

    FD_ZERO(&rset);
    while(1){
        FD_SET(fileno(stdin), &rset);
        FD_SET(clifd, &rset);

        maxfdp1 = (fileno(stdin) < clifd? clifd : fileno(stdin)) + 1;

        if(select(maxfdp1, &rset, NULL, NULL, NULL) == -1)
            internalError("select");

        if(FD_ISSET(clifd, &rset)){
            bzero(recvline, MAX);
            bytes = read(clifd, recvline, MAX);

            if(bytes == -1)
                internalError("read");
            else if(bytes == 0)
                break;

            fputs(recvline, stdout);
        }

        if(FD_ISSET(fileno(stdin), &rset)){
            bzero(sendline, MAX);
            bytes = read(fileno(stdin), sendline, MAX);

            if(bytes == -1)
                internalError("read");

            if(write(clifd, sendline, strlen(sendline)) == -1)
                internalError("write");
        }
    }/* while */

    /* close connection */
    close(clifd);
}
