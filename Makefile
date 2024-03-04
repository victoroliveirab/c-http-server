CC = gcc
CFLAGS = -g -Wall -Wextra -pthread
SRC = src
OBJ = obj

all: $(OBJ) server

$(OBJ):
	mkdir -p $(OBJ)

$(OBJ)/http.o: $(SRC)/http.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/log.o: $(SRC)/log.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/queue.o: $(SRC)/queue.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ)/thread_pool.o: $(SRC)/thread_pool.c
	$(CC) $(CFLAGS) -c -o $@ $<

server: $(OBJ)/http.o $(OBJ)/log.o $(OBJ)/main.o $(OBJ)/queue.o $(OBJ)/server.o $(OBJ)/thread_pool.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(OBJ) server
