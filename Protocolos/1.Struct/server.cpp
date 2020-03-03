#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "base64.h"

using namespace std;
struct CPhoto{
    char m_name[1000];
    char m_file[1000000];
};

struct CClient{
    char m_nombre[1000];
    char m_initial[1000];
    char m_surname[1000];
    char m_dob[1000];
    char m_dni[9];
    char m_address[1000];
    char m_profession[1000];
    char m_bio[10000];
    CPhoto m_photo;
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
    struct CPhoto photo;

    for (;;){
        int ConnectFD=accept(SocketFD,NULL,NULL);

        do{
            bzero(buffer,sizeof(buffer));
            n=read(ConnectFD,buffer,sizeof(buffer));
            cout<<"Cliente envió siguiente informacion: "<<endl;
            char tmp[sizeof(CClient)];
            int cont=0;
            int ope=0;
            for (int i = 0; i < strlen(buffer); i++){
                if(buffer[i]!='|'){
                    tmp[cont]=buffer[i];
                    cont++;
                }
                else{
                    if(ope==0){
                        strcpy(client.m_nombre,tmp);
                    }
                    if(ope==1){
                        strcpy(client.m_initial,tmp);
                    }
                    if(ope==2){
                        strcpy(client.m_surname,tmp);
                    }
                    if(ope==3){
                        strcpy(client.m_dob,tmp);
                    }
                    if(ope==4){
                        strcpy(client.m_dni,tmp);
                    }
                    if(ope==5){
                        strcpy(client.m_address,tmp);
                    }
                    if(ope==6){
                        strcpy(client.m_profession,tmp);
                    }
                    if(ope==7){
                        strcpy(client.m_bio,tmp);
                    }
                    if(ope==8){
                        strcpy(photo.m_name,tmp);
                    }
                    if(ope==9){
                        ofstream file ("happy_server.jpg");
                        if (file.is_open())
                        {
                            file << base64_decode(tmp);
                            strcpy(photo.m_file,tmp);
                            file.close();
                        }
                    }
                    ope++;
                    cont=0;
                    bzero(tmp,sizeof(tmp));
                }
            }
            cout<<client.m_nombre<<endl; 
            cout<<client.m_initial<<endl;
            cout<<client.m_surname<<endl;
            cout<<client.m_dob<<endl;
            cout<<client.m_dni<<endl;
            cout<<client.m_address<<endl;
            cout<<client.m_profession<<endl;
            cout<<client.m_bio<<endl;
            cout<<photo.m_name<<endl;            
            
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
