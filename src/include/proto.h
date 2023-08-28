#ifndef PROTO__H__
#define PROTO__H__

#define DEFAULT_MGROUP  "224.2.2.2"                         // 多播组
#define DEFAULT_RCVPORT "1989"                              // 默认端口
#define CHN_NUM         200                                 // 最大频道数量

#define LIST_CHN_ID     0                                   // 约定 0 号为节目单频道
#define MIN_CHN_ID      1                                   // 最小广播频道号
#define MAX_CHN_ID      MIN_CHAN_ID+CHN_NUM-1               // 最大广播频道号

#define MSG_CHN_MAX     (512-20-8)                          // 推荐长度 -ip包的长度 - udp包的报头
#define MAX_DATA        MSG_CHN_MAX-sizeof(chnid_t)

#define MSG_LIST_MAX    (65536 -20 -8)                      //推荐长度 -ip包的长度 - udp包的报头
#define MAX_ENTRY       (MSG_LIST_MAX  - sizeof(chnid_t))


#include "type.h"

/* 一个频道包的结构体 */
/* 频道包，第一字节描述频道号，data[0]在结构体最后作用为变长数组，根据malloc到的实际内存大小决定 */
struct msg_channels_st
{
    chnid_t chnid; /* must between [MIN_CHN_ID,MAX_CHN_ID]*/
    uint8_t data[1];
}__attribute__((packed));

/*  节目包结构体 */
/* 单个节目信息，第一字节描述频道号，第二三字节为本条信息的总字节数，desc[0]为变长数组 */
struct  msg_listentry_st
{
    chnid_t chnid;  /* channel num  */
    uint16_t len;
    uint8_t desc[1]; /*  通道描述信息*/

}__attribute__((packed));

/*  总包结构体 包括节目单和节目描述*/
/* 节目单数据包，第一字节描述频道号，list[0]为变长数组，存储msg_listdesc_t变长内容 */
struct msg_list_st
{
   chnid_t chnid; /*must be LIST_CHN_ID*/
   struct msg_listentry_st entry[1];
}__attribute__((packed));

#endif // PROTO__H__
