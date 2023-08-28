# IPv4_streaming_media

一个简单的流媒体项目，基于客户端/服务器模型（C/S）开发，采用UDP组播技术，实现MP3格式音乐广播系统。服务器端采用多线程处理频道信息，实现媒体库（MP3）读取并进行流量控制（令牌桶），并通过UDP组播发送；客户端采用多进程，实现父进程接受来自网络的数据，通过进程间通信技术管道发送给子进程，子进程进行数据解码并输出到特定频道。

## 系统框架

![系统框架](https://user-images.githubusercontent.com/46079639/174961551-a048afc3-6574-4b03-a8e9-a177e24031ed.jpg)

## 设置本地媒体库

``` text
|---media
| --- | --- ch1
| --- | --- | --- desc.txt
| --- | --- | --- 1.mp3
| --- | --- | --- 2.mp3
| --- | --- ch2
| --- | --- | --- desc.txt
| --- | --- | --- 3.mp3
| --- | --- | --- 4.mp3
| --- | --- ch3
| --- | --- | --- desc.txt
```

> 注：频道目录chn(ch1, ch2, ch3...)位于 /var/media 目录下(如果不存在需要自己创建),每一个频道中包含需要播放的媒体文件(mp3),以及该频道音乐的类型(中文\英文\流行\古典),将该标签写入 desc.txt ,将来会在节目单频道出现供用户选择。

## 参考

- [IPV4流媒体C语言项目](https://blog.csdn.net/qq_43745917/article/details/128048044)
- [基于IPv4的流媒体广播系统](https://github.com/missFuture/IPv4_streaming_media)
- [基于IPv4的流媒体广播系统Plus](https://github.com/litbubo/Streaming_media_broadcasting_system_based_on_IPv4/tree/main)
