#include<stdio.h>

typedef struct{
    char url[];//配置文件获取
    char domain[];//域名
    char ip[];//主机ip
    char name[];//资源名
    int port;
}url_t;//解析结构体
int Url_analytic(url_t*,char*);//解析域名
char* Get_ip(char*);
int Create_http_head(url_t*,char*);
int ConnectServer(url_t*);
int Get_reHead(char*);
int Get_reBody(char*,url_t*);//读取响应体
