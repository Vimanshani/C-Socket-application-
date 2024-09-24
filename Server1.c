#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <ws2tcpip.h>





void error(const char * msg){
    perror(msg);
    exit(1);
}
int main (int argc,char * argv[]){
// argc is the total number of parameters(file name,port number)
//and argv contains the orginal arguments in cmd arv[0] for file name and argv[1] for port number
    if(argc < 2){
        fprintf(stderr , "Port number not provided");
        exit(1);
    }
     WSADATA wsaData;  // Windows-specific socket initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit (1);
    }
    int  portno, n;
    char buffer[250];
    SOCKET sockfd, newsockfd;
    struct sockaddr_in serv_addr , cli_addr;
    int clilen;
    

    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd <0){
        error("error opening socket.");
    }

    memset((char *)&serv_addr , 0 ,sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd , (struct sockaddr * )&serv_addr , sizeof(serv_addr)) <0)
          error("binding failed.");

    listen(sockfd , 5);
    clilen = sizeof(cli_addr);
    // accepting connection
    newsockfd = accept(sockfd , (struct sockaddr *) &cli_addr , &clilen);

    if(newsockfd <0)
       error("Error on accept.");
    while(1){
        memset(buffer ,0, sizeof(buffer));
         
        n= recv(newsockfd , buffer ,sizeof(buffer)-1,0);
        if(n<0){
           error("Error on reading.");
        }
        printf("Client : %s\n", buffer);
        memset(buffer ,0, sizeof(buffer));
        fgets(buffer , 250 , stdin);
       buffer[strcspn(buffer, "\n")] = '\0';

        n= send(newsockfd , buffer , strlen(buffer),0);
        if(n<0)
          error("Error on writing.");
        int i = strncmp("Bye" , buffer , 3);
        if(i==0)
           break;

    }
    closesocket(newsockfd);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
