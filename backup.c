    /* parse command line args */
    int c;
    while (optind < argc) {
        if ((c = getopt(argc, argv, "ls:u")) !=-1) {
            /* parse command line args */
            switch(c){
                case 'l': o.listen = 1;
                          break;

                case 's': o.source = 1;
                          o.ip = validate_ip(optarg);
                          break;

                case 'u': o.udp = 1;
                          break;

                default: break; //inputError();
            }
        }
        else {
            printf("optind: %d\n", optind);
            printf("argc: %d\n", argc);
            optind++;
            /* mandatory last argument as port number */
            o.port = validate_port(argv[argc-1]);
            /* presume second to the last is hostname, will get checked later*/
            if(optind<argc-1) {
                o.hostname = argv[optind];
            }
            
        }
    }




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



////////////////////////////client



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

    int status = inet_pton(AF_INET, opt.hostname, &servaddr.sin_addr);
    if(status == -1)
        internalError("inet_pton");

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
///////////////////server



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



////////////////////////////client



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

    int status = inet_pton(AF_INET, opt.hostname, &servaddr.sin_addr);
    if(status == -1)
        internalError("inet_pton");

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