## CS4220 OpenSSL Secure Transmissions

Group members: Joel-Ira Willis, Tara Prasad

We have neither given nor received unauthorized assistance on this work.

The server and client use the Winsock library. When you go to compile server.c and client.c, you must include the -lws2_32 flag so the linker knows to use the Windows Socket 2 library. If you do not do this, you will encounter a multitude of errors. Examples: "gcc server.c -o server -lws2_32" "gcc client.c -o client -lws2_32"

## For server:
	
	- Feel free to modify port and buffer size as necessary.

## For client: 

	- Choose the IP address of your device.

## General instructions:

	- When using one machine, open two terminal windows and compile the server and client programs in different windows. Run server.c first, followed by client.c. The program should output the data sent by the server on the client-side terminal. When using two machines, use one machine for each program.

## Project Notes:

	- Challenges included compiling and testing the program, as one of the group members did not have the proper Windows machine. However, the code itself ran, and the server and client were able to connect properly. There was a lot of guess-work involved when writing the client code due to it being written on a MacBook. This was solved by installing a virtual machine and incorporating Visual Studio there, although this did not work well when it came to testing the program. In the end, the group member with the Windows machine was able to test both files on the same device to verify its execution.

	- Concepts learned during this process include working with Winsock, working with OpenSSL using the command line, and creating digital certificates.
