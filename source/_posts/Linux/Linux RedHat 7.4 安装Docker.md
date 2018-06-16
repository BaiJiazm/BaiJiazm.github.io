---
title: Linux RedHat 7.4 安装Docker
date: 2018/3/20 23:51:46 
tags:
- Linux
- Docker
---

本来Linux下安装就比较简单，奈何在Linux RedHat 7.4下安装Docer去遇到了小小麻烦，记录一下，希望对以后遇到此问题的人有帮助。

### 需要订阅时遇到问题

[root@RHEL74-SVR ~]# yum -y install docker      
已加载插件：langpacks, product-id, search-disabled-repos, subscription-manager
This system is not registered with an entitlement server. You can use subscription-manager to register.     
没有可用软件包 docker。     
错误：无须任何处理   

### Linux RedHat 7.4 安装Docker

#### 操作系统版本：
Red Hat Enterprise Linux Server release 7.4 (Maipo)

#### 添加yum源：
```Shell
vi /etc/yum.repos.d/docker.repo

[dockerrepo]
name=Docker Repository
baseurl=http://yum.dockerproject.org/repo/main/centos/7/
enabled=1
gpgcheck=0
```

#### 安装：
    yum install docker-engine -y

#### 启动Docker服务：
    systemctl start docker

#### 检查是否安装成功：
```Shell
[root@RHEL74-SVR ~]# docker version
Client:
 Version:      17.05.0-ce
 API version:  1.29
 Go version:   go1.7.5
 Git commit:   89658be
 Built:        Thu May  4 22:06:25 2017
 OS/Arch:      linux/amd64

Server:
 Version:      17.05.0-ce
 API version:  1.29 (minimum version 1.12)
 Go version:   go1.7.5
 Git commit:   89658be
 Built:        Thu May  4 22:06:25 2017
 OS/Arch:      linux/amd64
 Experimental: false
```

#### 查看Docker容器总体状况：
```Shell
[root@RHEL74-SVR ~]# docker info
Containers: 0
 Running: 0
 Paused: 0
 Stopped: 0
Images: 0
Server Version: 17.05.0-ce
Storage Driver: overlay
 Backing Filesystem: xfs
 Supports d_type: true
Logging Driver: json-file
Cgroup Driver: cgroupfs
Plugins: 
 Volume: local
 Network: bridge host macvlan null overlay
Swarm: inactive
Runtimes: runc
Default Runtime: runc
Init Binary: docker-init
containerd version: 9048e5e50717ea4497b757314bad98ea3763c145
runc version: 9c2d8d184e5da67c95d601382adf14862e4f2228
init version: 949e6fa
Security Options:
 seccomp
  Profile: default
Kernel Version: 3.10.0-693.el7.x86_64
Operating System: Red Hat Enterprise Linux Server 7.4 (Maipo)
OSType: linux
Architecture: x86_64
CPUs: 4
Total Memory: 976.3MiB
Name: RHEL74-SVR
ID: F3ES:DCYT:B6HM:Z5DG:GEWD:PJ2O:2GPW:K44O:M2OU:GOXQ:NYLY:YEVM
Docker Root Dir: /var/lib/docker
Debug Mode (client): false
Debug Mode (server): false
Registry: https://index.docker.io/v1/
Experimental: false
Insecure Registries:
 127.0.0.0/8
Live Restore Enabled: false
```

#### 设置开机自动启动：
```Shell
#默认是非开机自启的
systemctl disabled docker

#设置开机启动
systemctl enable docker
```