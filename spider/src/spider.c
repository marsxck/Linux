#include<spider.h>
//加载配置文件
char* Load_Config(char* path,char* key)
{
    if(path==NULL||key==NULL)
        return "error";
    printf("加载配置中...\n");
    FILE* pf=NULL;
    char* value=NULL;
    size_t len=0;
    char* p=NULL;
    pf=fopen(path,"r+");
    if(pf==NULL)
    {
        printf("%s\n",strerror(errno));
    }
    while(getline(&value,&len,pf)!=-1)//读一行
    {
        if(value[0]=='#'||value[0]==';'||value[0]=='[')
            continue;
        p=strstr(value,key);
        if(p==NULL)
            continue;
        if(p[strlen(key)]!='=')
            continue;
        else
        {
            strtok(p,"=");
            value=strtok(NULL,"=");
            return value;
        }
    }
    if(pf!=NULL)
        fclose(pf);
    return NULL;
}
//初始化结构体
void Init_Url_t(url_t* url)
{
    bzero(url,sizeof(url_t));
    url->port=80;
}
//url分析器
int Url_analytic(url_t* u)
{
    if(u==NULL)
        return 1;
    printf("URL解析中.......\n");
    int i;
    int j=0;
    int nStart=0;
    char* p;
    //解析域名
    char* htt[]={"http://","https://"};
    for(i=0;i<2;i++)
    {
        if(strncmp(u->url,htt[i],strlen(htt[i]))==0)
        {
            nStart=strlen(htt[i]);
            break;
        }
    }
    if(nStart==0) return 2;
    for(i=nStart;u->url[i]!='/'&&u->url[i];i++,j++)
    {
        u->domain[j]=u->url[i];
    }
    //解析路径
    j=0;
    for(i=nStart;u->url[i]!='\0';i++,j++)
    {
        u->path[j]=u->url[i];
    }
    //是否特殊端口
    p=strstr(u->domain,":");
    if(p)
    {
        sscanf(p,":%d",&u->port);
        p[0]='\0';
    }
    //获取ip
    if(Get_ip(u)!=0)
    {
        printf("ip get error");
    }
    return 0;
}
//获取域名对应ip
int Get_ip(url_t* u)
{
    struct hostent* ent;
    ent=gethostbyname(u->domain);
    if(ent)
    {
        inet_ntop(AF_INET,ent->h_addr_list[0],u->ip,16);
        return 0;
    }
    return -1;
}
//连接webserver
int Connect(url_t* u)
{
    if(u==NULL) return -1;
    printf("连接%s中.....\n",u->domain);
    int socketfd;
    int res;
    socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(socketfd==-1)
        printf("%s\n",strerror(errno));
    struct sockaddr_in addrserver;
    bzero(&addrserver,sizeof(struct sockaddr_in));
    addrserver.sin_family=AF_INET;
    inet_pton(AF_INET,u->ip,&addrserver.sin_addr.s_addr);
    addrserver.sin_port=htons(u->port);
    res=connect(socketfd,(struct sockaddr*)&addrserver,sizeof(addrserver));
    if(res==-1)
    {
        printf("连接失败\n");
        printf("%s\n",strerror(errno));
        return -1;
    }
    return socketfd;
}
//创建http请求头
char* Create_http_head(url_t* u)
{
    char head[1024];
    char* temp;
    sprintf(head,"GET %s HTTP/1.1\r\n"\
            "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
            "User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2525Sagari/537.36\r\n"\
            "Host:%s\r\n"\
            "Connection:close\r\n"\
            "\r\n",u->url,u->domain);
    //printf("%s",head);
    temp=(char*)malloc(sizeof(char)*strlen(head)+1);
    strcpy(temp,head);
    return temp;
}
//获取响应头
char* Get_reHead(int sockfd,char* head)
{
    char buf[1024];
    char rebuf[1024];
    char recvbuf[1024];
    char* temp;
    int res;
    int j=0;
    int fd;
    int len;
    bzero(buf,sizeof(buf));
    bzero(rebuf,sizeof(rebuf));
    write(sockfd,head,strlen(head));
    int flag=0;
    while(read(sockfd,buf,1)!=-1)
    {
        rebuf[j++]=buf[0];
        if(buf[0]=='\r'||buf[0]=='\n')
            flag++;
        else
            flag=0;
        if(flag==4)
            break;
    }
    temp=(char*)malloc(strlen(rebuf)+1);
    free(head);
    strncpy(temp,rebuf,strlen(rebuf));
    fd=open("./res/sss.html",O_WRONLY|O_CREAT,0755);
    printf("%s",temp);
    while((len=read(sockfd,recvbuf,sizeof(recvbuf)))!=-1)
    {
        printf("%d\n",len);
        if(len==0)
            break;
        write(fd,recvbuf,len);
    }
    close(fd);
    close(sockfd);
    printf("download finish\n");
    return temp;
}
int main()
{
    int sockfd;
    char* head;
    char* rehead;
    char* value=Load_Config("./conf/spider.ini","URL");
    url_t *pUrl=(url_t*)malloc(sizeof(url_t));
    Init_Url_t(pUrl);
    strncpy(pUrl->url,value,strlen(value)-1);
    if(Url_analytic(pUrl))
        printf("URL analy error");
    printf("%s\n%s\n%s\n%d\n%s\n",pUrl->url,pUrl->domain,pUrl->path,pUrl->port,pUrl->ip);
    sockfd=Connect(pUrl);
    head=Create_http_head(pUrl);
    printf("%s",head);
    rehead=Get_reHead(sockfd,head);
    printf("%s",rehead);
    free(pUrl);
    return 0;
}
