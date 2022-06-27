#include"socket.h"
#include"thread.h"
int main(){
    int sockfd=socket_init();
    if(sockfd==-1){
        printf("create socket err\n");
        exit(0);
    }
    struct sockaddr_in caddr;
    while (1)
    {
        /* code */
        int len=sizeof(caddr);
        int c=accept(sockfd,(struct sockaddr *)&caddr,&len);
        if(c<0){
            continue;
        }
        printf("accept c=%d\n",c);
        start_thread(c);
    }
    return 0;
    
}