#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2)
    {
        printf("P Client : Please enter the IP address of server\n");
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }

    // Zeroout the buffer memory and structure
    memset(recvBuff, '0',sizeof(recvBuff));
    memset(&serv_addr, '0', sizeof(serv_addr));

    // Create unnamed socket inside Kernel and return socket descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    printf("P Client : Socket created succesfully\n");

    // Initialise the members of structure.
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    // Conver the address into numeric bunary format
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    // Connecting to the server
    // 1 : Socket FD
    // 2 : Address
    // 3 : Length of address
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\nP Client Error : Connect Failed \n");
       return 1;
    }

    printf("P Client : Connection succesfully\n");
    
    // Read the data from server
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        
        printf("P Client : Data received from client\n");

        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    }

    if(n < 0)
    {
        printf("\n Read error \n");
    }

    printf("P Client : Connection Closed\n");

    close(sockfd);
    
    return 0;
}

