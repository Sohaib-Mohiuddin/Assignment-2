#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    // Initialize variables
    int socket_desc, client_sock, c, read_size, n;
    struct sockaddr_in server, client;
    char client_message[2000];
    char * error_message = "Error Occurred.";

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("could not create socket");
    }
    puts("Socket created");

    // Creating connection address and setting it to any address, creating connection port and setting it to "8888"
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listening for any incoming connection from a client
    listen(socket_desc, 3);

    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    // If the client has connected to the same port, server checks and accepts client connection
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("Accept failed");
        return 1;
    }
    puts("Connection accepted");

    // While loop that will run so long there is a response from the client
    while((read_size = recv(client_sock, client_message, 2000, 0)) > 0) // (read(client_sock, &client_message, sizeof(client_message))) > 0
    {
        int n1, n2, result;
        int i = 0;
        char operation[4] = "";
        char num1[30] = "";
        char num2[30] = "";
        char sum[30];

        // Read client string and save it to "client_message"
        read(client_sock, &client_message, sizeof(client_message));
        
        // This while loop iterates through "client_message" and saves all characters before a '<space>' to the variable "operation"
        while(client_message[i]!= ' ')
        {
            // Each character is appended to "operation" till a '<space>' is reached
            strncat(operation, &client_message[i], 1);
            i++;
        }
        // Increment passed the '<space>'
        i++;

        // This while loop iterates through the "client_message" from the position of "i" from after the previous while loop
        // till another '<space>' is reached and then saves the value to "num1"        
        while(client_message[i]!= ' ')
        {
            // Each character is appended to "num1" till a '<space>' is reached
            strncat( num1, &client_message[i], 1);
            i++;
        }
        // Increment passed the '<space>'
        i++;

        // Format the value in "num1" to integer and save it to "n1"
        sscanf(num1, "%d", &n1);
        
        while(client_message[i]!= ' ')
        {
            // Each character is appended to "num2" till a '<space>' is reached
            strncat( num2, &client_message[i], 1);
            i++;
        }
        
        // Format the value in "num2" to integer and save it to "n2"
        sscanf(num2, "%d", &n2);
        
        // Each if statement compares the characters in "operation" to "add/sub/div/mul" and if matched with any of those,
        // the program enters that if statement
        if (!strcmp(operation, "add"))
        {
            // Save the value of n1 + n2 to "result"
            result = n1 + n2;

            // Format "result" from integer and save to character array "sum"
            sprintf(sum, "%d\n", result);

            // Write the "sum" back to the client
            n = write(client_sock, &sum, strlen(sum));
            if (n < 0)
                perror("Error writing to the socket");
        }
        else if (!strcmp(operation, "sub"))
        {
            // Save the value of n1 - n2 to "result"
            result = n1 - n2;

            // Format "result" from integer and save to character array "sum"
            sprintf(sum, "%d\n", result);

            // Write the "sum" back to the client
            n = write(client_sock, &sum, strlen(sum));
            if (n < 0)
                perror("Error writing to the socket");
        }
        else if (!strcmp(operation, "div"))
        {
            if (n2 == 0)
            {
                n = write(client_sock, "Cannot divide by 0, try again.", strlen("Cannot divide by 0, try again."));
                if (n < 0)
                    perror("Error writing to the socket");
            }
            // Save the value of n1 / n2 to "result"
            result = n1 / n2;

            // Format "result" from integer and save to character array "sum"
            sprintf(sum, "%d\n", result);

            // Write the "sum" back to the client
            n = write(client_sock, &sum, strlen(sum));
            if (n < 0)
                perror("Error writing to the socket");
        }
        else if (!strcmp(operation, "mul"))
        {
            // Save the value of n1 * n2 to "result"
            result = n1 * n2;

            // Format "result" from integer and save to character array "sum"
            sprintf(sum, "%d\n", result);

            // Write the "sum" back to the client
            n = write(client_sock, &sum, strlen(sum));
            if (n < 0)
                perror("Error writing to the socket");
        }
        memset(operation, 0, 4);
        memset(num1, 0, 30);
        memset(num2, 0, 30);
        memset(sum, 0, 5);
        memset(client_message, 0, 2000);
        fflush(stdin);
	    fflush(stdout);
    }

    // If the response from client is an empty message, read_size equals 0 and client will be disconnected
    if (read_size == 0)
    {
        puts("client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
    return 0;
}