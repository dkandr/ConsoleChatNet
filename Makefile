CC=g++
#CFLAGS= -g -Wextra -Wall -Werror -Wpedantic
CFLAGS= -g -Wall

install: server client

server: server.cpp message.o user.o net.o
	$(CC) $(CFLAGS) $^ -o $@

client: client.cpp message.o user.o net.o
	$(CC) $(CFLAGS) $^ -o $@

user.o: user.cpp user.h
	$(CC) $(CFLAGS) -c $< -o $@

message.o: message.cpp message.h
	$(CC) $(CFLAGS) -c $< -o $@

net.o: net.cpp net.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o server client