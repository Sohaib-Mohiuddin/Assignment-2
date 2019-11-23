#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    // Variable initialization
    int sock, n, buffer1;
    struct sockaddr_in server;
    char message[1000], server_reply[2000], buffer[1024];

    // Created socket and assigned it to variable sock
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) 
    {
        printf("Could not create socket");
    }
    puts("Socket Created");

    // Creating connection address and setting it to "127.0.0.1", creating connection port and setting it to "8888"
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");

    // Infinite while loop for client-server interaction
    while(1)
    {
        printf("Enter Expression: ");
        // Take user input and save the input string into variable "message"
        fgets(message, 1000, stdin);

        // Write "message" to the server
        write(sock, &message, sizeof(message));

        if (send(sock, message, strlen(message), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        // Reading a string from server
        n = read(sock, buffer, 255);
        if (n < 0)
            perror("Error reading from socket");
        
        // Printing the server message
        printf("Server - %s", buffer);

        memset(buffer, 0, 1024);
        memset(message, 0, 1000);
        memset(server_reply, 0, 2000);
        fflush(stdin);
	    fflush(stdout);
    }
    close(sock);
    return 0;
}