# CS4220-Project-1-OpenSSL-Secure-Transmissions

NOTE: The server and clientuse the Winsock library. When you go to compile server.c and client.c, you must include the -lws2_32 flag so the linker knows to use the Windows Socket 2 library. If you do not do this, you will encounter a multitude of errors.
Examples: "gcc server.c -o server -lws2_32"
          "gcc client.c -o client -lws2_32"

## Instructions for server

Feel free to modify the port and buffer size as necessary.

## Instructions for client

Choose the IP address of your device.

