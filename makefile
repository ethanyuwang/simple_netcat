CC= gcc
CFLAGS= -Wall
BINARY= snc

default:	snc_core.o snc_client.o snc_server.o snc_driver.o
	$(CC) $(CFLAGS) -o $(BINARY) $^

.o.c:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o
	-rm -f snc