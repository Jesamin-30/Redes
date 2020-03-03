#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "base64.h"

using namespace std;

struct CClient{
    char m_nombre[1000];
    char m_initial[1000];
    char m_surname[1000];
    char m_dob[1000];
    char m_dni[9];
    char m_address[1000];
    char m_profession[1000];
    char m_bio[10000];
    char m_photoName[1000];
    char m_fphoto[10000];
};

char buffer[sizeof(CClient)];
int n;

int main(int argc, char const *argv[]){
    struct sockaddr_in stSockAddr;
    int SocketFD= socket(PF_INET,SOCK_STREAM,0);
    char mensaje[256];

    if(-1==SocketFD){
        perror("No se puede crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr,0,sizeof(struct sockaddr_in));

    stSockAddr.sin_family=AF_INET;
    stSockAddr.sin_port=htons(45501);
    stSockAddr.sin_addr.s_addr=INADDR_ANY;

    if(-1 == bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
        perror("Cambiar puerto!!!");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }

    if(-1 == listen(SocketFD, 10))
    {
        perror("cambiar puerto!!!");
        close(SocketFD);
        exit(EXIT_FAILURE);
    }
    struct CClient client;

    for (;;){
        int ConnectFD=accept(SocketFD,NULL,NULL);
        cout<<"----ENVIADO POR COMANDOS---"<<endl;
        do{            
            n=read(ConnectFD,buffer,sizeof(buffer));
            cout<<"Cliente envió siguiente informacion: "<<buffer<<endl;
            if(buffer[0]=='n'){
                strcpy(client.m_nombre,buffer+5);
            }
            if(buffer[0]=='i'){
                strcpy(client.m_initial,buffer+8);
            }
            if(buffer[0]=='s'){
                strcpy(client.m_surname,buffer+8);
            }
            if(buffer[0]=='d'){
                if (buffer[1]=='o'){
                    strcpy(client.m_dob,buffer+4);
                }                
                if (buffer[1]=='n'){
                    strcpy(client.m_dni,buffer+4);
                }
            }
            if(buffer[0]=='a'){
                strcpy(client.m_address,buffer+8);
            }
            if(buffer[0]=='p'){
                if(buffer[1]=='r'){
                    strcpy(client.m_profession,buffer+11);
                }
                if(buffer[1]=='h'){
                    strcpy(client.m_photoName,buffer+6);
                    ofstream file ("happy_server.jpg");
                    string decoded;
                    if (file.is_open())
                    {
                        decoded=base64_decode(buffer);
                        file << decoded;
                        file.close();
                    }
                    strcpy(client.m_fphoto,decoded.c_str());
                }
            }
            if(buffer[0]=='b'){
                strcpy(client.m_bio,buffer+4);
            }            
            if(buffer[0]==' '){
                cout<<"---------INFORMACION ENVIADA COMPLETA:---------"<<endl;
                cout<<client.m_nombre<<endl; 
                cout<<client.m_initial<<endl;
                cout<<client.m_surname<<endl;
                cout<<client.m_dob<<endl;
                cout<<client.m_dni<<endl;
                cout<<client.m_address<<endl;
                cout<<client.m_profession<<endl;
                cout<<client.m_bio<<endl;                
                cout<<client.m_photoName<<endl;             
            }

            bzero(buffer,sizeof(buffer));
            
            if(strcmp("chau",buffer)==0){
                shutdown(ConnectFD, SHUT_RDWR);//LIBERA RECURSOS
                close(ConnectFD); 
                close(SocketFD);
                return 0;       
            }

            //cout<<"Para cliente: ";
            strcpy(buffer," ");
            //cin.getline(buffer, 256);
            n=write(ConnectFD,buffer,256);            
        } while (buffer!="chau");
    }
}
