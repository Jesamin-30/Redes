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
    int SocketFD= socket(PF_INET,SOCK_STREAM,0);
    char buffer[256];
    char mensaje[256];
    int n;

    if(-1==SocketFD){
        perror("No se puede crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr,0,sizeof(struct sockaddr_in));

    stSockAddr.sin_family=AF_INET;
    stSockAddr.sin_port=htons(45500);
    stSockAddr.sin_addr.s_addr=INADDR_ANY;

    bind(SocketFD,(const struct sockaddr *)& stSockAddr,sizeof(struct sockaddr_in));
    listen(SocketFD,10);

    for (;;){
        int ConnectFD=accept(SocketFD,NULL,NULL);

        do{
            bzero(buffer,256);
            n=read(ConnectFD,buffer,256);
            cout<<"Cliente envió: "<<buffer<<endl;
            
            if(strcmp("chau",buffer)==0){
                shutdown(ConnectFD, SHUT_RDWR);//LIBERA RECURSOS
                close(ConnectFD); 
                close(SocketFD);
                return 0;       
            }

            cout<<"Para cliente: ";
            cin.getline(buffer, 256);
            n=write(ConnectFD,buffer,256);            
        } while (buffer!="chau");
    }
}
