CFLAGS=-O3 -Wall -I../common -I. -fPIC

LIBS=-lpthread -lm
OBJS=fifo.o server.o glclient.o
EXES=samples/sample1 samples/sample2 gls.so

all: $(EXES)

fifo.o: ../common/fifo.c ../common/fifo.h
	gcc -c -o fifo.o $(CFLAGS) ../common/fifo.c

server.o: ../common/server.c ../common/server.h fifo.o
	gcc -c -o server.o $(CFLAGS) ../common/server.c

glclient.o: glclient.c glclient.h ../common/gls_command.h server.o
	gcc -c -o glclient.o $(CFLAGS) glclient.c

gls.so: $(OBJS) glsshared.c
	gcc -shared -o gls.so $(CFLAGS) glsshared.c $(OBJS) -Wl,--no-undefined -pthread -ldl

samples/glsutil.o: samples/glsutil.c samples/glsutil.h
	gcc -c -o samples/glsutil.o $(CFLAGS) samples/glsutil.c

samples/sample1: samples/sample1.c glclient.o samples/glsutil.o
	gcc -o samples/sample1 $(CFLAGS) samples/sample1.c samples/glsutil.o $(OBJS) $(LIBS)

samples/sample2: samples/sample2.c glclient.o samples/glsutil.o
	gcc -o samples/sample2 $(CFLAGS) samples/sample2.c samples/glsutil.o $(OBJS) $(LIBS)

clean:
	rm $(EXES) $(OBJS)
