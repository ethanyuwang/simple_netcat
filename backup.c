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