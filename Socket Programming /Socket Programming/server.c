#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    int iCnt = 0;
    
    char sendBuff[1025];
    
    // Structure to hold the timing information
    time_t ticks;

    // Create unnamed socket inside Kernel and return socket descriptor
    listenfd = socket(AF_INET,          // Domain or Family name AF_INET is for IPv4
                            SOCK_STREAM,    // Transport layer protocol (TCP)
                            0);         // Default protocol by kernel
    
    printf("Marvellous Server : socket created succefully with FD : %d\n",listenfd);
    
    // Zeroout the memory of serv_addr
    memset(&serv_addr, '0', sizeof(serv_addr));
    
    // Zeroout the memory of sendBuff
    memset(sendBuff, '0', sizeof(sendBuff));

    // Initialise the members of sockaddr_in structure
    /*
     // IPv4 AF_INET sockets:
     
     struct sockaddr_in
     {
         short            sin_family;   // e.g. AF_INET, AF_INET6
         unsigned short   sin_port;     // e.g. htons(3490)
         struct in_addr   sin_addr;     // see struct in_addr, below
         char             sin_zero[8];  // zero this if you want to
     };
     
     struct in_addr
     {
         unsigned long s_addr;          // load with inet_pton()
     };
     
     */
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // IP address
    serv_addr.sin_port = htons(5000);   // Port Number

    // Assign the above information to socket
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("Marvellous Server : Binding succefully\n");
    
    listen(listenfd,        // FD of socket
           10);                // Maximum number of clients

    printf("Marvellous Server :Server is ready to listen\n");

    // Waiting for client request
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        printf("Marvellous Server : Client request accepted succesfully with number : %d\n",iCnt);

        ticks = time(NULL);
        
        // Form a string to send to the server
        snprintf(sendBuff, sizeof(sendBuff), "Marvellous Server @ %.24s\r\n", ctime(&ticks));
        
        // Sends data to client
        write(connfd, sendBuff, strlen(sendBuff));

        printf("Marvellous Server : Data succesfully sent to client\n");

        close(connfd);  // Close the client connection
        
        printf("Marvellous Server : Connection with client closed\n");

        sleep(1);   // sleep for one second

        printf("Marvellous Server : Waiting for another client\n");

        iCnt++;
     }
}
