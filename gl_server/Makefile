CFLAGS=-O3 -Wall -g

LIBS=-L/opt/vc/lib/ -lGLESv2 -lEGL -pthread -lrt -lX11 -g
INCLUDES= -I../common
OBJS=fifo.o glcontrol.o server.o glserver.o

all: gl_server

fifo.o: ../common/fifo.c ../common/fifo.h
	gcc -c -o fifo.o $(CFLAGS) ../common/fifo.c

glcontrol.o: glcontrol.c glcontrol.h
	gcc -c -o glcontrol.o $(CFLAGS) $(INCLUDES) glcontrol.c

server.o: ../common/server.c ../common/server.h fifo.o
	gcc -c -o server.o $(CFLAGS) ../common/server.c

glserver.o: glserver.c glserver.h ../common/gls_command.h glcontrol.o server.o
	gcc -c -o glserver.o $(CFLAGS) $(INCLUDES) glserver.c

gl_server: main.c glserver.o
	gcc -o gl_server $(CFLAGS) $(INCLUDES) main.c $(OBJS) $(LIBS)

clean:
	rm gl_server $(OBJS)
