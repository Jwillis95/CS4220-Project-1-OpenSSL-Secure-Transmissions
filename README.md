# CS4220-Project-1-OpenSSL-Secure-Transmissions


## Instructions for server

Feel free to modify the port and buffer size as necessary.

NOTE: The server uses the Winsock library. When you go to compile server.c, you must include the -lws2_32 flag so the linker knows to use the Windows Socket 2 library. If you do not do this, you will encounter a multitude of errors.
Example: "gcc server.c -o server -lws2_32"
