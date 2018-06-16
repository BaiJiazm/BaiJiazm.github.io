---
title: Linux 局域网localhost
date: 2018/2/5 16:01:29   
tags:
- Linux
---

背景：有一个部署在Linux虚拟机中的web应用，但是虚拟机没有安装图形界面只有字符界面，Linux作为服务器部署web应用，但是不能通过其自身localhost访问，不方便调试，如何能够让主机通过访问Linux虚拟机的局域网IP+端口访问web应用呢？

步骤如下：

Linux虚拟机为NAT或桥接模式，确保主机能够ping通；

    # vi /etc/hosts
      127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4  
      ::1         localhost localhost.localdomain localhost6 localhost6.localdomain6

修改为：

      192.168.80.21   localhost localhost.localdomain localhost4 localhost4.localdomain4
      ::1         localhost localhost.localdomain localhost6 localhost6.localdomain6

其中192.168.80.21是Linux虚拟机的局域网IP;

将web应用部署在localhost中；

打开主机浏览器，输入192.168.80.21:端口 即可访问；