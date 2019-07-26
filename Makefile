project:	project.o list.o queue1.o
	gcc project.o list.o queue1.o -Wall -o project
project.o:	project.c list.h queue1.h
	gcc -c project.c -Wall
list.o:		list.c list.h
	gcc -c list.c -Wall
queue1.o:	queue1.c queue1.h
	gcc -c queue1.c -Wall
