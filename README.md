## CS4220 OpenSSL Secure Transmissions

Group members: Joel-Ira Willis, Tara Prasad

We have neither given nor received unauthorized assistance on this work.


This project consists of a client and server written in C. It utilizes the OpenSSL library to implement SHA-256, along with the following features:

-	A TLS handshake that performs mutual authentication.
-	Only supports TLS 1.2+.
-	Utilizes digital certificates: One server certificate signed by a private key and associated with the server’s hostname, and another client certificate signed with a client-side private key.
-	Ensures the secure exchange of secret keys.
-	Uses HMAC to ensure data integrity

The client and server instructions have been adapted for the Windows installation and setup.



## Prerequisites

# 1.)	Installing OpenSSL.

To install OpenSSL, visit the official website and follow the installation instructions.
https://www.openssl.org/ 
Once you have installed OpenSSL, ensure you configure the PATH variable.

# 2.)	Generate certificates and keys

Ensure you have completed step one, and that OpenSSL is properly configured for use. As you progress through these steps, enter the relevant information when prompted to do so.
a)	Begin by generating a CA (certificate authority) with the following command:
openssl genrsa -out ca.key 4096
openssl req -x509 -new -nodes -key ca.key -sha256 -days 3650 -out ca.pem

b)	Generate the server key and a CSR (certificate signing request).
openssl genrsa -out server-key.pem 2048
openssl req -new -key server-key.pem -out server.csr

c)	Sign the server CSR with the CA.
openssl x509 -req -in server.csr -CA ca.pem -CAkey ca.key -CAcreateserial \
  -out server-cert.pem -days 365 -sha256

d)	Generate a client key and a CSR.
openssl genrsa -out client-key.pem 2048
openssl req -new -key client-key.pem -out client.csr

e)	Sign the client CSR with the CA.

openssl x509 -req -in client.csr -CA ca.pem -CAkey ca.key -CAcreateserial \
  -out client-cert.pem -days 365 -sha256

f)	(Optional, but highly recommended) Verify that both certificates have been successfully signed by the CA. The output should read “server-cert.pem: OK” and “client-cert.pem: OK” for the server and client certificates respectively.

openssl verify -CAfile ca.pem server-cert.pem

openssl verify -CAfile ca.pem client-cert.pem

g)	Place all the files generated from the above steps into the project’s root directory. The server will not work if these files are not within the same directory as your client and server files.


# 3.)	Compile the program. Run the following command to compile the .c files into executables.

cl server.c /I"C:\Path_to\include>" /link "C:\Path_tolib\libssl.lib" "C:\Path_to\lib\libcrypto.lib" ws2_32.lib

cl server.c /I"C:\Path_to\include>" /link "C:\Path_tolib\libssl.lib" "C:\Path_to\lib\libcrypto.lib" ws2_32.lib

Once completed, you should have two executable files.

# 4.)	To run the server, ensure that the server is running first. It should read in the terminal that the server is listening on the port of your choice. Once the server is running, the client application can be launched, and you should be able to communicate with the server. To end transmissions, you can utilize Ctrl + C in either window, or type “exit”.



## For server:
	
	- Feel free to modify port and buffer size as necessary.

## For client: 

	- Choose the IP address of your device.


## Project Notes:

	- Challenges included compiling and testing the program, as one of the group members did not have the proper Windows machine. However, the code itself ran, and the server and client were able to connect properly. There was a lot of guess-work involved when writing the client code due to it being written on a MacBook. This was solved by installing a virtual machine and incorporating Visual Studio there, although this did not work well when it came to testing the program. In the end, the group member with the Windows machine was able to test both files on the same device to verify its execution.
	Another significant challenge was the actual setup of OpenSSL on Windows. For this reason, some the testing was done by using WSL (Windows Subsystem for Linux), as the command line interactions and OpenSSL installation are far more straightforward on Linux systems than on Windows.

	- Concepts learned during this process include working with Winsock, working with OpenSSL using the command line, troubleshooting connections, implementing error handling, and creating digital certificates.

## Sources used:

	"Creating a Self-Signed Certificate with OpenSSL"
	https://www.baeldung.com/openssl-self-signed-cert

	"OpenSSL"
	https://www.openssl.org/

	"Socket Programming Tutorials in C for Beginners"
	https://www.youtube.com/watch?v=LtXEMwSG5-8&list=PLDmvslp_VR0zCLnQZObYMZ_uSjmXLoRxl
