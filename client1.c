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
//and argv contains the orginal arguments in cmd arv[0] for file name and argv[1] for server ip address argv[2] for port number
    int   portno, n;
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent  * server;
    char buffer[260];

    if(argc < 3){
        fprintf(stderr , "usage %s hostname port\n" , argv[0]);
        exit(1);
    }
    
    portno = atoi(argv[2]);
    WSADATA wsaData;  // Windows-specific socket initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }

    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd <0){
        error("error opening socket.");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr , "Error, no such host.");
        

    }
    memset( (char *)&serv_addr , 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *) &serv_addr.sin_addr.s_addr ,(char *)server-> h_addr ,  server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd , (struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0){
         error("Connection failed.");
    }


    while(1){
        memset(buffer ,0, sizeof(buffer));
        fgets(buffer , sizeof(buffer) , stdin);
        n = send(sockfd , buffer ,strlen(buffer),0);
        if(n <0){
           error("Error on writing.");
        }
        memset( buffer , 0, sizeof(buffer));
        n = recv(sockfd , buffer , sizeof(buffer)-1,0);
        if(n<0){
          error("Erroe on reading.");
        }
        printf("Server : %s", buffer);

        int i = strncmp("Bye" , buffer , 3);
        if(i==0)
           break;
    }
    closesocket(sockfd);
    WSACleanup();
    return 0;
}