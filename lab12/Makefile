.PHONY: clean

RM = rm -f	

webserv : socklib.o webserv.o
	gcc webserv.o socklib.o -o webserv 

socklib.o: socklib.c
	gcc socklib.c -c

webserv.o : webserv.c
	gcc webserv.c -c

clean: 
	$(RM) *.o