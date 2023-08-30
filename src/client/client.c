#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "utils/log.h"
#include "include/proto.h"

#include "client.h"

#define __USE_MISC 1
#include <features.h>
/*
    -M  --mgroup    指定多播组
    -P  --port      指定接收端口
    -p  --player    指定播放器
    -H  --help      显示帮助
*/
struct client_conf_st client_conf = {
    .recvport = DEFAULT_RCVPORT,
    .mgroup = DEFAULT_MGROUP,
    .player_cmd = DEFAULT_PLAYCMD};

static void print_help()
{
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
    int index = 0;
    int c;
    int sd;
    int val;

    struct ip_mreqn mreq;
    struct in6_addr i6;
    struct sockaddr_in laddr;

    struct option argarr[] = {{"port", 1, NULL, 'P'},
                              {"mgroup", 1, NULL, 'M'},
                              {"player", 1, NULL, 'p'},
                              {"help", 1, NULL, 'h'},
                              {0, 0, 0, 0}};
    while (1)
    {
        c = getopt_long(argc, argv, "P:M:p:H:", argarr, &index);
        if (c < 0)
        {
            break;
        }
        switch (c)
        {
        case 'P':
            client_conf.recvport = optarg;
            break;
        case 'M':
            client_conf.mgroup = optarg;
            break;
        case 'p':
            client_conf.player_cmd = optarg;
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
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sd < 0)
    {
        log_error("socket");
        exit(0);
    }

    inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("eth0");
    if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        log_error("setsockopt-IP_ADD_MEMBERSHIP");
        exit(0);
    }

    val = 1;
    if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0)
    {
        log_error("setsockopt-IP_MULTICAST_LOOP");
        exit(0);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.recvport));
    inet_pton(AF_INET, "0,0,0,0", &laddr.sin_addr.s_addr);
    if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0)
    {
        log_error("bind");
        exit(0);
    }
#if 0
    pipe(); /*匿名管道*/

    fork();

    /*
        子进程  调用解码器
        父进程  从网络上收包,发送给子进程
    */
#endif
    exit(0);
}
