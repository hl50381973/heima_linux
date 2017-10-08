

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm01_pipe dm02_pipe dm03_pipe dm04_pipe dm05_pipe_closew dm06_capcity dm07_pipebuf
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




