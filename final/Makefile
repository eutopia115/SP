.PHONY: clean

RM = rm -f	

Q1 : builtin.o controlflow.o execute2.o process2.o smsh4.o splitline.o varlib.o
	gcc -o Q1 builtin.o controlflow.o execute2.o process2.o smsh4.o splitline.o varlib.o

builtin.o : builtin.c smsh.h varlib.h
	gcc builtin.c -c

controlflow.o : controlflow.c smsh.h
	gcc controlflow.c -c

execute2.o : execute2.c varlib.h
	gcc execute2.c -c

process2.o : process2.c smsh.h
	gcc process2.c -c

smsh4.o : smsh4.c smsh.h varlib.h
	gcc smsh4.c -c
	
splitline.o : splitline.c smsh.h
	gcc splitline.c -c

varlib.o : varlib.c varlib.h
	gcc varlib.c -c

clean: 
	$(RM) *.o