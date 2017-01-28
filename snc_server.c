#include "snc_server.h"

int sockfd;

void start_server() {
	printf("server is running\n");

    int listenfd = 0;
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    char recvBuff[1024], sendBuff[1024];
	
	/* initialize a socket as UDP or TCP */
	if(opt.udp)
        listenfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    else
        listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenfd == -1) {
        internalError("Socket failed to initialize");
        endProgram("Socket failed to initialize");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    /* if hostname is provided use it */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(opt.port);
    if (opt.hostname!=NULL) {
        if (inet_pton(AF_INET, opt.hostname, &serv_addr.sin_addr)<0) {
            internalError("inet_pton failed");
        }
        //serv_addr.sin_addr.s_addr = inet_addr(opt.hostname);
    }
    else {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }


    if(bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr))<0) {
        internalError("Failed to bind");
    } 

    if(listen(listenfd, 5) == -1) {
        internalError("Socket failed to listen");
    }
    
    while (1) {
        struct sockaddr_in cliaddr;
        int clifd, clilen = sizeof(cliaddr);
        /* accept a connection */
        if((clifd = accept(listenfd, (struct sockaddr *)&cliaddr, (socklen_t *)&clilen)) == -1) {
            internalError("accept failed");
        }

        fd_set rset;
        int maxfdp1;
        int bytes;

        FD_ZERO(&rset);

        //if(!opt.udp){
        while(1){
            FD_SET(fileno(stdin), &rset);
            FD_SET(clifd, &rset);

            maxfdp1 = (fileno(stdin) < clifd? clifd : fileno(stdin)) + 1;

            if(select(maxfdp1, &rset, NULL, NULL, NULL) == -1) {
                internalError("select failed");
            }
            if(FD_ISSET(clifd, &rset)){
                bzero(recvBuff, 1024);
                bytes = read(clifd, recvBuff, 1024);

                if(bytes == -1) {
                    internalError("read failed");
                }
                else if(bytes == 0) {
                    break;
                }

                fputs(recvBuff, stdout);
            }
            if(FD_ISSET(fileno(stdin), &rset)){
                bzero(sendBuff, 1024);
                bytes = read(fileno(stdin), sendBuff, 1024);

                if(bytes == -1) {
                    internalError("read failed");
                }

                if(write(clifd, sendBuff, strlen(sendBuff)) == -1) {
                    internalError("write failed");
                }
            }
        }
        //}

        /*if (opt.udp) {
        while(1){

            printf("Waiting for data...");
            fflush(stdout);

            FD_SET(fileno(stdin), &rset);
            FD_SET(clifd, &rset);

            //try to receive some data, this is a blocking call
            if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
            {
                die("recvfrom()");
            }

            //print details of the client/peer and the data received
            //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
            printf("Data: %s\n" , buf);

            if(FD_ISSET(fileno(stdin), &rset)){
                bzero(sendBuff, 1024);
                bytes = read(fileno(stdin), sendBuff, 1024);

                if(bytes == -1) {
                    internalError("read failed");
                }

                //now reply the client with the same data
                if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
                {
                    printf("%s\n", );("sendto()");
                }
            }

        }}*/
    /* close connection */
    close(clifd);
    }
    endProgram("Connection closed");
}

