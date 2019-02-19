CFLAGS=-O3 -Wall -I../common

LIBS=-lpthread -lm
OBJS=fifo.o server.o glclient.o glsutil.o
EXES=sample1 sample2

all: $(EXES)

fifo.o: ../common/fifo.c ../common/fifo.h
	gcc -c -o fifo.o $(CFLAGS) ../common/fifo.c

server.o: ../common/server.c ../common/server.h fifo.o
	gcc -c -o server.o $(CFLAGS) ../common/server.c

glclient.o: glclient.c glclient.h ../common/gls_command.h server.o
	gcc -c -o glclient.o $(CFLAGS) glclient.c

glsutil.o: glsutil.c glsutil.h
	gcc -c -o glsutil.o $(CFLAGS) glsutil.c

sample1: sample1.c glclient.o glsutil.o
	gcc -o sample1 $(CFLAGS) sample1.c $(OBJS) $(LIBS)

sample2: sample2.c glclient.o glsutil.o
	gcc -o sample2 $(CFLAGS) sample2.c $(OBJS) $(LIBS)

clean:
	rm $(EXES) $(OBJS)
