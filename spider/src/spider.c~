#include<spider.h>
//加载配置文件
char* Load_Config(char* path,char* key)
{
    FILE* pf=NULL;
    char* value=NULL;
    size_t len=0;
    char* p=NULL;
    pf=fopen(path,"r");
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
    fclose(pf);
    return NULL;
}
void Init_Url_t(url_t* url,int port)
{
    bzero(url,sizeof(url_t));
    url->port=port;
}

int main()
{
    printf("%s",Load_Config("../conf/spider.ini","URL"));
    url_t *pUrl=(url_t*)malloc(sizeof(url_t));
    Init_Url_t(pUrl,80);
    free(pUrl);
}
