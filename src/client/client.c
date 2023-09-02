#include <arpa/inet.h>
#include <getopt.h>
#include <net/if.h>

#define __USE_MISC 1
#include <errno.h>
#include <error.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client.h"
#include "include/proto.h"
#include "utils/log.h"

/*
	-M  --mgroup    指定多播组
	-P  --port      指定接收端口
	-p  --player    指定播放器
	-H  --help      显示帮助
*/
struct client_conf_st client_conf = {.recvport = DEFAULT_RCVPORT, .mgroup = DEFAULT_MGROUP, .player_cmd = DEFAULT_PLAYCMD};

static void print_help()
{
	log_info("-P  --port      指定接收端口");
	log_info("-M  --mgroup    指定多播组");
	log_info("-p  --player    指定播放器");
	log_info("-H  --help      显示帮助");
}
static ssize_t writen(int fd, const char *buf, size_t len)
{
	int pos, ret;
	while (len > 0) {
		ret = write(fd, buf + pos, len);
		if (fd < 0) {
			if (errno == EINTR) {
				continue;
			}
			log_error("write");
			return -1;
		}
		len -= ret;
		pos += ret;
	}

	return len;
}
int main(int argc, char **argv)
{
	/*
	初始化级别
		默认值 配置文件 环境变量 命令行参数
	*/
	// log_info("client begin...");
	int index = 0;
	int c = 0;
	int sd = 0;
	int val = 0;
	int pd[2] = {0, 0};
	pid_t pid = 0;
	int len = 0;
	int chosenid = 0;
	int ret = 0;

	struct ip_mreqn mreq;
	struct in6_addr i6;
	struct sockaddr_in laddr, saddr, raddr;
	socklen_t saddr_len, raddr_len;

	struct option argarr[] = {
		{"port", 1, NULL, 'P'}, {"mgroup", 1, NULL, 'M'}, {"player", 1, NULL, 'p'}, {"help", 1, NULL, 'h'}, {0, 0, 0, 0}};
	while (1) {
		c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);
		log_info("c = %d\n", c);
		if (c < 0) {
			break;
		}
		switch (c) {
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
	if (sd < 0) {
		log_error("socket");
		exit(0);
	}

	inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
	mreq.imr_ifindex = if_nametoindex("eth0");
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		log_error("setsockopt-IP_ADD_MEMBERSHIP");
		exit(0);
	}

	val = 1;
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)) < 0) {
		log_error("setsockopt-IP_MULTICAST_LOOP");
		exit(0);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(client_conf.recvport));
	inet_pton(AF_INET, "0,0,0,0", &laddr.sin_addr.s_addr);
	if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
		log_error("bind");
		exit(0);
	}

	/*匿名管道*/
	if (pipe(pd) < 0) {
		log_error("pipe");
		exit(0);
	}

	pid = fork();
	if (pid < 0) {
		log_error("pipe");
		exit(0);
	}

	if (pid == 0) {
		/*子进程调用解码器*/
		close(sd);
		close(pd[1]);
		dup2(pd[0], 0);
		if (pd[0] > 0) {
			close(pd[0]);
		}
		execl("/bin/bash", "bash", "-c", client_conf.player_cmd, NULL);
		log_error("execl");
	}

	/*父进程 从网络上进行收包 发送给子进程*/
	/*收节目单*/
	struct msg_list_st *msg_list;
	msg_list = malloc(MSG_LIST_MAX);
	if (msg_list == NULL) {
		log_error("malloc");
		exit(0);
	}

	while (1) {
		len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&saddr, &saddr_len);
		if (len < sizeof(struct msg_list_st)) {
			log_warn("message is too small.\n");
			continue;
		}
		if (msg_list->chnid != LIST_CHN_ID) {
			log_warn("channel id is not match\n");
			continue;
		}
		break;
	}

	/*打印节目单，选择频道*/
	struct msg_listentry_st *pos;
	for (pos = msg_list->entry; (char *)pos < ((char *)msg_list + len); pos = (void *)((char *)pos + ntohs(pos->len))) {
		printf("channel [%d] | %s\n", pos->chnid, pos->desc);
	}

	while (1) {
		ret = scanf("%d", &chosenid);
		if (ret != 1) {
			exit(1);
		}
	}
	free(msg_list);

	/*收频道包，发送给子进程*/
	struct msg_channels_st *msg_channel;
	msg_channel = malloc(MSG_CHN_MAX);
	if (msg_channel == NULL) {
		log_error("malloc");
		exit(0);
	}
	while (1) {
		len = recvfrom(sd, msg_channel, MSG_CHN_MAX, 0, (void *)&raddr, &raddr_len);
		if (raddr.sin_addr.s_addr != saddr.sin_addr.s_addr || raddr.sin_port != saddr.sin_port) {
			log_info("addr not match");
			continue;
		}
		if (len < sizeof(struct msg_channels_st)) {
			log_info("message is too small.\n");
			continue;
		}
		if (msg_channel->chnid == chosenid) {
			log_info("accepted msg %d received\n", msg_channel->chnid);
			if (writen(pd[1], msg_channel->data, len - sizeof(chnid_t)) <= 0) {
				exit(1);
			}
		}
	}
	free(msg_channel);
	close(sd);

	exit(0);
}
