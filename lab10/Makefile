threads: threads.o
	gcc -Wall -o threads threads.o ./libgcc.a -lpthread
threads.o: threads.c
	gcc -Wall -c -o threads.o threads.c