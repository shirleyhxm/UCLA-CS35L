OPTIMIZE = -O2

CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

all: randcpuid.o, randlibhw.so, randlibsw.so, randmain

randcpuid.o:
	$(CC) $(CFLAGS) -c randcpuid.c 

randlibhw.so:		
	$(CC) $(CFLAGS) -c -fPIC -o randlibhw.o randlibhw.c
	$(CC) $(CFLAGS) -shared -o randlibhw.so randlibhw.o
randlibsw.so:
	$(CC) $(CFLAGS) -c -fPIC -o randlibsw.o randlibsw.c
	$(CC) $(CFLAGS) -shared -o randlibsw.so randlibsw.o
randmain:
	$(CC) $(CFLAGS) -c randcpuid.c -o randcpuid.o
	$(CC) $(CFLAGS) -c randmain.c -o randmain.o
	$(CC) $(CFLAGS) randcpuid.o randmain.o -ldl -Wl,-rpath=$(PWD) -o randmain 
