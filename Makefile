CC = gcc
SERVER_SRC = server.c
CLIENT_SRC = client.c
SERVER_OUT = server.exe
CLIENT_OUT = client.exe
CFLAGS = -Wall -Wextra -g

OPENSSL_INCLUDE = # path to \Include
OPENSSL_LIB = # path to \lib
OPENSSL_CFLAGS = -I$(OPENSSL_INCLUDE)
OPENSSL_LIBS = -L$(OPENSSL_LIB) -lssl -lcrypto -lws2_32


all: $(SERVER_OUT) $(CLIENT_OUT)
$(SERVER_OUT): $(SERVER_SRC)
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -o $@ $< $(OPENSSL_LIBS)
$(CLIENT_OUT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) $(OPENSSL_CFLAGS) -o $@ $< $(OPENSSL_LIBS)


clean:
	del /Q *.exe *.o
