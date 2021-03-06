#include<sys/epoll.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#define EPOLLSIZE 10000
int main()
{
    //创建sockfd
    int listenfd=socket(AF_INET,SOCK_STREAM,0);
    int clientfd,len,epollfd,nready,i;
    //初始化结构体
    struct sockaddr_in socklisten,clientaddr; 
    socklisten.sin_family=AF_INET;
    socklisten.sin_port=htons(7788);
    socklisten.sin_addr.s_addr=inet_addr("172.21.16.2");
    //绑定
    bind(listenfd,(struct sockaddr*)&socklisten,sizeof(struct sockaddr_in));
    //监听
    listen(listenfd,80);
    int addrlen=sizeof(clientaddr);
    char buf[1024];
    //epoll
    epollfd=epoll_create(EPOLLSIZE);
    struct epoll_event ev,evarr[EPOLLSIZE];
    ev.events=EPOLLIN;
    ev.data.fd=listenfd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);
    fcntl(0,F_SETFL,O_NONBLOCK);

    while(1)
    {
        i=0;
        nready=epoll_wait(epollfd,evarr,EPOLLSIZE,-1);
        for(i=0;i<nready;i++)
        {
            if(evarr[i].data.fd==listenfd)
            {
                clientfd=accept(listenfd,NULL,NULL);
                fcntl(clientfd,F_SETFL,O_NONBLOCK);
                ev.data.fd=clientfd;
                epoll_ctl(epollfd,EPOLL_CTL_ADD,clientfd,&ev);
            }
            else
            {
                bzero(buf,1024);
                len=read(evarr[i].data.fd,buf,1024);
                if(len==0)
                {
                    epoll_ctl(epollfd,EPOLL_CTL_DEL,evarr[i].data.fd,NULL);
                    close(evarr[i].data.fd);
                }
                printf("%s\n",buf);
                bzero(buf,1024);
                scanf("%s",buf);
                if(strlen(buf)>0)
                    write(evarr[i].data.fd,buf,strlen(buf));
            }
        }
    }


    return 0;
}
