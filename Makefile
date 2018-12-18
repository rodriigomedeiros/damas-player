all: dama

dama: dama.o funcoesDama.o 
	gcc -ggdb dama.o funcoesDama.o -o DamasPlayer -Wall `pkg-config --cflags --libs gtk+-2.0`

dama.o: dama.c
	gcc -ggdb -c dama.c `pkg-config --cflags gtk+-2.0`

funcoesDama.o: funcoesDama.c
	gcc -ggdb -c funcoesDama.c `pkg-config --cflags gtk+-2.0`
	
clean:
	rm *~ *.o DamasPlayer
	
jogar:
	./DamasPlayer
