#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]){
    struct sockaddr_in stSockAddr;
    int Res,n;
    char buffer[256];
    char mensaje[256];

    int SocketFD=socket(PF_INET,SOCK_STREAM,0);
    if(-1==SocketFD){
        perror("No se puede crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr,0, sizeof(struct sockaddr_in));//asigna valores en una direccion de memoria
    stSockAddr.sin_family= AF_INET;
    stSockAddr.sin_port= htons(45500);
    Res= inet_pton(AF_INET,"127.0.0.1",&stSockAddr.sin_addr); //ip de servidor, lo pone en sin_addr
    connect(SocketFD,(const struct sockaddr *)&stSockAddr,sizeof(struct sockaddr_in));
    
    do{
        cout<<"Para servidor: ";
        
        cin.getline( buffer, 256);
        n=write(SocketFD,buffer,256);
        if(strcmp("chau",buffer)==0){
            shutdown(SocketFD, SHUT_RDWR);
            close(SocketFD);        
            return 0;
        }

        n=read(SocketFD,buffer,256);
        cout<<"Envió servidor: "<<buffer<<endl;
    } while (buffer!="chau");
    
}
