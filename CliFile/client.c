#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        printf("create socket err");
        exit(0);
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
    int res=connect(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(res==-1){
        close(sockfd);
        exit(0);
    }
    while(1){
        printf("connect>>");
        fflush(stdout);
        char buff[128]={0};
        fgets(buff,128,stdin);
        if(strncmp(buff,"end",3)==0){
            break;
        }
        //解析命令参数，异常检测

        send(sockfd,buff,strlen(buff),0);
        memset(buff,0,128);
        recv(sockfd,buff,127,0);
        printf("buff=%s\n",buff);
    }
    return 0;
}
