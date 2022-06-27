#include"socket.h"
#define IPS "ipstr="
#define PORT "port="
#define LISMAX "lismax="
struct sock_info
{
    /* data */
    char ips[32];
    short port;
    short lismax;
};

int read_conf(struct sock_info *pa)//读取配置文件--ip,port,lismax
{
    //如果失败 返回-1
    if(pa==NULL){
        return -1;
    }
    FILE *fp=fopen("my.conf","r");
    if(fp==NULL){
        printf("my.conf not exists \n");
        return -1;
    }
    char buff[128]={0};
    //记录当前的行号
    int index=0;
    while(fgets(buff,128,fp)!=NULL){
        index++;
        //如果第一个字符为空格或者\n直接跳过
        if(strncmp(buff,"#",1)==0||strncmp(buff,"\n",1)==0){
            continue;
        }
        buff[strlen(buff)-1]='\0';
        if(strncmp(buff,IPS,strlen(IPS))==0){
            strcpy(pa->ips,buff+strlen(IPS));

        }else if(strncmp(buff,PORT,strlen(PORT))==0){
            pa->port=atoi(buff+strlen(PORT));

        }else if(strncmp(buff,LISMAX,strlen(LISMAX))==0){
            pa->lismax=atoi(buff+strlen(LISMAX));
        }else {
            printf("Unrecognized configuration items%d行：%s\n",index,buff);
        }
        //printf("%s",buff);
    }
    fclose(fp);
}
int socket_init(){
    struct sock_info a;
    if(read_conf(&a)==-1){
        printf("read conf err\n");
        return -1;
    }
    printf("ip:%s\n",a.ips);
    printf("port:%d\n",a.port);
    printf("lismax:%d\n",a.lismax);
    //创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1){
        return sockfd;
    }
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(a.port);
    saddr.sin_addr.s_addr=inet_addr(a.ips);
    int res=bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
    if(res==-1){
        return -1;
    }
    res=listen(sockfd,a.lismax);
    if(res==-1){
        return -1;
    }
    return sockfd;
}