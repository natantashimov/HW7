CC     = gcc
CCLINK=$(CC)
CFLAGS = -Wall -g -std=c99
OBJS=grades.o
EXEC=libgrades.so

make all: libgrades.so 

libgrades.so: grades.o
	$(CCLINK) -shared grades.o -o libgrades.so  -llinked-list -L.


grades.o: grades.c grades.h linked-list.h 
	$(CCLINK) $(CFLAGS) -c -fPIC grades.c 

clean:
	rm -fr *.o libgrades.so