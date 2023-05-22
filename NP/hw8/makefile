.PHONY: clean

RM = rm -f	

client : client.c
	gcc -o client client.c

server : server.c
	gcc -o server server.c

mcl : mclient.c
	gcc -o mcl mclient.c

msv : mserver.c
	gcc -o msv mserver.c

clean: 
	$(RM) *.o