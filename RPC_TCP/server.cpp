#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<cstring>
#include<poll.h>
#include<vector>
#include<queue>
#include<sstream>

pthread_mutex_t queue_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond=PTHREAD_COND_INITIALIZER;
std::queue<int> client_queue;

std::string process_request(const std::string& req){
  std::istringstream iss(req);
  std::string op;
  double a,b,res;
  if(!(iss>>op>>a>>b))return"ERROR: Invalid command format";
  if(op=="ADD")res=a+b;
  else if(op=="SUB")res=a-b;
  else if(op=="MUL")res=a*b;
  else if(op=="DIV"){if(b==0)return"ERROR: Division by zero";res=a/b;}
  else return"ERROR: Unknown operation";
  return std::to_string(res);
}

void* handle_client(void* arg){
  int cli_sock=*(int*)arg;
  delete(int*)arg;
  while(true){
    char buf[1024]={0};
    int rcv=recv(cli_sock,buf,1023,0);
    if(rcv<=0){
      if(rcv==0)std::cout<<"Client disconnected\n";
      else perror("recv error");
      break;
    }
    buf[rcv]='\0';
    std::string resp=process_request(buf);
    send(cli_sock,resp.c_str(),resp.length(),0);
  }
  close(cli_sock);
  return nullptr;
}

int main(){
  int serv_sock=socket(AF_INET,SOCK_STREAM,0);
  if(serv_sock==-1){
    perror("socket error");
    exit(errno);
  }
  struct sockaddr_in serv_addr;
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(8888);
  
  if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
    perror("bind failed");
    exit(errno);
  }
  if(listen(serv_sock,10)<0){
    perror("listen failed");
    exit(errno);
  }
  std::cout<<"Waiting for connection\n";
  
  struct pollfd fds[10];
  fds[0].fd=serv_sock;
  fds[0].events=POLLIN;
  int cli_count=1;
  
  while(true){
    int p_cnt=poll(fds,cli_count,-1);
    if(p_cnt<0){
      perror("poll error");
      continue;
    }
    if(fds[0].revents&POLLIN){
      struct sockaddr_in cli_addr;
      socklen_t cli_len=sizeof(cli_addr);
      int cli_sock=accept(serv_sock,(struct sockaddr*)&cli_addr,&cli_len);
      if(cli_sock>=0){
        std::cout<<"New client connected\n";
        fds[cli_count].fd=cli_sock;
        fds[cli_count].events=POLLIN;
        cli_count++;
        pthread_t thr;
        int* cli_sock_ptr=new int(cli_sock);
        pthread_create(&thr,nullptr,handle_client,cli_sock_ptr);
        pthread_detach(thr);
      }
    }
    for(int i=1;i<cli_count;i++){
      if(fds[i].revents&POLLIN){
        char buf[1024]={0};
        int cli_sock=fds[i].fd;
        int rcv=recv(cli_sock,buf,1023,0);
        if(rcv>0){
          buf[rcv]='\0';
          std::string resp=process_request(buf);
          send(cli_sock,resp.c_str(),resp.length(),0);
        }else{
          if(rcv==0)std::cout<<"Client disconnected\n";
          else perror("recv error");
          close(cli_sock);
          fds[i]=fds[--cli_count];
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}
