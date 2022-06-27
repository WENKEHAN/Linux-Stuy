#include"thread.h"
#define ARG_MAX 10
//解析收到的字符串以及对应的参数
char *get_cmd(char buff[],char *argv[]){
    char *p=NULL;
    if(buff==NULL||argv==NULL){
        printf("客户端没有传入字符串\n");
        exit(1);
    }
    char *p=NULL;//记录当前字符串分割的位置
    char *s=NULL;//记录每次返回的字符串
    s=strtok_r(buff,' ',&p);
    while(p!=NULL){
        strtok(buff,'\0');
    }
}
void *work_thread(void *arg){
    int c=(int )arg;
    while(1){
        char buff[128]={0};
        int n=recv(c,buff,127,0);
        //对方已经关闭
        if(n<=0){
            break;
        }
        //解析命名以及参数
        char *myargv[ARG_MAX]={0};
        //传入命令--以及对应的参数
        char *cmd=get_cmd(buff,myargv);
        if(cmd==NULL){
            
        }
        printf("buff=%s\n",buff);//测试
        send(c,"ok",2,0);
    }
    close(c);
    printf("client close\n");
}
void start_thread(int c){
    pthread_t id;
    pthread_create(&id,NULL,work_thread,(void *)c);
}