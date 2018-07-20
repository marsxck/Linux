#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctype.h>
#define	CLIENT_MAX	1024
#define	PORT		7788


int main(void)
{
		struct sockaddr_in serveraddr;
		fd_set newset,oldset;
		int maxfd,i,sockfd,clientfd,nready,j;
		int clientarr[CLIENT_MAX];
		sockfd = socket(AF_INET,SOCK_STREAM,0);
		bzero(&serveraddr,sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr("172.21.16.2");
		serveraddr.sin_port = htons(PORT);
		bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
		listen(sockfd,128);
		FD_ZERO(&oldset);
		FD_SET(sockfd,&oldset);
		maxfd = sockfd;
		for(i = 0;i<CLIENT_MAX;i++){
				clientarr[i]=-1;
		}
		while(1)
		{
				newset = oldset;
				nready = select(maxfd+1,&newset,NULL,NULL,NULL);
                i=0;
                sleep(1);
				while(nready){
                         
                        printf("一共有nread:%d\tnewset:%x\n",nready,newset);
                        fd_set temp=newset;
						if(FD_ISSET(sockfd,&newset)&&i==0){
                                //printf("accept...\n");
                                i++;
								clientfd = accept(sockfd,NULL,NULL);
                                //printf("server%d处理完 newset:%d\n",sockfd,newset); 
								for(j=1;j<CLIENT_MAX;j++)
								{
										if(clientarr[j]==-1){
												clientarr[j]=clientfd;
												break;
										}
								}
                                //printf("server%d处理完 newset:%d\n",sockfd,newset);
								FD_SET(clientfd,&oldset);
                                //printf("server%d处理完 newset:%d\n",sockfd,newset);
								if(clientfd > maxfd)
										maxfd = clientfd;
                                printf("server%d处理完 newset:%x\n",sockfd,newset);
						}else{
								int len,j;
								char buf[1500];
								for(i;i<CLIENT_MAX;i++){
										if(clientarr[i]!=-1)
                                                printf("一共有nread:%d\tnewset:%d\n",nready,newset);
												if(FD_ISSET(clientarr[i],&newset)){
                                                        printf("clienti%d处理完 newset:%d\n",i,nready,newset);
														len = read(clientarr[i],buf,sizeof(buf));
                                                        printf("获取了%d\n",len);
														if(len == 0){
																printf("clienti:%d closeing..\n",clientarr[i]);
																FD_CLR(clientarr[i],&oldset);
																close(clientarr[i]);
																clientarr[i]=-1;
                                                                i++;
																break;
														}
														j=0;
														while(len > j){
																buf[j] = toupper(buf[j]);
																j++;
														}
														write(clientarr[i],buf,len);
														bzero(buf,sizeof(buf));
                                                        i++;
														break;
												}
                                                        //printf("clienti%d处理完 newset:%d\n",clientarr[i],nready,newset);
								}
						}
						nready--;
				}
		}
		close(sockfd);
		return 0;
}
