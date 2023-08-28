#ifndef CLIENT_H__
#define CLIENT_H__

#define DEFAULT_PLAYCMD "/usr/bin/mpg123 > /dev/null"

struct client_conf_st
{
    char *recvport;     // 接收端口
    char * mgroup;      // 多播组
    char* player_cmd;   // 播放器
};

extern struct client_conf_st client_conf; 

#endif