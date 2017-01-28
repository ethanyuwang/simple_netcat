#include "snc_client.h"


int sockfd;


void start_client(){
	printf("client is running\n");

	/* initialize a socket as UDP or TCP */
	if(opt.udp)
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1)
        internalError("socket");

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(opt.port);

    int status = inet_pton(AF_INET, opt.hostname, &servaddr.sin_addr);
    if(status == -1)
        internalError("inet_pton");
    else if(status == 0){/* hostname given not IP */
        //TODO
        /* get IP from hostname
         * but for now print error and exit
         */
        internalError("hostname not supported\ngive IP-address\n");
    }

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        internalError("connect");

    char sendline[MAX], recvline[MAX];
    
    fd_set rset;
    int maxfdp1;
    int bytes;

    FD_ZERO(&rset);
    while(1){
        FD_SET(fileno(stdin), &rset);
        FD_SET(sockfd, &rset);

        maxfdp1 = (fileno(stdin) < sockfd ? sockfd : fileno(stdin)) + 1;

        if(select(maxfdp1, &rset, NULL, NULL, NULL) == -1)
            internalError("select");

        if(FD_ISSET(sockfd, &rset)){
            bzero(recvline, MAX);
            bytes = read(sockfd, recvline, MAX);

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
            else if(bytes == 0)
                break;

            if(write(sockfd, sendline, strlen(sendline)) == -1)
                internalError("write");
        }
    }/* while */

    close(sockfd);
}