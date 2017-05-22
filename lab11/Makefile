hive: hive.o
	gcc -Wall -o hive hive.o ./libgcc.a -lpthread
	rm hive.o
bee: bee.o
	gcc -Wall -o bee bee.o ./libgcc.a -lpthread
	rm bee.o
bear: bear.c
	gcc -Wall -o bear bear.c
hive.o: hive.c
	gcc -c hive.c
bee.o: bee.c
	gcc -c bee.c