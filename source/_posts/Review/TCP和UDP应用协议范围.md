---
title: TCP和UDP应用协议范围
date: 2018/3/5 10:36:54     
tags:
- TCP
- UDP
- TCP/IP协议
---

![TCP和UDP应用协议范围](https://i.imgur.com/AUHCePc.png)

#### 运行在TCP协议上的协议：

* HTTP（Hypertext Transfer Protocol，超文本传输协议），主要用于普通浏览。
* HTTPS（Hypertext Transfer Protocol over Secure Socket Layer, or HTTP over SSL，安全超文本传输协议）,HTTP协议的安全版本。
* FTP（File Transfer Protocol，文件传输协议），由名知义，用于文件传输。
* POP3（Post Office Protocol, version 3，邮局协议），收邮件用。
* SMTP（Simple Mail Transfer Protocol，简单邮件传输协议），用来发送电子邮件。
* TELNET（Teletype over the Network，网络电传），通过一个终端（terminal）登陆到网络。
* SSH（Secure Shell，用于替代安全性差的TELNET），用于加密安全登陆用。

#### 运行在UDP协议上的协议：

* BOOTP（Boot Protocol，启动协议），应用于无盘设备。
* NTP（Network Time Protocol，网络时间协议），用于网络同步。
* DHCP（Dynamic Host Configuration Protocol，动态主机配置协议），动态配置IP地址。
* 其他：
* DNS（Domain Name Service，域名服务），用于完成地址查找，邮件转发等工作（运行在TCP和UDP协议上）。
* ECHO（Echo Protocol，回绕协议），用于查错及测量应答时间（运行在TCP和UDP协议上）。
* SNMP（Simple Network Management Protocol，简单网络管理协议），用于网络信息的收集和网络管理。
* ARP（Address Resolution Protocol，地址解析协议），用于动态解析以太网硬件的地址。

#### TCP/IP协议栈中协议的层级：

    7	应用层   application layer	例如HTTP、SMTP、SNMP、FTP、Telnet、SIP、SSH、NFS、RTSP、XMPP、Whois、ENRP
    6	表示层   presentation layer	例如XDR、ASN.1、SMB、AFP、NCP
    5	会话层   session layer	例如ASAP、SSH、ISO 8327 / CCITT X.225、RPC、NetBIOS、ASP、IGMP、Winsock、BSD sockets
    4	传输层   transport layer	例如TCP、UDP、TLS、RTP、SCTP、SPX、ATP、IL
    3	网络层   network layer	例如IP、ICMP、IPX、BGP、OSPF、RIP、IGRP、EIGRP、ARP、RARP、X.25
    2	数据链路层 data link layer	例如以太网、令牌环、HDLC、帧中继、ISDN、ATM、IEEE 802.11、FDDI、PPP
    1	物理层   physical layer	例如线路、无线电、光纤