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
    stSockAddr.sin_port=htons(45506);
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

        do{
            bzero(buffer,sizeof(buffer));
            n=read(ConnectFD,buffer,sizeof(buffer));
            cout<<"Cliente envió informacion en JSON: \n"<<buffer<<endl;
            char tmp[10000];
            char var[10000];
            int cont=0;
            int k=0;
            for (int i = 1; i < strlen(buffer)-1; i++){
                if(buffer[i]!=','){
                    tmp[cont]=buffer[i];
                    cont++;
                }
                else{                    
                    if(tmp[1]=='n'){
                        for (int j = 8; j<strlen(tmp)-1; j++){
                            var[k]+=tmp[j];
                            k++;
                        }
                        strcpy(client.m_nombre,var);
                    }
                    if(tmp[1]=='i'){
                        for (int j = 11; j<strlen(tmp)-1; j++){
                            var[k]+=tmp[j];
                            k++;
                        }
                        strcpy(client.m_initial,var);
                    }
                    if(tmp[1]=='s'){
                        for (int j = 11; j<strlen(tmp)-1; j++){
                            var[k]+=tmp[j];
                            k++;
                        }
                        strcpy(client.m_surname,var);
                    }
                    if(tmp[1]=='d'){
                        if (tmp[2]=='o'){
                            for (int j = 7; j<strlen(tmp)-1; j++){
                                var[k]+=tmp[j];
                                k++;
                            }
                            strcpy(client.m_dob,var);
                        }                
                        if (buffer[2]=='n'){
                            for (int j = 7; j<strlen(tmp)-1; j++){
                                var[k]+=tmp[j];
                                k++;
                            }
                            strcpy(client.m_dni,var);
                        }
                    }
                    if(tmp[1]=='a'){
                        for (int j = 11; j<strlen(tmp)-1; j++){
                            var[k]+=tmp[j];
                            k++;
                        }
                        strcpy(client.m_address,var);
                    }
                    if(tmp[1]=='p'){
                        if(tmp[2]=='r'){
                            for (int j = 14; j<strlen(tmp)-1; j++){
                                var[k]+=tmp[j];
                                k++;
                            }
                            strcpy(client.m_profession,var);
                        }
                        if(tmp[2]=='h'){
                            for (int j = 9; j<strlen(tmp)-1; j++){
                                var[k]+=tmp[j];
                                k++;
                            }
                            strcpy(client.m_photoName,var);

                            ofstream file ("happy_server.jpg");
                            string decoded;
                            if (file.is_open())
                            {
                                decoded=base64_decode(var);
                                file << decoded;
                                file.close();
                            }
                            strcpy(client.m_fphoto,decoded.c_str());
                        }
                    }
                    if(tmp[1]=='b'){
                        for (int j = 7; j<strlen(tmp)-1; j++){
                            var[k]+=tmp[j];
                            k++;
                        }
                        strcpy(client.m_bio,var);
                    }
                    k=0;
                    cont=0;
                    bzero(tmp,sizeof(tmp));
                    bzero(var,sizeof(var));
                }
            }

            cout<<"----------------------------INFORMACION COMPLETA------------------------"<<endl;
            cout<<client.m_nombre<<endl; 
            cout<<client.m_initial<<endl;
            cout<<client.m_surname<<endl;
            cout<<client.m_dob<<endl;
            cout<<client.m_dni<<endl;
            cout<<client.m_address<<endl;
            cout<<client.m_profession<<endl;
            cout<<client.m_bio<<endl;
            cout<<client.m_photoName<<endl;
            
            
            
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
