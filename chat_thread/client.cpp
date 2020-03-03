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
//g++ -std=c++11 client.cpp -o client -pthread

char buffer[256];
int n;

void f_write(int socket){
    for(;;){
        cout<<"Para servidor: ";
        cin.getline( buffer, 256);
        if(strcmp("chau",buffer)==0){
            shutdown(socket, SHUT_RDWR);
            close(socket);  
            return;              
        }       
        n=write(socket,buffer,256);    
    }
}

void f_read(int socket){
    for (;;){    
        bzero(buffer,256);
        do{
            n=read(socket,buffer,256);       
        }while(n==0);
    
        if(strcmp("chau",buffer)==0){
            shutdown(socket, SHUT_RDWR);
            close(socket);  
            return;         
        }
        cout<<"Envió servidor: "<<buffer<<endl;                 
    }
}

int main(int argc, char const *argv[]){
    struct sockaddr_in stSockAddr;
    int Res,n;

    int SocketFD=socket(PF_INET,SOCK_STREAM,0);//socketFD-> socketSErver
    if(-1==SocketFD){
        perror("No se puede crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr,0, sizeof(struct sockaddr_in));//asigna valores en una direccion de memoria
    stSockAddr.sin_family= AF_INET;
    stSockAddr.sin_port= htons(45500);
    Res= inet_pton(AF_INET,"192.168.164.26",&stSockAddr.sin_addr); //ip de servidor, lo pone en sin_addr
    connect(SocketFD,(const struct sockaddr *)&stSockAddr,sizeof(struct sockaddr_in));

    read(SocketFD,buffer,0);
    cout<<buffer<<endl;
      
    thread t1(f_write,SocketFD);
    thread t2(f_read,SocketFD);

    t2.join();
    t1.join();

    shutdown(SocketFD,SHUT_RDWR);
    close(SocketFD);
}
