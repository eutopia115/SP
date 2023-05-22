.PHONY: clean

RM = rm -f	

rlsd : rlsd.o
	gcc -o rlsd rlsd.o

rls : rls.o
	gcc -o rls rls.o

rlsd.o: rlsd.c
	gcc -c rlsd.c

rls.o:rls.c
	gcc -c rls.c

clean: 
	$(RM) *.o