
.PHONY:clean

WORKDIR=.

VPATH = ./src


LIBOBJS=  socketclient.o itcastlog.o
OBJS= demo01_testplatform.o

LIB1 = libmysocket.so
EXE1 = mysocketexe
CC = gcc
CFLGS= -Wall -g -I$(WORKDIR)/inc/
LIBFLAG = -L$(HOME)/lib

all: $(LIB1) $(EXE1)


$(LIB1):$(LIBOBJS)
	$(CC) -shared -fPIC $^ -o $@ 
	cp  $(LIB1) $(HOME)/lib
	mv  $(LIB1) ./lib
	

$(EXE1):$(OBJS)
	$(CC)  $^ $(LIBFLAG) -lmysocket  -o $@


.c.o:
	$(CC) $(CFLGS) -c $< -o $@  -shared -fPIC
clean:
	rm -rf $(BIN) *.o $(HOME)/lib/$(LIB1) ./$(LIB1) 
	

