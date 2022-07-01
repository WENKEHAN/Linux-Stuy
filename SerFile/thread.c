#include"thread.h"
#include<sys/wait.h>
#define ARG_MAX 10
#define FILEERR "err"
#define OPENERR  "不存在这个文件"
//下载函数--c 表示文件描述符
// void recv_file(int c,char *filename){
//     if(filename==NULL){
//         send(c,FILEERR,strlen(FILEERR),0);
//         return;
//     }
//     int fd=open(filename,O_RDONLY);
//     if(fd<=0){
//         send(c,FILEERR,strlen(OPENERR),0);
//         return ;
//     }  
// }
//解析收到的字符串以及对应的参数
char *get_cmd(char buff[],char *argv[]){
    //strtok_r()  多线程--线程安全
    if(buff==NULL||argv==NULL){
        printf("客户端没有传入字符串\n");
        return NULL;
    }
    char *p=NULL;//记录当前字符串分割的位置
    char *s=NULL;//记录每次返回的字符串
    int i=0;
    s=strtok_r(buff," ",&p);
    while (s!=NULL)
    {
        /* code */\
        //将分割后的字符串保存到argv中
        argv[i++]=s;
        s=strtok_r(buff," ",&p);
    }
    return argv[0];//命令
}
void *work_thread(void *arg){
    
    //int c=*((int *)arg);
    int c=(int)arg;
    while(1){
        char buff[128]={0};
        int n=recv(c,buff,127,0);//接受客户端发来的数据//ls -l ,mv ,rm
        //对方已经关闭
        if(n<=0){
            printf("没有接受到\n");
            break;
        }
        //解析命名以及--对应的参数
        char *myargv[ARG_MAX]={0};
        //传入命令--以及对应的参数
        char *cmd=get_cmd(buff,myargv);
        if(cmd==NULL){
            send(c,"CMD_ERR",6,0);
        }
        if(strncmp(cmd,"download",3)==0){
            //下载
        }else if(strncmp(cmd,"upload",3)==0){
            //上传
        }else{
            //创建无名管道
            int pipfd[2];
            if(pipe(pipfd)==-1){
                send(c,"PIPE_ERR",8,0);
                continue;
            }
            pid_t pid=fork();
            if(pid==-1){
                send(c,"FORK_ERR",8,0);
            }
            //fork-child 替换标准输出--exec()--在子进程中
            if(pid==0){
                close(pipfd[0]);
                dup2(pipfd[1],1);//管道的写端覆盖标准输出
                dup2(pipfd[1],2);//管道的写端覆盖标准错误输出
                execvp(cmd,myargv);
                perror("exec err");
                close(pipfd[1]);
                exit(0);
            }
            //fork-parent 读管道文件-发送给客户端
            close(pipfd[1]);//关闭管道的写端
            wait(NULL);
            //获取管道中的数据--无论是否有数据都给客户端进行回复--如果管道中没有数据--则发送不出去-所以在这里前置
            char read_buff[1024]={"OK#"};
            read(pipfd[0],read_buff+strlen("OK#"),1021);
            close(pipfd[0]);
            send(c,read_buff,strlen(read_buff),0);
        }
        //测试--是否连通
        /*
        printf("buff=%s\n",buff);
        send(c,"ok",2,0);
        */
    }
    close(c);
    printf("client close\n");
}
void start_thread(int c){
    pthread_t id;
    pthread_create(&id,NULL,work_thread,(void*)&c);
}