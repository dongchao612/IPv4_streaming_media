#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <unistd.h>  
#include <getopt.h>

#include "utils/log.h"
#include "include/proto.h"

#include "client.h"
/*
    -M  --mgroup    指定多播组
    -P  --port      指定接收端口
    -p  --player    指定播放器
    -H  --help      显示帮助
*/
struct client_conf_st client_conf ={
    .recvport=DEFAULT_RCVPORT,
    .mgroup=DEFAULT_MGROUP,
    .player_cmd=DEFAULT_PLAYCMD
};
static void  print_help(){
    log_info("-P  --port      指定接收端口");
    log_info("-M  --mgroup    指定多播组");
    log_info("-p  --player    指定播放器");
    log_info("-H  --help      显示帮助");
}
int main(int argc, char const **argv)
{
    /*
    初始化级别
        默认值 配置文件 环境变量 命令行参数
    */
    log_info("client begin...");
    int index=0;
    int c;
    struct option argarr[]={{"port",1,NULL,'P'},
                            {"mgroup",1,NULL,'M'},
                            {"player",1,NULL,'p'},
                            {"help",1,NULL,'h'},
                            { 0, 0, 0, 0 }};
    while(1)
    {
        c= getopt_long(argc,argv,"P:M:p:H:",argarr,&index);
        log_trace("c = ",c);
        if(c<0){
            break;
        }
        switch (c)
        {
        case 'P':
            client_conf.recvport=optarg;
            break;
       case 'M':
            client_conf.mgroup=optarg;
            break;
        case 'p':
            client_conf.player_cmd=optarg;
            break;
        case 'h':
            print_help();
            exit(0);
            break;
        default:
            abort();
            break;
        }

    }
#if 0
    socket();

    pipe(); /*匿名管道*/

    fork();

    /*
        子进程  调用解码器
        父进程  从网络上收包,发送给子进程
    */
   #endif
    exit(0);
}
