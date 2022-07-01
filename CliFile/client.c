#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#define OK "OK#"
char *get_cmd(char buff[],char *myargv[]){
    if(buff==NULL||myargv==NULL){
        return NULL;
    }
    char *s=strtok(buff," ");
    int i=0;
    while(s!=NULL){
        myargv[i++]=s;
        s=strtok(NULL," ");
    }
    return myargv[0];
}
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
    saddr.sin_addr.s_addr=inet_addr("124.223.7.124");
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
        buff[strlen(buff)-1]=0;
        char cmd_buff[128]={0};
        strcpy(cmd_buff,buff);
        //解析命令参数，异常检测
        char *myargv[10]={0};
        char *cmd=get_cmd(buff,myargv);
        if(cmd==NULL){
            continue;
        }else if(strcmp(cmd,"exit")==0){//客户端退出
            break;
        }else if(strcmp(cmd,"download")==0){
             //download -下载
        }else if(strcmp(cmd,"upload")==0){
            //upload--上传
        }else {
            //执行命名
            send(sockfd,cmd_buff,strlen(cmd_buff),0);
            char read_buff[1024]={0};
            //读取服务器返回的结果
            int num=recv(sockfd,read_buff,1023,0);
            if(num<=0){
                break;
            }
            if(strncmp(read_buff,OK,strlen(OK))==0){
                printf("%s\n",read_buff+strlen(OK));
            }else{
                printf("%s",read_buff);
            }
        }
        /*测试链接
        send(sockfd,buff,strlen(buff),0);
        memset(buff,0,128);
        recv(sockfd,buff,127,0);
        printf("buff=%s\n",buff);
        */
    }
    return 0;
}
