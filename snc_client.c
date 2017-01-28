#include "snc_client.h"


void start_client(){
	printf("client is running\n");

    int sockfd = 0;
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    char recvBuff[1024], sendBuff[1024];

    fd_set rset;
    int maxfdp1;
    int bytes;

    /* initialize a socket as UDP or TCP */
    if(opt.udp)
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    else
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1){
        internalError("Socket failed to initialize");
        endProgram("Socket failed to initialize");
    }

    memset(recvBuff, '0', sizeof(recvBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(opt.port);

    /* if hostname is provided use it */  
    if (opt.hostname!=NULL) {
        if (inet_pton(AF_INET, opt.hostname, &serv_addr.sin_addr)<0) {
            internalError("inet_pton for hostname failed");
        }
    }
    /* if source is provided use it */   
    else if (opt.ip!=NULL) {
        if (inet_pton(AF_INET, opt.ip, &serv_addr.sin_addr)<0) {
            internalError("inet_pton source failed");
        }
        //serv_addr.sin_addr.s_addr = inet_addr(opt.hostname);
    }
    /* defualt case */ 
    else {
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) {
        internalError("Connect Failed");
    }

    FD_ZERO(&rset);
    /*if (opt.udp) {
    while(1)
    {
        printf("Enter message : ");
        gets(message);
         
        //send the message
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            internalError("sendto()");
        }
         
        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(sendBuff,'\0', 1024);
        //try to receive some data, this is a blocking call
        if (recvfrom(s, buf, recvBuff, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            internalError("recvfrom()");
        }
         
        puts(buf);
    }       
    }*/
    //else {
    while(1){
        FD_SET(fileno(stdin), &rset);
        FD_SET(sockfd, &rset);

        maxfdp1 = (fileno(stdin) < sockfd ? sockfd : fileno(stdin)) + 1;

        if(select(maxfdp1, &rset, NULL, NULL, NULL) == -1) {
            internalError("select failed");
        }

        if(FD_ISSET(sockfd, &rset)){
            bzero(recvBuff, 1024);
            bytes = read(sockfd, recvBuff, 1024);

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
            else if(bytes == 0) {
                break;
            }

            if(write(sockfd, sendBuff, strlen(sendBuff)) == -1)
                internalError("write failed");
        }
    }
    //}
    close(sockfd);
    endProgram("Connection closed");
}