#include<iostream>
#include<unistd.h>
#include<cstring>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string>

int main(){
  struct sockaddr_in srv_addr;
  int sock=socket(AF_INET,SOCK_STREAM,0);
  if(sock==-1){
    perror("socket error");
    exit(errno);
  }
  srv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  srv_addr.sin_family=AF_INET;
  srv_addr.sin_port=htons(8888);
  
  if(connect(sock,(struct sockaddr*)&srv_addr,sizeof(srv_addr))==-1){
    perror("connection failed");
    exit(errno);
  }
  
  while(true){
    std::string msg;
    std::getline(std::cin,msg);
    if(msg=="exit")break;
    if(send(sock,msg.c_str(),msg.size(),0)==-1){
      exit(errno);
    }
    char buf[1024]={0};
    int rcv=recv(sock,buf,1023,0);
    if(rcv>0){
      buf[rcv]='\0';
      std::cout<<"result "<<buf<<std::endl;
    }else if(rcv==0){
      std::cerr<<"connection lost"<<std::endl;
    }else{
      perror("error");
      break;
    }
  }
  close(sock);
  return 0;
}
