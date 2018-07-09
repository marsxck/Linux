#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#define URLSIZE 1024
#define DOMAIN 128
#define IP     16
#define NAME   1024
typedef struct{
    char url[URLSIZE];//配置文件获取
    char domain[DOMAIN];//域名
    char ip[IP];//主机ip
    char name[NAME];//资源名
    int port;
}url_t;
void Init_Url_t(url_t*,int port);//初始化结构体
int Url_analytic(url_t*);//解析域名
char* Get_ip(char*);
int Create_http_head(url_t*,char*);
int ConnectServer(url_t*);
int Get_reHead(char*);
int Get_reBody(char*,url_t*);//读取响应体
char* Load_Config(char*,char*);//读配置文件
