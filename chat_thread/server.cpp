#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>

using namespace std;
//g++ -std=c++11 server.cpp -o server -pthread

char buffer[256];
    int n;

void f_read(int socket){    
    for(;;){
        bzero(buffer,256);
        do{
            n=read(socket,buffer,256);    
        }while(n==0);
        cout<<"Cliente envió: "<<buffer<<endl;
    }
}

void f_write(int socket){
    for(;;){
        cout<<"Para cliente: ";
        cin.getline(buffer, 256);
        n=write(socket,buffer,256); 
    }
}

int main(int argc, char const *argv[]){
    struct sockaddr_in stSockAddr;
    int SocketFD= socket(PF_INET,SOCK_STREAM,0);

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

    int ConnectFD=accept(SocketFD,NULL,NULL);//connectFD ->socket cliente

    bzero(buffer,256);

    thread t1(f_read,ConnectFD);
    thread t2(f_write,ConnectFD);                               
    
    t1.join();
    t2.join();
    
    shutdown(SocketFD,SHUT_RDWR);
    close(SocketFD);
    
}
