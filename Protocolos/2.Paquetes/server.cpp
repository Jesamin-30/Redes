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
        int cont=0;
        cout<<"----ENVIADO POR PAQUETES---"<<endl;
        do{
            
            bzero(buffer,sizeof(buffer));
            n=read(ConnectFD,buffer,sizeof(buffer));
            if(cont!=9){
                cout<<"Cliente envió siguiente informacion: "<<buffer<<endl;
            }
            if(cont==0){
                strcpy(client.m_nombre,buffer);
            }
            if(cont==1){
                strcpy(client.m_initial,buffer);
            }
            if(cont==2){
                strcpy(client.m_surname,buffer);
            }
            if(cont==3){
                strcpy(client.m_dob,buffer);
            }
            if(cont==4){
                strcpy(client.m_dni,buffer);
            }
            if(cont==5){
                strcpy(client.m_address,buffer);
            }
            if(cont==6){
                strcpy(client.m_profession,buffer);
            }
            if(cont==7){
                strcpy(client.m_bio,buffer);
            }
            if(cont==8){
                strcpy(client.m_photoName,buffer);
            }
            if(cont==9){
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
            if(cont==10){
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
            cont++;
            
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
