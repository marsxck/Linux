#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netdb.h>
#include<errno.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define URLSIZE 1024
#define DOMAIN 128
#define IP     16
#define NAME   1024
typedef struct{
    char url[URLSIZE];//配置文件获取
    char domain[DOMAIN];//域名
    char ip[IP];//主机ip
    char path[NAME];//资源名
    int port;
}url_t;
void Init_Url_t(url_t*);//初始化结构体
int Url_analytic(url_t*);//解析URL
int Get_ip(url_t*);//获取域名ip
int Create_http_head(url_t*,char*);
int ConnectServer(url_t*);//连接webserver
int Get_reHead(char*);
int Get_reBody(char*,url_t*);//读取响应体
char* Load_Config(char*,char*);//读配置文件
