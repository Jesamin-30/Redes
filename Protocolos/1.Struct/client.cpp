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


int main(int argc, char const *argv[]){
    struct sockaddr_in stSockAddr;
    int Res;
    char mensaje[256];
    char buffer[sizeof(CClient)];
    int n;

    int SocketFD=socket(PF_INET,SOCK_STREAM,0);
    if(-1==SocketFD){
        perror("No se puede crear socket");
        exit(EXIT_FAILURE);
    }

    memset(&stSockAddr,0, sizeof(struct sockaddr_in));//asigna valores en una direccion de memoria
    stSockAddr.sin_family= AF_INET;
    stSockAddr.sin_port= htons(45501);
    Res= inet_pton(AF_INET,"192.168.3.109",&stSockAddr.sin_addr); //ip de servidor, lo pone en sin_addr
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in))){
      perror("Cambiar puerto!!!");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    struct CClient client={"Jesamin","J","Zevallos","30-09-97","23415674","avenida argentina 106","estudiante","hakuna matata"};
    struct CPhoto photo={"happy.jpg",""};

    strcpy(buffer,client.m_nombre);
    strcat(buffer,"|");
    strcat(buffer,client.m_initial);
    strcat(buffer,"|");
    strcat(buffer,client.m_surname);
    strcat(buffer,"|");
    strcat(buffer,client.m_dob);
    strcat(buffer,"|");
    strcat(buffer,client.m_dni);
    strcat(buffer,"|");
    strcat(buffer,client.m_address);
    strcat(buffer,"|");
    strcat(buffer,client.m_profession);
    strcat(buffer,"|");
    strcat(buffer,client.m_bio);  
    strcat(buffer,"|");
    strcat(buffer,photo.m_name);  
    strcat(buffer,"|");

    cout<<client.m_nombre<<endl; 
    cout<<client.m_initial<<endl;
    cout<<client.m_surname<<endl;
    cout<<client.m_dob<<endl;
    cout<<client.m_dni<<endl;
    cout<<client.m_address<<endl;
    cout<<client.m_profession<<endl;
    cout<<client.m_bio<<endl;
    cout<<photo.m_name<<endl;
    
    string line;
    ifstream myfile ("happy.jpg");
    string encoded="";
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            encoded+=base64_encode(reinterpret_cast<const unsigned char*>(line.c_str()), line.length());
        }
        myfile.close();
    }
    strcpy(photo.m_file,encoded.c_str());

    strcat(buffer,photo.m_file);  
    strcat(buffer,"|");
    
    char op[2];
    do{
        cout<<"Se enviará la siguiente información, aprete la tecla (s) para enviar(s): ";
        cin>>op;
        if(op[0]=='s'){
            n=write(SocketFD,buffer,sizeof(buffer));
        }
        if(strcmp("chau",buffer)==0){
            shutdown(SocketFD, SHUT_RDWR);
            close(SocketFD);        
            return 0;
        }

        n=read(SocketFD,buffer,256);
        cout<<"Envió servidor: "<<buffer<<endl;
    } while (buffer!="chau");
    
}
