rwmain: main.o readwriter.o
	gcc -o rwmain main.o readwriter.o -lpthread

main.o: main.c readwriter.c readwriter.h
	gcc -c main.c -lpthread

readwriter.o: readwriter.c readwriter.h
	gcc -c readwriter.c
